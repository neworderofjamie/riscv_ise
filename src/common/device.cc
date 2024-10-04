#include "common/device.h"

// Standard C++ includes
#include <chrono>
#include <stdexcept>
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

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
constexpr off_t instructionBase = 0xA5000000;
constexpr size_t instructionSize = 4 * 1024;

constexpr off_t dataBase = 0xA6000000;
constexpr size_t dataSize = 128 * 1024;

constexpr off_t gpioBase = 0xA7000000;
constexpr size_t gpioSize = 2 * 1024;
}

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
Device::Device()
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
                                                           m_Memory, instructionBase));
    if(m_InstructionMemory == MAP_FAILED) {
        throw std::runtime_error("ITCM map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map data memory
    m_DataMemory = reinterpret_cast<uint32_t*>(mmap(nullptr, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, 
                                                    m_Memory, dataBase));
    if(m_DataMemory == MAP_FAILED) {
        throw std::runtime_error("DTCM map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Memory map GPIO
    m_GPIO = reinterpret_cast<uint32_t*>(mmap(nullptr, gpioSize, PROT_READ | PROT_WRITE, MAP_SHARED,
                                              m_Memory, gpioBase));
    if(m_GPIO == MAP_FAILED) {
        throw std::runtime_error("GPIO map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
#elif
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
void Device::setReset(bool reset)
{
    m_GPIO[0] = reset ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void Device::waitOnNonZero(uint32_t address)
{
    // Wait until address is non-zero
    assert((address % 4) == 0);
    volatile const uint32_t *data = &m_DataMemory[address / 4];
    while(*data == 0){
        std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
