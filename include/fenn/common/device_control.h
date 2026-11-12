#pragma once

// Standard C++ includes
#include <memory>

// Standard C includes
#include <cstdint>

// Common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/uio.h"

// Forward includes
namespace FeNN::Common
{
class Downsampler;
class GenX320;
class MIPICSI2Receiver;
}

//----------------------------------------------------------------------------
// FeNN::Common::DeviceControl
//----------------------------------------------------------------------------
namespace FeNN::Common
{
class FENN_COMMON_EXPORT DeviceControl
{
public:
    DeviceControl(int numCores = 1);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setEnabled(bool enabled);
    void setBootAddress(uint32_t bootAddress);

    MIPICSI2Receiver *getMIPICSI2Receiver(){ return m_MIPICSI2Receiver.get(); }
    const MIPICSI2Receiver *getMIPICSI2Receiver() const{ return m_MIPICSI2Receiver.get(); }

    GenX320 *getGenX320(){ return m_GenX320.get(); }
    const GenX320 *getGenX320() const{ return m_GenX320.get(); }

    Downsampler *getDownsampler(){ return m_Downsampler.get(); }
    const Downsampler *getDownsampler() const{ return m_Downsampler.get(); }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    const volatile uint32_t *getGPIO() const{ return m_GPIOUIO->getData<uint32_t>(); }
    volatile uint32_t *getGPIO(){ return m_GPIOUIO->getData<uint32_t>(); }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<UIO> m_GPIOUIO;
    std::unique_ptr<MIPICSI2Receiver> m_MIPICSI2Receiver;
    std::unique_ptr<GenX320> m_GenX320;
    std::unique_ptr<Downsampler> m_Downsampler;
};
}