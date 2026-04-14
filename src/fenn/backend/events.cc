#include "fenn/backend/events.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/logging.h"

// FeNN backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/model.h"
#include "fenn/backend/runtime.h"

using namespace FeNN;
using namespace FeNN::Backend;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
std::unique_ptr<Frontend::ArrayBase> createEventBitArray(const Frontend::Shape &deviceShape, Frontend::DeviceBase &device)
{
    // Event containers are implemented as word-aligned bitfields so divide and pad last axis
    auto wordAlignedShape = deviceShape;
    wordAlignedShape.getLast() = ::Common::Utils::ceilDivide(wordAlignedShape.getLast(), 32);

    // Create BRAM array
    return static_cast<DeviceFeNN&>(device).createBRAMArray(CompilerFrontend::Type::Uint32, wordAlignedShape);
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceBuffer
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
std::unique_ptr<Frontend::ArrayBase> EventSourceBuffer::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                                    Frontend::DeviceBase &device) const
{
    assert(false);
    return nullptr;
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkBuffer
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSinkBuffer::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                                  Frontend::DeviceBase &device) const
{
    LOGI_FENN_BACKEND << "Creating event sink buffer '" << getName() << "' array in BRAM";

    return createEventBitArray(deviceShape, device);
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventSinkBuffer::genPreamble(
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    uint32_t eventFieldOffset, std::optional<uint32_t> numTimesteps,
    Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
    Assembler::ScalarRegisterPtr numEventBytes, bool hasTime) const
{
    // Allocate scalar register to hold address of variable
    const auto reg = scalarRegisterAllocator.getRegister((getName() + "Buffer X").c_str());

    // Generate code to load address
    c.lw(*reg, *fieldBaseReg, eventFieldOffset);

    // If there are multiple timesteps, multiply timestep by stride and add to register
    // **TODO** currently this just handles providing entire simulation kernel worth of event data or
    // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
    if (hasTime) {
        // Check there is a buffer entry for each timestep with one extra
        // **NOTE** variables get read from timestep and written to timestep + 1 so extra buf
        if(getShape().getFirst() < (numTimesteps.value() + 1)) {
            throw std::runtime_error("Events need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
        }

        // reg = stride * (time + 1)
        ALLOCATE_SCALAR(STmp);
        c.addi(*STmp, *timeReg, 1);
        c.mul(*STmp, *STmp, *numEventBytes);
        c.add(*reg, *reg, *STmp);
    }

    return {reg};
}
//----------------------------------------------------------------------------
std::pair<Compiler::RegisterPtr, bool> EventSinkBuffer::genEmit(
    Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    Assembler::ScalarRegisterPtr spikeMaskReg, Assembler::ScalarRegisterPtr maskReg,
    uint32_t r, const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // If this loop iteration has a mask, AND it with spike mask and store word
    assert(state.size() == 1);
    if(maskReg) {
        ALLOCATE_SCALAR(STmp);
        env.getCodeGenerator().and_(*STmp, *spikeMaskReg, *maskReg);
        env.getCodeGenerator().sw(*STmp, *state[0], 4 * r);
    }
    // Otherwise, just store spike mask register
    else {
        env.getCodeGenerator().sw(*spikeMaskReg, *state[0], 4 * r);
    }

    return std::make_pair(Compiler::RegisterPtr{}, false);
}
//----------------------------------------------------------------------------
void EventSinkBuffer::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                   const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    assert(state.size() == 1);
    c.addi(*state[0], *state[0], 4 * numUnrolls);
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannel
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventChannel::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                               Frontend::DeviceBase &device) const
{
    // **TODO** recording
    return nullptr;
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventChannel::genPreamble(
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    uint32_t eventFieldOffset, std::optional<uint32_t> numTimesteps,
    Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
    Assembler::ScalarRegisterPtr numEventBytes, bool hasTime) const
{
    return {};
}
//----------------------------------------------------------------------------
std::pair<Compiler::RegisterPtr, bool> EventChannel::genEmit(
    Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    Assembler::ScalarRegisterPtr spikeMaskReg, Assembler::ScalarRegisterPtr maskReg,
    uint32_t r, const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    assert(false);
    return std::make_pair(Compiler::RegisterPtr{}, false);
}
//----------------------------------------------------------------------------
void EventChannel::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    assert(false);
}
}
