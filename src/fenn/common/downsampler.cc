#include "fenn/common/downsampler.h"

// Standard C++ includes
#include <limits>
#include <stdexcept>

// Standard C includes
#include <cassert>
#include <cmath>

// FeNN common includes
#include "fenn/common/logging.h"

//----------------------------------------------------------------------------
// FeNN::Common::Downsampler
//----------------------------------------------------------------------------
namespace FeNN::Common
{
Downsampler::Downsampler(const std::string &uioName)
:   m_RegisterUIO(uioName)
{
    LOGI_FENN_COMMON << "Creating Downsampler using UIO '" << uioName << "'";
}
//----------------------------------------------------------------------------
void Downsampler::setXScale(double xScale)
{
    // Validate scale
    if (xScale <= 0.0 || xScale > 1.0) {
        throw std::runtime_error("Invalid downsampler scale " + std::to_string(xScale));
    }

    // Obtain width by multiplying GenX320 width by scale
    const uint32_t width = static_cast<uint32_t>(std::round(320 * xScale));

    // Convert scale into S1.15 fixed point, pre-multiply by width and write to register
    writeReg(Register::X_SCALE,
             width * static_cast<uint32_t>(std::round(xScale * (1 << 15))));
}
//----------------------------------------------------------------------------
void Downsampler::setYScale(double yScale)
{
    // Validate scale
    if (yScale <= 0.0 || yScale > 1.0) {
        throw std::runtime_error("Invalid downsampler scale " + std::to_string(yScale));
    }

    // Convert scale into S1.15 fixed point and write to register
    writeReg(Register::Y_SCALE,
             static_cast<uint32_t>(std::round(yScale * (1 << 15))));
    
}
//----------------------------------------------------------------------------
void Downsampler::setPolarityControl(bool passOn, bool passOff, bool mergePolarities)
{
    // Set bits in value
    uint32_t value = 0;
    if (passOn) {
        value |= static_cast<uint32_t>(PolarityBits::PASS_ON);
    }
    if (passOff) {
        value |= static_cast<uint32_t>(PolarityBits::PASS_OFF);
    }
    if (mergePolarities) {
        value |= static_cast<uint32_t>(PolarityBits::MERGE_POLARITY);
    }

    // Write to register
    writeReg(Register::P_CONTROL, value);
}
//----------------------------------------------------------------------------
void Downsampler::setCoreRange(uint32_t core0Max, uint32_t core1Min)
{
    writeReg(Register::CORE_0_MAX, core0Max);
    writeReg(Register::CORE_1_MIN, core1Min);
}
//----------------------------------------------------------------------------
void Downsampler::writeReg(Register reg, uint32_t val)
{ 
    volatile uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    registers[static_cast<int>(reg) / 4] = val; 
}
//----------------------------------------------------------------------------
uint32_t Downsampler::readReg(Register reg) const
{ 
    volatile const uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    return registers[static_cast<int>(reg) / 4]; 
}
}   // namespace FeNN::Common
