#pragma once

// Standard C++ includes
#include <memory>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// DeviceControl
//----------------------------------------------------------------------------
class COMMON_EXPORT DeviceControl
{
public:
    DeviceControl(int numCores);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const volatile uint32_t *getGPIO() const{ return m_GPIOUIO->getData<uint32_t>(); }
    volatile uint32_t *getGPIO(){ return m_GPIOUIO->getData<uint32_t>(); }

    void setEnabled(bool enabled);
    void setILATrigger(bool enabled);

   
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<UIO> m_GPIOUIO;
};
