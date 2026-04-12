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
    //! Generate code to implement process
    virtual void generateCode() const{};
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventContainer
//----------------------------------------------------------------------------
class EventContainer : public Frontend::EventContainer, public EventSourceImplementation
{
public:
    EventContainer(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventContainer(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventContainer> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventContainer>(Private(), shape, name);
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