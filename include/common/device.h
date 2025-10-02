#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"

// Forward declarations
class DMAController;

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
class COMMON_EXPORT Device
{
public:
    Device(off_t coreBaseAddress = 0x80000000);
    ~Device();

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const volatile uint32_t *getInstructionMemory() const{ return m_InstructionMemory; }
    const volatile uint8_t *getDataMemory() const{ return m_DataMemory; };
    const volatile uint32_t *getGPIO() const{ return m_GPIO; }
    volatile uint32_t *getInstructionMemory(){ return m_InstructionMemory; }
    volatile uint8_t *getDataMemory(){ return m_DataMemory; };
    volatile uint32_t *getGPIO(){ return m_GPIO; }

    DMAController *getDMAController(){ return m_DMAController.get(); }
    const DMAController *getDMAController() const{ return m_DMAController.get(); }

    void setEnabled(bool enabled);
    void setILATrigger(bool enabled);

    void waitOnNonZero(uint32_t address) const;

    void uploadCode(const std::vector<uint32_t> &code);

    void memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count);
    void memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const;

    std::optional<unsigned int> getSOCPower() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint32_t *m_InstructionMemory;
    uint8_t *m_DataMemory;
    uint32_t *m_GPIO;

    std::unique_ptr<DMAController> m_DMAController;
};
