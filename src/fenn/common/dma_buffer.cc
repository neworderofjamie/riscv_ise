#include "fenn/common/dma_buffer.h"

#ifdef __linux__     
    // POSIX includes
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>

    // UDMABUF includes
    #include "fenn/common/u-dma-buf-ioctl.h"
#endif

// FeNN common includes
#include "fenn/common/logging.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
template<typename T>
T getIOCtl(int file, unsigned long op)
{
#ifdef __linux__ 
    T val;
    if(ioctl(file, op, &val) == -1) {
        throw std::runtime_error("ioctl failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
    return val;
#else
    throw std::runtime_error("DMABuffer interface only supports Linux");
#endif
}

template<typename T>
void setIOCtl(int file, unsigned long op, const T &data)
{
#ifdef __linux__ 
    if(ioctl(file, op, &data) == -1) {
        throw std::runtime_error("ioctl failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
#else
    throw std::runtime_error("DMABuffer interface only supports Linux");
#endif
}
}

//----------------------------------------------------------------------------
// FeNN::Common::DMABuffer
//----------------------------------------------------------------------------
namespace FeNN::Common
{
DMABuffer::DMABuffer(int index)
:   m_Memory(-1),  m_Data(nullptr), m_PhysicalAddress(0), m_Size(0), m_Parent(nullptr)
{
#ifdef __linux__ 
    LOGI_FENN_COMMON << "Creating DMA buffer " << index;

    // Open memory
    // **NOTE** this is cached for performance and so we ARM can perform memset etc
    const std::string bufferFile = "/dev/udmabuf" + std::to_string(index);
    m_Memory = open(bufferFile.c_str(), O_RDWR);
    if(m_Memory == -1) {
        throw std::runtime_error(bufferFile + " open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Read size and physical address from device
    m_Size = getIOCtl<uint64_t>(m_Memory, U_DMA_BUF_IOCTL_GET_SIZE);
    m_PhysicalAddress = getIOCtl<uint64_t>(m_Memory, U_DMA_BUF_IOCTL_GET_DMA_ADDR);

    LOGD_FENN_COMMON << "\tPhysical address: " << std::hex << m_PhysicalAddress;
    LOGD_FENN_COMMON << "\tSize: " << m_Size << " bytes";

    // Configure manual synchronisation
    u_dma_buf_ioctl_sync_args syncArgs = {};

    // Manage the whole buffer
    syncArgs.offset = 0;
    syncArgs.size = m_Size;

    // Give device bi-direction access to buffer and start with CPU ownership
    SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_DIR(&syncArgs, 0);
    SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD(&syncArgs, U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD_FOR_CPU);

    // Send IOCtrl
    setIOCtl<u_dma_buf_ioctl_sync_args>(m_Memory, U_DMA_BUF_IOCTL_SET_SYNC, syncArgs);

    // Memory map data
    m_Data = reinterpret_cast<uint8_t*>(mmap(nullptr, m_Size, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                             m_Memory, 0));
    if(m_Data == MAP_FAILED) {
        throw std::runtime_error("Data map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
#else
    throw std::runtime_error("DMABuffer interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
DMABuffer::DMABuffer(DMABuffer &parent, uint64_t physicalStartAddress, uint64_t physicalEndAddress)
:   m_Memory(-1), m_Data(nullptr), m_PhysicalAddress(0), m_Size(0), m_Parent(&parent)
{
#ifdef __linux__ 
    LOGI_FENN_COMMON << "Creating child DMA buffer with target physical memory region: " << std::hex << physicalStartAddress << " - " << physicalEndAddress;

    // If parent DMA buffer does not overlap region
    const uint64_t parentPhysicalEndAddress = m_Parent->getPhysicalAddress() + m_Parent->getSize();
    if((m_Parent->getPhysicalAddress() > physicalEndAddress)
       || (parentPhysicalEndAddress < physicalStartAddress))
    {
        throw std::runtime_error("Parent DMA buffer does not overlap target physical memory region");
    }

    // Calculate start address
    m_PhysicalAddress = std::max(physicalStartAddress, m_Parent->getPhysicalAddress());
    LOGD_FENN_COMMON << "\tPhysical address: " << std::hex << m_PhysicalAddress;

    // Calculate end and hence sie
    const size_t actualEndAddress = std::min(physicalEndAddress, parentPhysicalEndAddress);
    m_Size = actualEndAddress - m_PhysicalAddress;
    LOGD_FENN_COMMON << "\tSize: " << m_Size << " bytes";

    // Calculate data pointer
    m_Data = m_Parent->getData() + (m_PhysicalAddress - m_Parent->getPhysicalAddress());
#else
    throw std::runtime_error("DMABuffer interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
DMABuffer::~DMABuffer()
{
#ifdef __linux__ 
    // If we are the parent DMA buffer
    if(!m_Parent) {
        // Unmap data
        munmap(m_Data, m_Size);

        // Close file
        close(m_Memory);
    }
#endif  // __linux__
}
//----------------------------------------------------------------------------
void DMABuffer::setAccessMode(AccessMode mode)
{
#ifdef __linux__ 
    if(m_Parent) {
        throw std::runtime_error("DMA buffer access mode can only be set on parent");
    }

    LOGD_FENN_COMMON << "Setting DMA buffer access mode '" << ((mode == AccessMode::CPU) ? "CPU" : "FeNN") << "'";
    
    // Set access mode
    const uint64_t arg = 1;
    setIOCtl<uint64_t>(
        m_Memory, (mode == AccessMode::CPU) ? U_DMA_BUF_IOCTL_SET_SYNC_FOR_CPU : U_DMA_BUF_IOCTL_SET_SYNC_FOR_DEVICE, 
        arg);

#else
    throw std::runtime_error("DMABuffer interface only supports Linux");
#endif  // __linux__
}
}   // namespace FeNN::Common