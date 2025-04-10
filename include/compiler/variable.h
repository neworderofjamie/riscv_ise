#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// GeNN includes
#include "type.h"

// Compiler includes
#include "model_component.h"
#include "shape.h"

//----------------------------------------------------------------------------
// Variable
//----------------------------------------------------------------------------
class Variable : public AcceptableModelComponent<Variable, State>
{
public:
    Variable(Private, const Shape &shape, const GeNN::Type::UnresolvedType &type, const std::string &name)
    :   AcceptableModelComponent<Variable, State>(name), m_Shape(shape), m_Type(type.resolve({}))
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }
    const auto &getType() const{ return m_Type; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const Shape &shape, const GeNN::Type::UnresolvedType &type, const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
    GeNN::Type::ResolvedType m_Type;
};