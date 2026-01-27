#include "fenn/common/device.h"

// Standard C++ includes
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>

// Standard C includes
#include <cassert>
#include <cstring>

// POSIX includes
#ifdef __linux__ 
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/mman.h>
#endif

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

//----------------------------------------------------------------------------
// FeNN::Common::Device
//----------------------------------------------------------------------------
namespace FeNN::Common
{
Device::Device(int core, int numCores)
{
#ifdef __linux__ 
    const std::string targetNamePrefix = (numCores == 1) ? "" : ("core_" + std::to_string(core) + "_");
    LOGI << "Creating Device for core  " << core << " / " << numCores;
    
    // Create UIO
    m_InstructionMemoryUIO = std::make_unique<UIO>(targetNamePrefix + "axi_bram_ctrl_instr");
    m_DataMemoryUIO = std::make_unique<UIO>(targetNamePrefix + "axi_bram_ctrl_data");
    m_GPIOUIO = std::make_unique<UIO>(targetNamePrefix + "axi_gpio");

    // Create DMA controller
    m_DMAController = std::make_unique<DMAController>(targetNamePrefix + "dm_cmd_and_fsm");

#else
    throw std::runtime_error("Device interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
void Device::setEnabled(bool enabled)
{
    // Channel 1 AXI GPIO Data Register
    volatile uint32_t *gpio = getGPIO();
    gpio[0] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void Device::setILATrigger(bool enabled)
{
    // Channel 2 AXI GPIO Data Register.
    volatile uint32_t *gpio = getGPIO();
    gpio[2] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void Device::waitOnNonZero(uint32_t address) const
{
    // Wait until address is non-zero
    assert((address % 4) == 0);
    volatile const uint32_t *data = reinterpret_cast<volatile const uint32_t*>(getDataMemory() + address);
    while(*data == 0){
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
//----------------------------------------------------------------------------
void Device::uploadCode(const std::vector<uint32_t> &code)
{
    // Check there is space
    if(code.size() > (m_InstructionMemoryUIO->getSize() / 4)) {
        throw std::runtime_error("Insufficient code memory (" + std::to_string(m_InstructionMemoryUIO->getSize()) + " bytes)");
    }

    // Copy via volatile pointer to ensure no dumbness
    volatile uint32_t *iMem = getInstructionMemory();
    for(uint32_t c : code) { 
        *iMem++ = c;
    }
}
//----------------------------------------------------------------------------
void Device::memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count)
{
    // Check destination offset is valid
    if((destinationOffset + count) > m_DataMemoryUIO->getSize()) {
        throw std::runtime_error("Destination address out of range");
    }

    // Copy data from source to data memory
    volatile uint8_t *destination = getDataMemory() + destinationOffset;
    for(size_t i = 0; i < count; i++) {
        *destination++ = *source++;
    }
}
//----------------------------------------------------------------------------
void Device::memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const
{
    // Check source offset is valid
    if((sourceOffset + count) > m_DataMemoryUIO->getSize()) {
        throw std::runtime_error("Source address out of range");
    }

    // Copy data from source to data memory
    volatile const uint8_t *source = getDataMemory() + sourceOffset;
    for(size_t i = 0; i < count; i++) {
        *destination++ = *source++;
    }
}
//----------------------------------------------------------------------------
std::optional<unsigned int> Device::getSOCPower() const
{
    std::ifstream powerStream("/sys/class/hwmon/hwmon2/power1_input");
    if(powerStream.good()) {
        unsigned int power;
        powerStream >> power;
        return power;
    }
    else {
        return std::nullopt;
    }
}
}   // namespace FeNN::Common