#pragma once

// Standard C++ includes
#include <memory>

// GeNN includes
#include "type.h"

// Compiler includes
#include "model/model_component.h"

//----------------------------------------------------------------------------
// Parameter
//----------------------------------------------------------------------------
class Parameter : public AcceptableModelComponent<Parameter, State>
{
public:
    Parameter(Private, const GeNN::Type::NumericValue &value,
              const GeNN::Type::UnresolvedType &type, const std::string &name)
    :   AcceptableModelComponent<Parameter, State>(name), m_Value(value), m_Type(type.resolve({}))
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getValue() const{ return m_Value; }
    const auto &getType() const{ return m_Type; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Parameter> create(const GeNN::Type::NumericValue &value, const GeNN::Type::UnresolvedType &type,
                                             const std::string &name = "")
    {
        return std::make_shared<Parameter>(Private(), value, type, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    GeNN::Type::NumericValue m_Value;
    GeNN::Type::ResolvedType m_Type;
};