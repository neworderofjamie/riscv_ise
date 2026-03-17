#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// EventStreamSmartTracker
//----------------------------------------------------------------------------
class COMMON_EXPORT EventStreamSmartTracker
{
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // Event Stream Smart Tracker registers
    enum class Register : int
    {
        CONTROL                     = 0x0,  // Controlling the IP basic bahavior 
        CONFIG                      = 0x4,  // Configuration of the IP; Bypassing the IP if needed.
        STATUS                      = 0x8,  // Status flags available for the IP
        VERSION                     = 0x10, // Register use to track the actual version number of the IP.
        
        SMART_DROPPER_CONTROL       = 0x14, // Register controlling the basic behavior of the Smart Dropper sub-core.
        SMART_DROPPER_TH_DROP_CNT   = 0x18, // Counter increments register when a drop of Time High event occurs
        SMART_DROPPER_TL_DROP_CNT   = 0x1C, // Counter increments register when a drop of Time Low event occurs
        SMART_DROPPER_EVT_DROP_CNT  = 0x20, // Counter increments register when a drop of any event occurs

        TH_RECOVERY_CONTROL         = 0x24, // Register controlling the behavior of the Time High Recovery sub-core.
        TS_CHECKER_CONTROL          = 0x28, // Register controlling the behavior of the TimeStamp Checker sub-core.
        TS_CHECKER_TH_DETECT_CNT    = 0x2C, // Register counting the number of jump in time between the two last Time High events
        TS_CHECKER_TH_CORRUPT_CNT   = 0x30, // Register counting the Time High corrupted events
        TS_CHECKER_TH_ERROR_CNT     = 0x34, // Register counting the unknown errors occuring in the Time High event flow
    };

    // Control register bits
    enum class ControlBits : uint32_t
    {
        ENABLE          = (1 << 0), // 1: ESST is enable (ready goes high), 0: ESST is disabled (ready stays low)
        GLOBAL_RESET    = (1 << 1), // 1: Reset the register and send signal to reset the data pipeline, 0: Nothing
        CLEAR           = (1 << 2), // 1: Reset the data pipeline only, 0: Release the data pipeline from reset
    };

    // Config register bits
    enum class ConfigBits : uint32_t
    {
        BYPASS  = (1 << 0), // 1: Put the core in bypass by connecting the AXI4-Stream master and slave interfaces.
    };

    // Status register bits
    enum class StatusBits : uint32_t
    {
        EVT_DROP_FLAG       = (1 << 0), // Indicates that an event is dropped. Clear when red.
        EVT_TH_DROP_FLAG    = (1 << 1), // Indicated that an event in TH Recovery was dropped. Clear when red
        GEN_TH_FLAG         = (1 << 2), // Indicated that an TH Event was generated. Clear when red
    };

    // Smart Dropper control register bits
    enum class SmartDropperControlBits : uint32_t
    {
        BYPASS          = (1 << 0), // Bypass the Smart Dropper
        GEN_OTHER_EVT   = (1 << 1), // Generate an OTHER Evt when an event is dropped
    };

    // TH recovery control register bits
    enum class THRecoveryControlBits : uint32_t
    {
        BYPASS          = (1 << 0), // Bypass the TH Recovery
        GEN_MISSING_TH  = (1 << 1), // Enable TH Recovery to add missing Th Markers in the event flow
        ENABLE_DROP_EVT = (1 << 2), // Enable TH Recovery to drop evts when errors in flow occurs (if jump backward in time)
        GEN_OTHER_EVT   = (1 << 3), // Generate an OTHER Evt when an event is dropped
    };

    // TS checker control register bits
    enum class TSCheckerControlBits : uint32_t
    {
        BYPASS              = (1 << 0), // Bypass the TS Checker
        ENABLE_DROP_EVT     = (1 << 1), // Enabling TS Checker to drop evts if TH diff > threshold
        GEN_OTHER_EVT       = (1 << 2), // Generate an OTHER Evt when an event is dropped
        GEN_TLAST_ON_OTHER  = (1 << 3), // Generate an the tlast signal aligned with the OTHER event
    };

public:
    EventStreamSmartTracker(const std::string &uioName);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    // Control register
    void setEnabled(bool enable){ setRegBit(Register::CONTROL, ControlBits::ENABLE, enable); }
    bool isEnabled() const{ return getRegBit(Register::CONTROL, ControlBits::ENABLE); }

    void setGlobalReset(bool globalReset) { setRegBit(Register::CONTROL, ControlBits::GLOBAL_RESET, globalReset); }
    void setClear(bool clear) { setRegBit(Register::CONTROL, ControlBits::CLEAR, clear); }

    // Config register
    void setBypased(bool bypass) { setRegBit(Register::CONFIG, ConfigBits::BYPASS, bypass); }
    bool isBypassed() const{ return getRegBit(Register::CONFIG, ConfigBits::BYPASS); }

    // Status register
    bool wasEventDropped() const{ return getRegBit(Register::STATUS, StatusBits::EVT_DROP_FLAG); }
    bool wasTHEventDropped() const{ return getRegBit(Register::STATUS, StatusBits::EVT_TH_DROP_FLAG); }
    bool wasTHEventGenerated() const{ return getRegBit(Register::STATUS, StatusBits::GEN_TH_FLAG); }

    // Smart Dropper control
    void setSmartDropperBypassed(bool bypass){ setRegBit(Register::SMART_DROPPER_CONTROL, SmartDropperControlBits::BYPASS, bypass); }
    bool isSmartDropperBypassed() const{ return getRegBit(Register::SMART_DROPPER_CONTROL, SmartDropperControlBits::BYPASS); }
    
    void setSmartDropperGenOtherEvent(bool genOther){ setRegBit(Register::SMART_DROPPER_CONTROL, SmartDropperControlBits::GEN_OTHER_EVT, genOther); }
    bool shouldSmartDroppedGenOtherEvent() const{ return getRegBit(Register::SMART_DROPPER_CONTROL, SmartDropperControlBits::GEN_OTHER_EVT); }

    uint32_t getSmartDropperTHDropCount() const{ return readReg(Register::SMART_DROPPER_TH_DROP_CNT); }
    uint32_t getSmartDropperTLDropCount() const{ return readReg(Register::SMART_DROPPER_TL_DROP_CNT); }
    uint32_t getSmartDropperEventDropCount() const{ return readReg(Register::SMART_DROPPER_EVT_DROP_CNT); }

    // TH recovery control
    void setTHRecoveryBypassed(bool bypass){ setRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::BYPASS, bypass); }
    bool isTHRecoveryBypassed() const{ return getRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::BYPASS); }
    
    void setTHRecoveryGenMissingTh(bool genMissing){ setRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::GEN_MISSING_TH, genMissing); }
    bool shouldTHRecoveryGenMissingTh() const{ return getRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::GEN_MISSING_TH); }

    void setTHRecoveryEnableDropEvent(bool enableDrop){ setRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::ENABLE_DROP_EVT, enableDrop); }
    bool shouldTHRecoveryEnableDropEvent() const{ return getRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::ENABLE_DROP_EVT); }
    
    void setTHRecoveryGenOtherEvent(bool genOther){ setRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::GEN_OTHER_EVT, genOther); }
    bool shouldTHRecoveryGenOtherEvent() const{ return getRegBit(Register::TH_RECOVERY_CONTROL, THRecoveryControlBits::GEN_OTHER_EVT); }

    // TS checker
    void setTSCheckerBypassed(bool bypass){ setRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::BYPASS, bypass); }
    bool isTSCheckerBypassed() const{ return getRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::BYPASS); }
    
    void setTSCheckerEnableDropEvent(bool enableDrop){ setRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::ENABLE_DROP_EVT, enableDrop); }
    bool shouldTSCheckerEnableDropEvent() const{ return getRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::ENABLE_DROP_EVT); }
    
    void setTSCheckerGenOtherEvent(bool genOther){ setRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::GEN_OTHER_EVT, genOther); }
    bool shouldTSCheckerGenOtherEvent() const{ return getRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::GEN_OTHER_EVT); }

    void setTSCheckerGenTLastOnOther(bool genTLas){ setRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::GEN_TLAST_ON_OTHER, genTLas); }
    bool shouldTSCheckerTLastOnOther() const{ return getRegBit(Register::TS_CHECKER_CONTROL, TSCheckerControlBits::GEN_TLAST_ON_OTHER); }

    void setTSCheckerThreshold(uint32_t thereshold);

    uint32_t getTSCheckerTHDetectCount() const{ return readReg(Register::TS_CHECKER_TH_DETECT_CNT); }
    uint32_t getTSCheckerTHCorruptCount() const{ return readReg(Register::TS_CHECKER_TH_CORRUPT_CNT); }
    uint32_t getTSCheckerTHErrorCount() const{ return readReg(Register::TS_CHECKER_TH_ERROR_CNT); }

private:
    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    //! Get a typed bit from an enumerated register
    template<typename B>
    bool getRegBit(Register reg, B bit) const
    {
        return ((readReg(reg) & static_cast<uint32_t>(bit)) != 0);
    }
    
    //! Set a typed bit in an enumerated register
    template<typename B>
    void setRegBit(Register reg, B bit, bool set)
    {
        // Read old value
        uint32_t oldVal = readReg(reg);

        // Either set or toggle bit in value
        if(set) {
            oldVal |= static_cast<uint32_t>(bit);
        }
        else {
            oldVal &= ~static_cast<uint32_t>(bit);
        }

        // Write updated value
        writeReg(reg, oldVal);
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    UIO m_RegisterUIO;
};