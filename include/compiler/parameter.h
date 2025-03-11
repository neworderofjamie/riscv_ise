#pragma once

// GeNN includes
#include "type.h"

//----------------------------------------------------------------------------
// Parameter
//----------------------------------------------------------------------------
class Parameter
{
public:
    Parameter(const GeNN::Type::NumericValue &value)
    :   m_Value(value)
    {}

    Parameter(const Parameter&) = delete;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getValue() const{ return m_Value; }

private:
    GeNN::Type::NumericValue m_Value;
};