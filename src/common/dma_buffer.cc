#include "common/dma_buffer.h"

// PLOG includes
#include <plog/Log.h>

#ifdef __linux__     
    // POSIX includes
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>

    // UDMABUF includes
    #include "common/u-dma-buf-ioctl.h"
#endif

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
}

//----------------------------------------------------------------------------
// DMABuffer
//----------------------------------------------------------------------------
DMABuffer::DMABuffer(int index)
    : m_Memory(0), m_Data(nullptr), m_PhysicalAddress(0), m_Size(0)
{
#ifdef __linux__ 
    LOGI << "Creating DMA buffer " << index;

    // Open memory
    const std::string bufferFile = "/dev/udmabuf" + std::to_string(index);
    m_Memory = open(bufferFile.c_str(), O_RDWR | O_SYNC);
    if(m_Memory == -1) {
        throw std::runtime_error(bufferFile + " open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Read size and physical address from device
    m_Size = getIOCtl<uint64_t>(m_Memory, U_DMA_BUF_IOCTL_GET_SIZE);
    m_PhysicalAddress = getIOCtl<uint64_t>(m_Memory, U_DMA_BUF_IOCTL_GET_DMA_ADDR);

    LOGD << "\tPhysical address: " << std::hex << m_PhysicalAddress;
    LOGD << "\tSize: " << m_Size << " bytes";

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
DMABuffer::~DMABuffer()
{
#ifdef __linux__ 
    // Unmap memory
    munmap(m_Data, m_Size);
    
    // Close memory device
    close(m_Memory);
#endif  // __linux__
}

