#include "common/dma_controller.h"

// Standard C++ includes
#include <limits>
#include <stdexcept>

// Standard C includes
#include <cassert>
#include <cstring>

// PLOG includes
#include <plog/Log.h>

#ifdef __linux__     
    // POSIX includes
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/mman.h>
#endif

// Common includes
#include "common/dma_buffer.h"

//----------------------------------------------------------------------------
// DMAController
//----------------------------------------------------------------------------
DMAController::DMAController(int memory, size_t baseAddress)
{
#ifdef __linux__ 
    LOGI << "Creating DMA at  " << baseAddress;

    // Memory map DMA controller registers
    m_Registers = reinterpret_cast<uint32_t*>(mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                   memory, baseAddress));
    if(m_Registers == MAP_FAILED) {
        throw std::runtime_error("DMA register map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
#else
    throw std::runtime_error("DMA controller interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
void DMAController::startWrite(uint32_t destination, const DMABuffer &sourceBuffer, size_t sourceOffset, size_t size)
{
    // Build 64-bit source address
    const uint64_t sourceAddress = sourceBuffer.getPhysicalAddress() + sourceOffset;
    assert((sourceOffset + size) < sourceBuffer.getSize());

    LOGD << "Starting " << size << " byte DMA write from " << sourceAddress;
    if((destination & 63) != 0) {
        throw std::runtime_error("DMA writes to URAM must be 64 byte aligned");
    }

    if((sourceAddress & 63) != 0) {
        throw std::runtime_error("DMA reads from mapped memory must be 64 byte aligned");
    }

    if(sourceAddress > std::numeric_limits<uint32_t>::max()) {
        throw std::runtime_error("DMA controller can only access 32-bit address space");
    }
    
    if(size > ((1 << 19) - 1)) {
        throw std::runtime_error("Maximum size of DMA exceeded");
    }

    // Write source and destination addresses to registers
    writeReg(Register::MM2S_SRC_ADDR, static_cast<uint32_t>(sourceAddress & 0xFFFFFFFF));
    writeReg(Register::MM2S_DST_ADDR, destination);

    // Write count to registers
    writeReg(Register::MM2S_COUNT, size);

    // Run
    writeReg(Register::MM2S_CONTROL, 1);
}
//----------------------------------------------------------------------------
/*void DMAController::startRead(DMABuffer &destBuffer, size_t destOffset, uint32_t source, size_t size)
{
    // Build 64-bit destination address
    const uint64_t destAddress = destBuffer.getPhysicalAddress() + destOffset;
    assert((destOffset + size) < destBuffer.getSize());

    LOGD << "Starting " << size << " byte DMA read to " << destAddress;

    // Split into low and high words and write to registers
    writeReg(Register::S2MM_DA, static_cast<uint32_t>(destAddress & 0xFFFFFFFF));
    writeReg(Register::S2MM_DA_MSB, static_cast<uint32_t>(destAddress >> 32));

    // Set number of bytes
    writeReg(Register::S2MM_LENGTH, size);

    // Run
    writeReg(Register::S2MM_DMACR, 1);
}*/
//----------------------------------------------------------------------------
void DMAController::waitForWriteComplete() const
{
    // Loop while DMA controller isn't idle
    uint32_t status;
    do {
        status = readReg(Register::MM2S_STATUS);
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    } while(!(status & static_cast<uint32_t>(StatusBits::STATE_IDLE)));
    
    // If decode error bit is set
    if(status & static_cast<uint32_t>(StatusBits::ERROR_DECODE)) {
        throw std::runtime_error("DMA transfer failed with decode error");
    }
    // Otherwise, if internal error bit is set
    else if(status & static_cast<uint32_t>(StatusBits::ERROR_INTERNAL)) {
        throw std::runtime_error("DMA transfer failed with internal error");
    }
    // Otherwise, if slave error bit is set
    else if(status & static_cast<uint32_t>(StatusBits::ERROR_SLAVE)) {
        throw std::runtime_error("DMA transfer failed with slave error");
    }
    // Otherwise, check transfer ok bit is set
    else {
        assert(status & static_cast<uint32_t>(StatusBits::TRANSFER_OK));
    }
}
//----------------------------------------------------------------------------
/*void DMAController::waitForReadComplete() const
{
    while(!isReadIdle()) {
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}*/
//----------------------------------------------------------------------------
void DMAController::writeReg(Register reg, uint32_t val)
{ 
    volatile uint32_t *registers = m_Registers;
    registers[static_cast<int>(reg) / 4] = val; 
}
//----------------------------------------------------------------------------
uint32_t DMAController::readReg(Register reg) const
{ 
    volatile const uint32_t *registers = m_Registers;
    return registers[static_cast<int>(reg) / 4]; 
}