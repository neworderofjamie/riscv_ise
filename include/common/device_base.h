#pragma once

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"

//----------------------------------------------------------------------------
// DeviceBase
//----------------------------------------------------------------------------
class COMMON_EXPORT DeviceBase
{
public:
	virtual ~DeviceBase() = default;

	//------------------------------------------------------------------------
	// Declared virtuals
	//------------------------------------------------------------------------
    const volatile uint32_t *getInstructionMemory() const = 0;
    const volatile uint8_t *getDataMemory() const = 0;
    const volatile uint32_t *getGPIO() const = 0;
    volatile uint32_t *getInstructionMemory() = 0;
    volatile uint8_t *getDataMemory() = 0;
    volatile uint32_t *getGPIO() = 0;
	
	size_t getInstructionMemorySize() const = 0;
	size_t getDataMemorySize() const = 0;
	size_t getGPIOSize const = 0;
	
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setEnabled(bool enabled);
    void setILATrigger(bool enabled);

    void waitOnNonZero(uint32_t address) const;

    void uploadCode(const std::vector<uint32_t> &code);

    void memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count);
    void memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const;
}