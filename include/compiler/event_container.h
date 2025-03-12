#pragma once

// Standard C++ includes
#include <vector>

// Compiler includes
#include "model_component.h"
#include "shape.h"

//----------------------------------------------------------------------------
// EventContainer
//----------------------------------------------------------------------------
class EventContainer : public AcceptableModelComponent<EventContainer>
{
public:
    EventContainer(const Shape &shape, const std::string &name = "")
    :   AcceptableModelComponent<EventContainer>(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
};