#include "common/dma_controller.h"

// Standard C++ includes
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
DMAController::DMAController(int memory, size_t baseAddress, size_t destRegisterBaseAddress)
{
#ifdef __linux__ 
    LOGI << "Creating DMA at  " << baseAddress;

    // Memory map DMA controller registers
    m_Registers = reinterpret_cast<uint32_t*>(mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                   memory, baseAddress));
    if(m_Registers == MAP_FAILED) {
        throw std::runtime_error("DMA register map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map destination address register
    m_DestAddressRegister = reinterpret_cast<uint32_t*>(mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                             memory, destRegisterBaseAddress));
    if(m_DestAddressRegister == MAP_FAILED) {
        throw std::runtime_error("DMA destination address register map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
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

    LOGD << "Starting " << size << " byte DMA write from " << sourceAddress;
    if((destination & 63) != 0) {
        throw std::runtime_error("DMA writes to URAM must be 64 byte aligned");
    }
    
    if(size > ((1 << 19) - 1)) {
        throw std::runtime_error("Maximum size of DMA exceeded");
    }

    // Set destination address
    writeURAMDestinationAddress(destination);

    // Split into low and high words and write to registers
    writeReg(Register::MM2S_SA, static_cast<uint32_t>(sourceAddress & 0xFFFFFFFF));
    writeReg(Register::MM2S_SA_MSB, static_cast<uint32_t>(sourceAddress >> 32));

    // Run
    writeReg(Register::MM2S_DMACR, 1);
    // Set number of bytes
    writeReg(Register::MM2S_LENGTH, size);

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
/*bool DMAController::isReadIdle() const
{
    return (readReg(Register::S2MM_DMASR) & 0b10);
}*/
//----------------------------------------------------------------------------
void DMAController::waitForWriteComplete() const
{
    while(!isWriteIdle()) {
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
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
//----------------------------------------------------------------------------
void DMAController::writeURAMDestinationAddress(uint32_t val)
{
    volatile uint32_t *destAddressRegister = m_DestAddressRegister;
    destAddressRegister[0] = val; 
}
    
