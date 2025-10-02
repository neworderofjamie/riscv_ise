#include "common/device.h"

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

// RISC-V common includes
#include "common/dma_controller.h"
#include "common/utils.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
constexpr off_t instructionOffset = 0x00000000;
constexpr size_t instructionSize = 4 * 1024;

constexpr off_t dataOffset = 0x00100000;
constexpr size_t dataSize = 128 * 1024;

constexpr off_t gpioOffset = 0x00200000;
constexpr size_t gpioSize = 2 * 1024;

constexpr off_t dmaControllerOffset = 0x00300000;
}

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
Device::Device(size_t coreBaseAddress)
:   m_Memory(0), m_InstructionMemory(nullptr), m_DataMemory(nullptr), m_GPIO(nullptr)
{
#ifdef __linux__ 
    // Open memory
    // **NOTE** O_SYNC turns of caching
    m_Memory = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_Memory == -1) {
        throw std::runtime_error("/dev/mem open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map instruction memory
    m_InstructionMemory = reinterpret_cast<uint32_t*>(mmap(nullptr, instructionSize, PROT_WRITE, MAP_SHARED, 
                                                           m_Memory, coreBaseAddress + instructionOffset));
    if(m_InstructionMemory == MAP_FAILED) {
        throw std::runtime_error("ITCM map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map data memory
    m_DataMemory = reinterpret_cast<uint8_t*>(mmap(nullptr, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                   m_Memory, coreBaseAddress + dataOffset));
    if(m_DataMemory == MAP_FAILED) {
        throw std::runtime_error("DTCM map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map GPIO
    m_GPIO = reinterpret_cast<uint32_t*>(mmap(nullptr, gpioSize, PROT_READ | PROT_WRITE, MAP_SHARED,
                                              m_Memory, coreBaseAddress + gpioOffset));
    if(m_GPIO == MAP_FAILED) {
        throw std::runtime_error("GPIO map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Create DMA controller
    m_DMAController = std::make_unique<DMAController>(m_Memory, coreBaseAddress + dmaControllerOffset);

#else
    throw std::runtime_error("Device interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
Device::~Device()
{
#ifdef __linux__ 
    // Unmap memory
    munmap(m_GPIO, gpioSize);
    munmap(m_DataMemory, dataSize);
    munmap(m_InstructionMemory, instructionSize);
    
    // Close memory device
    close(m_Memory);
#endif  // __linux__
}
//----------------------------------------------------------------------------
void Device::setEnabled(bool enabled)
{
    // Channel 1 AXI GPIO Data Register
    volatile uint32_t *gpio = m_GPIO;
    gpio[0] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void Device::setILATrigger(bool enabled)
{
    // Channel 2 AXI GPIO Data Register.
    volatile uint32_t *gpio = m_GPIO;
    gpio[2] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void Device::waitOnNonZero(uint32_t address) const
{
    // Wait until address is non-zero
    assert((address % 4) == 0);
    volatile const uint32_t *data = reinterpret_cast<const uint32_t*>(m_DataMemory + address);
    while(*data == 0){
        //std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
//----------------------------------------------------------------------------
void Device::uploadCode(const std::vector<uint32_t> &code)
{
    // Check there is space
    if(code.size() > (instructionSize / 4)) {
        throw std::runtime_error("Insufficient code memory (" + std::to_string(instructionSize) + " bytes)");
    }

    // Copy via volatile pointer to ensure no dumbness
    volatile uint32_t *iMem = m_InstructionMemory;
    for(uint32_t c : code) { 
        *iMem++ = c;
    }
}
//----------------------------------------------------------------------------
void Device::memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count)
{
    // Check destination offset is valid
    if((destinationOffset + count) > dataSize) {
        throw std::runtime_error("Destination address out of range");
    }

    // Copy data from source to data memory
    volatile uint8_t *destination = m_DataMemory + destinationOffset;
    for(size_t i = 0; i < count; i++) {
        *destination++ = *source++;
    }
}
//----------------------------------------------------------------------------
void Device::memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const
{
    // Check source offset is valid
    if((sourceOffset + count) > dataSize) {
        throw std::runtime_error("Source address out of range");
    }

    // Copy data from source to data memory
    volatile const uint8_t *source = m_DataMemory + sourceOffset;
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
