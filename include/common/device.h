#pragma once

// Standard C++ includes
#include <optional>
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

    void setEnabled(bool enabled);
    void setILATrigger(bool enabled);

    void waitOnNonZero(uint32_t address) const;

    // Copy block of init data to startVectorDestPtr in vector memory using initialistion kernel. 
    // This works by copying blocks of data, starting at scratchScalarPtr in scalar memory up until it's end.
    // startVectorPtr and numVectorsScalarPtr point to words in memory used to communicate with kernel
    void runInit(const std::vector<uint8_t> &initData, uint32_t startVectorPtr, uint32_t numVectorsScalarPtr, 
                 uint32_t scratchScalarPtr, uint32_t startVectorDestPtr, uint32_t readyFlagPtr);

    void uploadCode(const std::vector<uint32_t> &code);

    void memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count);
    void memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint32_t *m_InstructionMemory;
    uint8_t *m_DataMemory;
    uint32_t *m_GPIO;
};
