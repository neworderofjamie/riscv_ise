#include "common/mipi_csi2_receiver.h"

// Standard C includes
#include <cassert>

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// MIPICSI2Receiver
//----------------------------------------------------------------------------
MIPICSI2Receiver::MIPICSI2Receiver(const std::string &uioName)
:   m_RegisterUIO(uioName)
{
    LOGI << "MIPI CSI-2 Receiver  using UIO '" << uioName << "'";
}
//----------------------------------------------------------------------------
uint32_t MIPICSI2Receiver::getMaxLanes() const
{
    // Maximum lanes of the core
    // 0x0 - 1 Lane
    // 0x1 - 2 Lanes
    // 0x2 - 3 Lanes
    // 0x3 - 4 Lanes 
    return ((readReg(Register::PROTOCOL_CONFIG) >> 3) & 0b11) + 1;
}
//----------------------------------------------------------------------------
uint32_t MIPICSI2Receiver::getActiveLanes() const
{
    // Active lanes of the core
    // 0x0 - 1 Lane
    // 0x1 - 2 Lanes
    // 0x2 - 3 Lanes
    // 0x3 - 4 Lanes 
    return (readReg(Register::PROTOCOL_CONFIG) & 0b11) + 1;
}
//----------------------------------------------------------------------------
uint32_t MIPICSI2Receiver::getPacketCount() const
{
    return (readReg(Register::CORE_STATUS) >> 16);
}
//----------------------------------------------------------------------------
void MIPICSI2Receiver::writeReg(Register reg, uint32_t val)
{ 
    volatile uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    registers[static_cast<int>(reg) / 4] = val; 
}
//----------------------------------------------------------------------------
uint32_t MIPICSI2Receiver::readReg(Register reg) const
{ 
    volatile const uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    return registers[static_cast<int>(reg) / 4]; 
}