#pragma once

// Standard C++ includes
#include <memory>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/events.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class EventSourceImplementation
{
public:
    //! Generate code to implement process
    virtual void generateCode() const{}
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceImplementation
//----------------------------------------------------------------------------
class EventSinkImplementation
{
public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        uint32_t varFieldOffset, std::optional<uint32_t> numTimesteps, 
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        Assembler::ScalarRegisterPtr numVariableBytes, bool hasTime, const Frontend::Model &model) const = 0;
    
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceBuffer
//----------------------------------------------------------------------------
class EventSourceBuffer : public Frontend::EventSourceBuffer, public EventSourceImplementation
{
public:
    EventSourceBuffer(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventSourceBuffer(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSourceBuffer> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventSourceBuffer>(Private(), shape, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannel
//----------------------------------------------------------------------------
class EventChannel : public Frontend::EventChannel, public EventSourceImplementation, public EventSinkImplementation
{
public:
    EventChannel(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventChannel(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Frontend::EventChannel> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventChannel>(Private(), shape, name);
    }
};
}