#include "common/device_base.h"

// Standard C includes
#include <cassert>
#include <cstring>

// PLOG includes
#include <plog/Log.h>

// RISC-V common includes
#include "common/utils.h"

//----------------------------------------------------------------------------
// DeviceBase
//----------------------------------------------------------------------------
void DeviceBase::setEnabled(bool enabled)
{
    // Channel 1 AXI GPIO Data Register
    volatile uint32_t *gpio = getGPIO();
    gpio[0] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void DeviceBase::setILATrigger(bool enabled)
{
    // Channel 2 AXI GPIO Data Register.
    volatile uint32_t *gpio = getGPIO();
    gpio[2] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void DeviceBase::waitOnNonZero(uint32_t address) const
{
    // Wait until address is non-zero
    assert((address % 4) == 0);
    volatile const uint32_t *data = reinterpret_cast<volatile const uint32_t*>(getDataMemory() + address);
    while(*data == 0){
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
//----------------------------------------------------------------------------
void DeviceBase::uploadCode(const std::vector<uint32_t> &code)
{
    // Check there is space
    if(code.size() > (getInstructionMemorySize() / 4)) {
        throw std::runtime_error("Insufficient code memory (" + std::to_string(m_InstructionMemoryUIO->getSize()) + " bytes)");
    }

    // Copy via volatile pointer to ensure no dumbness
    volatile uint32_t *iMem = getInstructionMemory();
    for(uint32_t c : code) { 
        *iMem++ = c;
    }
}
//----------------------------------------------------------------------------
void DeviceBase::memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count)
{
    // Check destination offset is valid
    if((destinationOffset + count) > getDataMemorySize()) {
        throw std::runtime_error("Destination address out of range");
    }

    // Copy data from source to data memory
    volatile uint8_t *destination = getDataMemory() + destinationOffset;
    for(size_t i = 0; i < count; i++) {
        *destination++ = *source++;
    }
}
//----------------------------------------------------------------------------
void DeviceBase::memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const
{
    // Check source offset is valid
    if((sourceOffset + count) > getDataMemorySize()) {
        throw std::runtime_error("Source address out of range");
    }

    // Copy data from source to data memory
    volatile const uint8_t *source = getDataMemory() + sourceOffset;
    for(size_t i = 0; i < count; i++) {
        *destination++ = *source++;
    }
}	
