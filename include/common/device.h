#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/device_base.h"
#include "common/dma_controller.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// Device
//----------------------------------------------------------------------------
class COMMON_EXPORT Device : public DeviceBase
{
public:
    Device(int core = 0, int numCores = 1);

    //------------------------------------------------------------------------
    // DeviceBase virtuals
    //------------------------------------------------------------------------
    virtual const volatile uint32_t *getInstructionMemory() const override final{ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    virtual const volatile uint8_t *getDataMemory() const override final{ return m_DataMemoryUIO->getData<uint8_t>(); };
    virtual const volatile uint32_t *getGPIO() const override final{ return m_GPIOUIO->getData<uint32_t>(); }
    virtual volatile uint32_t *getInstructionMemory() override final{ return m_InstructionMemoryUIO->getData<uint32_t>(); }
    virtual volatile uint8_t *getDataMemory() override final{ return m_DataMemoryUIO->getData<uint8_t>(); };
    virtual volatile uint32_t *getGPIO() override final{ return m_GPIOUIO->getData<uint32_t>(); }

	virtual size_t getInstructionMemorySize() const override final{ return m_InstructionMemoryUIO->getSize(); }
	virtual size_t getDataMemorySize() const override final{ return m_DataMemoryUIO->getSize(); }
	virtual size_t getGPIOSize const override final{ return m_GPIOUIO->getSize(); }
	
	//------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    DMAController *getDMAController(){ return m_DMAController.get(); }
    const DMAController *getDMAController() const{ return m_DMAController.get(); }

    std::optional<unsigned int> getSOCPower() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<UIO> m_InstructionMemoryUIO;
    std::unique_ptr<UIO> m_DataMemoryUIO;
    std::unique_ptr<UIO> m_GPIOUIO;
    
    std::unique_ptr<DMAController> m_DMAController;
};
