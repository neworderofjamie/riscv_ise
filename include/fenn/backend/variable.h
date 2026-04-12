#pragma once

// Standard C++ includes
#include <memory>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/variable.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class Variable : public Frontend::Variable
{
public:
    using Frontend::Variable::Variable;

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;

    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const Frontend::Shape &shape, const CompilerFrontend::Type::UnresolvedType &type, 
                                            const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, name);
    }
};
}