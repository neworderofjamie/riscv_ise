#include "common/device_alveo.h"

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
#include "common/utils.h"

namespace
{
constexpr size_t instructionMemorySize = 0;	// **TODO**
constexpr size_t dataMemorySize = 0;	// **TODO**
constexpr size_t gpioSize = 0;	// **TODO**

constexpr size_t coreMapSize = 1 * 1024 * 1024;
constexpr size_t instructionMemoryOffset = 0;	// **TODO**
constexpr size_t dataMemoryOffset = 0;	// **TODO**
constexpr size_t gpioOffset = 0;	// **TODO**
}
//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
DeviceAlveo::DeviceAlveo(int core, int device)
:	m_Core(core), m_BypassData(nullptr)
{
#ifdef __linux__ 
    LOGI << "Creating DeviceAlveo for core " << core << " on device  " << device;
    
	const std::string bypassFilename = "/dev/xdma" + std::to_string(device) + "_bypass";
    int memory = open(bypassFilename.c_str(), O_RDWR | O_SYNC);
    if(memory == -1) {
        throw std::runtime_error(bypassFilename + " open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
    
    // Memory map instruction memory
    m_CoreBypassData = mmap(nullptr, coreMapSize, PROT_WRITE, MAP_SHARED, 
						    memory, 0);
    if(m_CoreBypassData == MAP_FAILED) {
        throw std::runtime_error("Bypass map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Close memory device
    close(memory);
    
#else
    throw std::runtime_error("Device interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
DeviceAlveo::~DeviceAlveo()
{
#ifdef __linux__ 
    // Unmap memory
    munmap(m_CoreBypassData, coreMapSize);
#endif
}
//----------------------------------------------------------------------------
const volatile uint32_t *DeviceAlveo::getInstructionMemory() const
{
	return reinterpret_cast<uint32_t*>(m_BypassData + instructionMemoryOffset);
}
//----------------------------------------------------------------------------
const volatile uint8_t *DeviceAlveo::getDataMemory() const
{
	return reinterpret_cast<uint8_t>(m_CoreBypassData + dataMemoryOffset);
}
//----------------------------------------------------------------------------
const volatile uint32_t *DeviceAlveo::getGPIO() const
{
	return reinterpret_cast<uint32_t>(m_CoreBypassData + gpioOffset);
}
//----------------------------------------------------------------------------
volatile uint32_t *DeviceAlveo::getInstructionMemory()
{
	return reinterpret_cast<uint32_t*>(m_BypassData + instructionMemoryOffset);
}
//----------------------------------------------------------------------------
volatile uint8_t *DeviceAlveo::getDataMemory()
{
	return reinterpret_cast<uint8_t>(m_CoreBypassData + dataMemoryOffset);
}
//----------------------------------------------------------------------------
volatile uint32_t *DeviceAlveo::getGPIO()
{
	return reinterpret_cast<uint32_t>(m_CoreBypassData + gpioOffset);
}
//----------------------------------------------------------------------------
size_t DeviceAlveo::getInstructionMemorySize() const
{
	return instructionMemorySize;
}
//----------------------------------------------------------------------------
size_t DeviceAlveo::getDataMemorySize() const
{
	return dataMemorySize;
}
//----------------------------------------------------------------------------
size_t DeviceAlveo::getGPIOSize const
{
	return gpioSize;
}