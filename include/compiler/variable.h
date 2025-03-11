#pragma once

// Standard C++ includes
#include <vector>

// GeNN includes
#include "type.h"

// Compiler includes
#include "model_component.h"
#include "shape.h"

//----------------------------------------------------------------------------
// Variable
//----------------------------------------------------------------------------
class Variable : public AcceptableModelComponent<Variable>
{
public:
    Variable(const Shape &shape, const GeNN::Type::ResolvedType &type)
    :   m_Shape(shape), m_Type(type)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }
    const auto &getType() const{ return m_Type; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
    GeNN::Type::ResolvedType m_Type;
};