#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler frontend includes
#include "compiler_frontend/type.h"

// Model includes
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"
#include "frontend/shape.h"

//----------------------------------------------------------------------------
// Frontend::Variable
//----------------------------------------------------------------------------
namespace Frontend
{
class FRONTEND_EXPORT Variable : public State
{
public:
    Variable(Private, const Shape &shape, const CompilerFrontend::Type::UnresolvedType &type, const std::string &name)
    :   State(name), m_Shape(shape), m_Type(type.resolve())
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const Shape &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    virtual std::unique_ptr<ArrayBase> createArray(const Shape &deviceShape, const Model &model, 
                                                   DeviceBase &device) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getType() const{ return m_Type; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const Shape &shape, const CompilerFrontend::Type::UnresolvedType &type, 
                                            const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
    CompilerFrontend::Type::ResolvedType m_Type;
};
}