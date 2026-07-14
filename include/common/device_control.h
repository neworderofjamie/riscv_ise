#pragma once

// Standard C++ includes
#include <memory>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/genx_320.h"
#include "common/mipi_csi2_receiver.h"
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

    MIPICSI2Receiver *getMIPICSI2Receiver(){ return m_MIPICSI2Receiver.get(); }
    const MIPICSI2Receiver *getMIPICSI2Receiver() const{ return m_MIPICSI2Receiver.get(); }

    GenX320 *getGenX320(){ return m_GenX320.get(); }
    const GenX320 *getGenX320() const{ return m_GenX320.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<UIO> m_GPIOUIO;
    std::unique_ptr<MIPICSI2Receiver> m_MIPICSI2Receiver;
    std::unique_ptr<GenX320> m_GenX320;
};
