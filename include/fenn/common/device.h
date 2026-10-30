#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/dma_controller.h"
#include "fenn/common/uio.h"

//----------------------------------------------------------------------------
// FeNN::Common::Device
//----------------------------------------------------------------------------
namespace FeNN::Common
{
class FENN_COMMON_EXPORT Device
{
public:
    Device(int core = 0, int numCores = 1);

    //------------------------------------------------------------------------
    // ExceptionData
    //------------------------------------------------------------------------
    struct ExceptionData
    {
        uint32_t registers[31];
        uint32_t cause;
        uint32_t pc;
    };
    static_assert(sizeof(ExceptionData) == 132);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const volatile uint32_t *getInstructionMemory() const{ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    const volatile uint8_t *getDataMemory() const{ return m_DataMemoryUIO->getData<uint8_t>(); };
    volatile uint32_t *getInstructionMemory(){ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    volatile uint8_t *getDataMemory(){ return m_DataMemoryUIO->getData<uint8_t>(); };

    DMAController *getDMAController(){ return m_DMAController.get(); }
    const DMAController *getDMAController() const{ return m_DMAController.get(); }

    //! Wait for ready flag to be set - returns true if successful, else false
    bool waitOnNonZero(uint32_t address) const;

    //! If waitOnNonZero returns error, read exception data
    ExceptionData getExceptionData(uint32_t address) const;

    void uploadCode(const std::vector<uint32_t> &code);

    void memcpyDataToDevice(size_t destinationOffset, const uint8_t *source, size_t count);
    void memcpyDataFromDevice(uint8_t *destination, size_t sourceOffset, size_t count) const;

    std::optional<unsigned int> getSOCPower() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<UIO> m_InstructionMemoryUIO;
    std::unique_ptr<UIO> m_DataMemoryUIO;

    std::unique_ptr<DMAController> m_DMAController;
};
}