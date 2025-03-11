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
    EventContainer(const Shape &shape)
    :   m_Shape(shape)
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