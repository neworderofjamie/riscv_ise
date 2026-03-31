#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <vector>

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/device_base.h"

//----------------------------------------------------------------------------
// DeviceAlveo
//----------------------------------------------------------------------------
class COMMON_EXPORT DeviceAlveo : public DeviceBase
{
public:
    DeviceAlveo(int core = 0, int device = 0);
	~DeviceAlveo();
	
	//------------------------------------------------------------------------
    // DeviceBase virtuals
    //------------------------------------------------------------------------
    virtual const volatile uint32_t *getInstructionMemory() const override final;
    virtual const volatile uint8_t *getDataMemory() const override final;
    virtual const volatile uint32_t *getGPIO() const override final;
    virtual volatile uint32_t *getInstructionMemory() override final;
    virtual volatile uint8_t *getDataMemory() override final;
    virtual volatile uint32_t *getGPIO() override final;

	virtual size_t getInstructionMemorySize() const override final;
	virtual size_t getDataMemorySize() const override final;
	virtual size_t getGPIOSize const override final;
	
	
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //DMAController *getDMAController(){ return m_DMAController.get(); }
    //const DMAController *getDMAController() const{ return m_DMAController.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
	int m_Core;
    //std::unique_ptr<DMAController> m_DMAController;
	std::byte *m_CoreBypassData;
};
