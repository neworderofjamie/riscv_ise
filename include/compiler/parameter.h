#pragma once

// GeNN includes
#include "type.h"

// Compiler includes
#include "model_component.h"

//----------------------------------------------------------------------------
// Parameter
//----------------------------------------------------------------------------
class Parameter : public ModelComponent
{
public:
    Parameter(const GeNN::Type::NumericValue &value)
    :   m_Value(value)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getValue() const{ return m_Value; }

private:
    GeNN::Type::NumericValue m_Value;
};