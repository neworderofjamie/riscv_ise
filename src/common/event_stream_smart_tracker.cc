#include "common/event_stream_smart_tracker.h"

// Standard C includes
#include <cassert>

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// EventStreamSmartTracker
//----------------------------------------------------------------------------
EventStreamSmartTracker::EventStreamSmartTracker(const std::string &uioName)
:   m_RegisterUIO(uioName)
{
    LOGI << "Event Stream Smart Tracker using UIO '" << uioName << "'";

    const uint32_t version = readReg(Register::VERSION);
    LOGI << "IP version " << (version >> 16) << "." << (version & 0xFFFF) <<"'";
}
//----------------------------------------------------------------------------
void EventStreamSmartTracker::setTSCheckerThreshold(uint32_t threshold)
{
    // Read old value
    uint32_t oldVal = readReg(Register::TS_CHECKER_CONTROL);

    // OR 4 lowest bits of TS checker control with new threshold
    assert(threshold < ((2 << 28) - 1));
    oldVal = (oldVal & 0b1111) | (threshold << 4);

    // Write updated value
    writeReg(Register::TS_CHECKER_CONTROL, oldVal);
}
//----------------------------------------------------------------------------
void EventStreamSmartTracker::writeReg(Register reg, uint32_t val)
{ 
    volatile uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    registers[static_cast<int>(reg) / 4] = val; 
}
//----------------------------------------------------------------------------
uint32_t EventStreamSmartTracker::readReg(Register reg) const
{ 
    volatile const uint32_t *registers = m_RegisterUIO.getData<uint32_t>();
    return registers[static_cast<int>(reg) / 4]; 
}