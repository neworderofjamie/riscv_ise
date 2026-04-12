#include "fenn/backend/variable.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN backend includes
#include "fenn/backend/model.h"
#include "fenn/backend/runtime.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void Variable::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    // Superclass
    Frontend::Variable::updateMergeHash(hash, model);

    // Include hash of selected memory space
    ::Common::Utils::updateHash(
        static_cast<const Model&>(model).getStateMemSpace(std::static_pointer_cast<const Frontend::State>(shared_from_this()), 
                                                          true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> Variable::createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                           Frontend::DeviceBase &device) const
{
    // Pad last dimension to multiplies of 32
    // **THINK** this is not correct 
    // - for neurons we just want to pad total
    // - for weighs we want to pad rows
    const auto paddedShape = deviceShape.padLast(32);

    // Create array in correct memory space depending on compatibility
    switch(static_cast<const Model&>(model).getStateMemSpace(std::static_pointer_cast<const Frontend::State>(shared_from_this()), 
                                                             false/*getRuntime().shouldUseDRAMForWeights()*/))
    {
    case MemSpace::DRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in DRAM";
        return static_cast<DeviceFeNN&>(device).createDRAMArray(getType(), paddedShape);
    }
    case MemSpace::URAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in URAM";
        return static_cast<DeviceFeNN&>(device).createURAMArray(getType(), paddedShape);
    }
    case MemSpace::LLM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in LLM";
        return static_cast<DeviceFeNN&>(device).createLLMArray(getType(), paddedShape);
    }
    case MemSpace::URAM_LLM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in URAM and LLM";
        if (paddedShape.getNumDims() < 2) {
            throw std::runtime_error("Arrays allocated in URAM and LLM are expected to have time dimension");
        }

        // Slice off time dimension from shape
        const auto oneTimestepShape = paddedShape.slice(1);

        return static_cast<DeviceFeNN&>(device).createURAMLLMArray(getType(), oneTimestepShape, paddedShape);
    }
    case MemSpace::BRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in BRAM";
        return static_cast<DeviceFeNN&>(device).createBRAMArray(getType(), paddedShape);
    }
    default:
        throw std::runtime_error("Variable '" + getName() + "' is not compatible "
                                 "with any memory spaces available on FeNN");
    }
}
}
