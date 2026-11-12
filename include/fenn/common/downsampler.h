#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// FeNN common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/uio.h"

//----------------------------------------------------------------------------
// FeNN::Common::Downsampler
//----------------------------------------------------------------------------
namespace FeNN::Common
{
class FENN_COMMON_EXPORT Downsampler
{
public:
    Downsampler(const std::string &uioName);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setXScale(double xScale);
    void setYScale(double yScale);
    void setPolarityControl(bool passOn, bool passOff, bool mergePolarities);
    void setCoreRange(uint32_t core0Max, uint32_t core1Min);

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // DMA controller registers
    enum class Register : int
    {
        P_CONTROL   = 0x0,  // Polarity Control
        X_SCALE     = 0x4,  // Scale factor to apply to X coordinate
        Y_SCALE     = 0x8,  // Scale factor to apply to Y coordinates
        CORE_0_MAX  = 0xC,  // Maximum output ID to route to core 0
        CORE_1_MIN  = 0x10, // Minimum output ID to route to core 1
    };

    // Status register bits
    enum class PolarityBits : uint32_t
    {
        PASS_ON         = (1 << 0), // Should CD_ON events be passed through (rather than dropped)
        PASS_OFF        = (1 << 1), // Should CD_OFF events be passed through (rather than dropped)
        MERGE_POLARITY  = (1 << 2), // Should CD_ON and CD_OFF events be merged
    };

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    UIO m_RegisterUIO;
};
}   // namespace FeNN::Common