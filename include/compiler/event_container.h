#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "model_component.h"
#include "shape.h"

//----------------------------------------------------------------------------
// EventContainer
//----------------------------------------------------------------------------
class EventContainer : public AcceptableModelComponent<EventContainer, State>
{
public:
    EventContainer(Private, const Shape &shape, const std::string &name)
    :   AcceptableModelComponent<EventContainer, State>(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventContainer> create(const Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventContainer>(Private(), shape, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
};