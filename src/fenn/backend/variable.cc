#include "fenn/backend/variable.h"

// Standard C++ includes
#include <algorithm>
#include <numeric>

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/model.h"
#include "fenn/backend/runtime.h"
#include "fenn/backend/utils.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
std::unique_ptr<Frontend::ArrayBase> Variable::createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions, 
                                                           size_t numDevices, const Frontend::Model &model, Frontend::DeviceBase &device) const
{
    // Get array shape and stride 
    auto [shape, strides] = getArrayShapeStride(splitDimension, indexDimensions, 
                                                device.getDeviceIndex(), numDevices, model);
    
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::DRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in DRAM";
        return static_cast<DeviceFeNN&>(device).createDRAMArray(getType(), shape, strides);
    }
    case MemSpace::URAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in URAM";
        return static_cast<DeviceFeNN&>(device).createURAMArray(getType(), shape, strides);
    }
    case MemSpace::LLM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in LLM";
        return static_cast<DeviceFeNN&>(device).createLLMArray(getType(), shape, strides);
    }
    case MemSpace::BRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in BRAM";
        return static_cast<DeviceFeNN&>(device).createBRAMArray(getType(), shape, strides);
    }
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride Variable::getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions, 
                                                           size_t deviceIndex, size_t numDevices, const Frontend::Model &model) const
{
    // Get memory space this variable is destined for
    const auto memorySpace = getMemSpace(model);

    // Copy shape
    std::vector<size_t> shape = getShape();

    // If a split dimension is specified, split this dimension appropriately
    if (splitDimension.has_value()) {
        const size_t absSplitDim = shape.size() - 1 - splitDimension.value();
        shape[absSplitDim] = Utils::getSplitDimension(shape, deviceIndex, absSplitDim,
                                                      numDevices, (memorySpace == MemSpace::BRAM) ? 1 : 32);
    }
    
    // Calculate strides
    std::vector<size_t> strides = Frontend::Shape::getStride(shape, getType().getSize());

    // If this variable is destined for a memory space with alignment constraints
    if (memorySpace != MemSpace::BRAM) {
        // Loop through stride dimensions
        const uint32_t topDim = 1 << (shape.size() - 1);
        for(size_t i = 0; i < strides.size(); i++) {
            // If this dimension is indexable, pad to 64 bytes
            if (indexDimensions & (topDim >> i)) {
                strides[i] = ::Common::Utils::padSize(strides[i], 64);
            }
        }
    }

    // Return tuple of shape and strides
    return std::make_tuple(shape, strides);
}
//----------------------------------------------------------------------------
MemSpace Variable::getMemSpace(const Frontend::Model &model) const
{
    return static_cast<const Model&>(model).getStateMemSpace(
        std::static_pointer_cast<const Frontend::State>(shared_from_this()));
}
}
