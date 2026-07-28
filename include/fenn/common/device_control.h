#pragma once

// Standard C++ includes
#include <memory>

// Standard C includes
#include <cstdint>

// Common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/genx_320.h"
#include "fenn/common/mipi_csi2_receiver.h"
#include "fenn/common/uio.h"


//----------------------------------------------------------------------------
// FeNN::Common::DeviceControl
//----------------------------------------------------------------------------
namespace FeNN::Common
{
class FENN_COMMON_EXPORT DeviceControl
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
}