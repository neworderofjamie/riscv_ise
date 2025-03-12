#pragma once

// GeNN includes
#include "type.h"

// Compiler includes
#include "model_component.h"

//----------------------------------------------------------------------------
// Parameter
//----------------------------------------------------------------------------
class Parameter : public AcceptableModelComponent<Parameter>
{
public:
    Parameter(const GeNN::Type::NumericValue &value, const GeNN::Type::ResolvedType &type,
              const std::string &name = "")
    :   AcceptableModelComponent<Parameter>(name), m_Value(value), m_Type(type)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getValue() const{ return m_Value; }
    const auto &getType() const{ return m_Type; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    GeNN::Type::NumericValue m_Value;
    GeNN::Type::ResolvedType m_Type;
};