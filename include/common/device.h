#pragma once

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
    const uint32_t *getInstructionMemory() const{ return m_InstructionMemory; }
    const uint32_t *getDataMemory() const{ return m_DataMemory; };
    const uint32_t *getGPIO() const{ return m_GPIO; }
    uint32_t *getInstructionMemory(){ return m_InstructionMemory; }
    uint32_t *getDataMemory(){ return m_DataMemory; };
    uint32_t *getGPIO(){ return m_GPIO; }

    void setReset(bool reset);

    void waitOnNonZero(uint32_t address);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint32_t *m_InstructionMemory;
    uint32_t *m_DataMemory;
    uint32_t *m_GPIO;
};