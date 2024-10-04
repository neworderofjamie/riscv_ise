#pragma once

// Standard C++ includes
#include <vector>

// Standard C includes
#include <cstdint>

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
class Device
{
public:
    Device();
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

    void setReset(bool reset);

    void waitOnNonZero(uint32_t address) const;

    void uploadCode(const std::vector<uint32_t> &code);
    void uploadData(const std::vector<uint8_t> &code);
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint32_t *m_InstructionMemory;
    uint8_t *m_DataMemory;
    uint32_t *m_GPIO;
};
