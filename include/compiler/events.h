#pragma once

// Standard C++ includes
#include <vector>

// Compiler includes
#include "shape.h"

//----------------------------------------------------------------------------
// Events
//----------------------------------------------------------------------------
class Events
{
public:
    Events(const Shape &shape)
    :   m_Shape(shape)
    {}

    Events(const Events&) = delete;

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