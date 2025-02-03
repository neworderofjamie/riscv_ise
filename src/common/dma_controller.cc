#include "common/dma_controller.h"

// Standard C++ includes
#include <stdexcept>

// Standard C includes
#include <cassert>
#include <cstring>

#ifdef __linux__     
    // POSIX includes
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/ioctl.h>
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
    // Memory map DMA controller registers
    m_Registers = reinterpret_cast<uint32_t*>(mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                   memory, baseAddress));
    if(m_Registers == MAP_FAILED) {
        throw std::runtime_error("DMA register map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Reset DMA controller into known state
    reset();
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

    // Split into low and high words and write to registers
    writeReg(Register::MM2S_SA, static_cast<uint32_t>(sourceAddress & 0xFFFFFFFF));
    writeReg(Register::MM2S_SA_MSB, static_cast<uint32_t>(sourceAddress >> 32));

    // Set number of bytes
    writeReg(Register::MM2S_LENGTH, size);

    // Run
    writeReg(Register::MM2S_DMACR, 1);
}
//----------------------------------------------------------------------------
void DMAController::startRead(DMABuffer &destBuffer, size_t destOffset, uint32_t source, size_t size)
{
    // Build 64-bit destination address
    const uint64_t destAddress = destBuffer.getPhysicalAddress() + destOffset;
    assert((destOffset + size) < destBuffer.getSize());

    // Split into low and high words and write to registers
    writeReg(Register::S2MM_DA, static_cast<uint32_t>(destAddress & 0xFFFFFFFF));
    writeReg(Register::S2MM_DA_MSB, static_cast<uint32_t>(destAddress >> 32));

    // Set number of bytes
    writeReg(Register::S2MM_LENGTH, size);

    // Run
    writeReg(Register::S2MM_DMACR, 1);
}
//----------------------------------------------------------------------------
void DMAController::reset()
{
    // Set reset flag for both channels
    // **NOTE** pretty sure this is unecessary
    writeReg(Register::MM2S_DMACR, 0b100);
    writeReg(Register::S2MM_DMACR, 0b100);

    // Clear reset flag again to re-enable normal operation
    writeReg(Register::MM2S_DMACR, 0);
    writeReg(Register::S2MM_DMACR, 0);
}
//----------------------------------------------------------------------------
bool DMAController::isWriteIdle() const
{
    return (readReg(Register::MM2S_DMASR) & 0b10);
}
//----------------------------------------------------------------------------
bool DMAController::isReadIdle() const
{
    return (readReg(Register::S2MM_DMASR) & 0b10);
}
//----------------------------------------------------------------------------
void DMAController::waitForWriteComplete() const
{
    while(!isWriteIdle()) {
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
//----------------------------------------------------------------------------
void DMAController::waitForReadComplete() const
{
    while(!isReadIdle()) {
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
//----------------------------------------------------------------------------