#pragma once

// Standard C++ includes
#include <memory>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/variable.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/model.h"
#include "fenn/backend/backend_export.h"

// Forward declarations
namespace FeNN
{
namespace Assembler
{
class CodeGenerator;
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT Variable : public Frontend::Variable
{
public:
    using Frontend::Variable::Variable;

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                             size_t numDevices, const Frontend::Model &model, Frontend::DeviceBase &device) const override final;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                            size_t deviceIndex, size_t numDevices, const Frontend::Model &model) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const std::vector<size_t> &shape, const CompilerFrontend::Type::UnresolvedType &type, 
                                            const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, name);
    }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    MemSpace getMemSpace(const Frontend::Model &model) const;
};
}