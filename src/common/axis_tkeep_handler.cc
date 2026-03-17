#include "common/axis_tkeep_handler.h"

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// AxisTkeepHandler
//----------------------------------------------------------------------------
AxisTkeepHandler::AxisTkeepHandler(const std::string &uioName)
:   m_RegisterUIO(uioName)
{
    LOGI << "AXIS tkeep Handler using UIO '" << uioName << "'";

    const uint32_t version = readReg(Register::VERSION);
    LOGI << "IP version " << (version >> 16) << "." << (version & 0xFFFF) <<"'";
}
//----------------------------------------------------------------------------
void AxisTkeepHandler::writeReg(Register reg, uint32_t val)
{ 
    volatile uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    registers[static_cast<int>(reg) / 4] = val; 
}
//----------------------------------------------------------------------------
uint32_t AxisTkeepHandler::readReg(Register reg) const
{ 
    volatile const uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    return registers[static_cast<int>(reg) / 4]; 
}
//----------------------------------------------------------------------------
bool AxisTkeepHandler::getControlBit(ControlBits controlBit) const
{
    return ((readReg(Register::CONTROL) & static_cast<uint32_t>(controlBit)) != 0);
}
//----------------------------------------------------------------------------
void AxisTkeepHandler::setControlBit(ControlBits controlBit, bool set)
{
    // Read old control value
    uint32_t oldControl = readReg(Register::CONTROL);

    // Either set or toggle bit in value
    if(set) {
        oldControl |= static_cast<uint32_t>(controlBit);
    }
    else {
         oldControl &= ~static_cast<uint32_t>(controlBit);
    }

    // Write updated value
    writeReg(Register::CONTROL, oldControl);
}