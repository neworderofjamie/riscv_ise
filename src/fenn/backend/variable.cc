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
std::vector<Compiler::RegisterPtr> Variable::genPreamble(Assembler::CodeGenerator &c,
                                                         Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                         Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                                         uint32_t varFieldOffset, std::optional<uint32_t> numTimesteps,
                                                         Assembler::ScalarRegisterPtr fieldBaseReg,
                                                         Assembler::ScalarRegisterPtr timeReg,
                                                         Assembler::ScalarRegisterPtr numVariableBytes,
                                                         bool hasTime, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        // Allocate scalar register to hold address of variable
        auto readBufferReg = scalarRegisterAllocator.getRegister((getName() + "Buffer X").c_str());

        // Generate code to load address
        c.lw(*readBufferReg, *fieldBaseReg, varFieldOffset);

        // **TODO** currently this just handles providing entire simulation kernel worth of variable data or
        // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
        if (hasTime) {
            // Check there is a buffer entry for each timestep with one extra
            // **NOTE** variables get read from timestep and written to timestep + 1 hence extra buf
            if(getShape().front() < (numTimesteps.value() + 1)) {
                throw std::runtime_error("Variables need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
            }

            ALLOCATE_SCALAR(STmp);
            c.mul(*STmp, *timeReg, *numVariableBytes);
            c.add(*readBufferReg, *readBufferReg, *STmp);

            // If variable is const, return read register
            if (getType().isConst) {
                return {readBufferReg};
            }
            // Otherwise, allocate second write register
            else {
                auto writeBufferReg = scalarRegisterAllocator.getRegister((getName() + "BufferWrite X").c_str());
                c.add(*writeBufferReg, *readBufferReg, *numVariableBytes);
                return {readBufferReg, writeBufferReg};
            }
        }
        else {
            return {readBufferReg};
        }
    }
    case MemSpace::LLM:
    {
        assert(!hasTime);

        // Allocate vector register to hold address of variable
        auto bufferReg = vectorRegisterAllocator.getRegister((getName() + "Buffer V").c_str());

        // Generate code to load address
        ALLOCATE_SCALAR(STmp);
        c.lw(*STmp, *fieldBaseReg, varFieldOffset);
        c.vfill(*bufferReg, *STmp);

        return {bufferReg};
    }
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
void Variable::genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r,
                       const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        assert(state.size() >= 1);
        env.getCodeGenerator().vloadv(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[0]), 64 * r);
        break;
    }
    case MemSpace::LLM:
    {
        assert(state.size() == 1);
        env.getCodeGenerator().vloadl(*reg, *std::get<Assembler::VectorRegisterPtr>(state[0]), 2 * r);
        break;
    }
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
void Variable::genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r,
                        const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    if (!getType().isConst) {
        // Create array in correct memory space depending on compatibility
        switch(getMemSpace(model))
        {
        case MemSpace::URAM:
        {
            if (state.size() == 1) {
                env.getCodeGenerator().vstore(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[0]), 64 * r);
            }
            else  if (state.size() == 2) {
                env.getCodeGenerator().vstore(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[1]), 64 * r);
            }
            else {
                assert(false);
            }
            break;
        }
        case MemSpace::LLM:
        {
            assert(state.size() == 1);
            env.getCodeGenerator().vstorel(*reg, *std::get<Assembler::VectorRegisterPtr>(state[0]), 2 * r);  
            break;
        }
        default:
            assert(false);
        }
    }
}
//----------------------------------------------------------------------------
void Variable::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                            Assembler::VectorRegisterPtr numUnrollBytesReg,
                            const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        c.addi(*std::get<Assembler::ScalarRegisterPtr>(state[0]),
               *std::get<Assembler::ScalarRegisterPtr>(state[0]),
               64 * numUnrolls);

        if (state.size() == 2) {
            c.addi(*std::get<Assembler::ScalarRegisterPtr>(state[1]),
                   *std::get<Assembler::ScalarRegisterPtr>(state[1]),
                   64 * numUnrolls);
        }
        else if(state.size() != 1) {
            assert(false);
        }
        break;
    }
    case MemSpace::LLM:
    {
        assert(state.size() == 1);
        c.vadd(*std::get<Assembler::VectorRegisterPtr>(state[0]), 
               *std::get<Assembler::VectorRegisterPtr>(state[0]), 
               *numUnrollBytesReg);
        break;
    }
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
bool Variable::needsNumUnrollBytesReg(const Frontend::Model &model) const
{
    return (getMemSpace(model) == MemSpace::LLM);
}
//----------------------------------------------------------------------------
MemSpace Variable::getMemSpace(const Frontend::Model &model) const
{
    return static_cast<const Model&>(model).getStateMemSpace(std::static_pointer_cast<const Frontend::State>(shared_from_this()), 
                                                             false/*getRuntime().shouldUseDRAMForWeights()*/);
}
}
