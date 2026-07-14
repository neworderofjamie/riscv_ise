#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/dma_controller.h"
#include "common/genx_320.h"
#include "common/mipi_csi2_receiver.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
class COMMON_EXPORT Device
{
public:
    Device(int core = 0, int numCores = 1);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const volatile uint32_t *getInstructionMemory() const{ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    const volatile uint8_t *getDataMemory() const{ return m_DataMemoryUIO->getData<uint8_t>(); };
    volatile uint32_t *getInstructionMemory(){ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    volatile uint8_t *getDataMemory(){ return m_DataMemoryUIO->getData<uint8_t>(); };

    DMAController *getDMAController(){ return m_DMAController.get(); }
    const DMAController *getDMAController() const{ return m_DMAController.get(); }

    MIPICSI2Receiver *getMIPICSI2Receiver(){ return m_MIPICSI2Receiver.get(); }
    const MIPICSI2Receiver *getMIPICSI2Receiver() const{ return m_MIPICSI2Receiver.get(); }

    GenX320 *getGenX320(){ return m_GenX320.get(); }
    const GenX320 *getGenX320() const{ return m_GenX320.get(); }

    void waitOnNonZero(uint32_t address) const;

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

    std::unique_ptr<MIPICSI2Receiver> m_MIPICSI2Receiver;
    std::unique_ptr<GenX320> m_GenX320;
};
