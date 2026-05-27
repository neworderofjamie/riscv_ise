#include "fenn/backend/events.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/isa.h"
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
    std::optional<uint32_t> numTimesteps, bool hasTime,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes,
    AddScalarConstantFn addScalarConstant, AddFieldFn addField) const
{
    // Add field and load it's address
    const auto reg = addField(c);

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
void EventSinkBuffer::genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                              Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // Store spike mask to address stored in state
    assert(state.size() == 1);
    env.getCodeGenerator().sw(*spikeMaskReg, *state[0], 4 * r);
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
    std::optional<uint32_t> numTimesteps, bool hasTime,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes,
    AddScalarConstantFn addScalarConstant, AddFieldFn addField) const
{
    const uint32_t eventSinkID = model->getEventSinkIDBase(this);

    // Add scalar constant to hold start ID of event channel
    auto neuronStartIDReg = addScalarConstant(
        c,
        [&e, &runtime, eventSinkID](size_t d, auto p)
        {
            const auto splitDimension = runtime.getModel<Model>()->getStateData(e.second.getUnderlying()).splitDimension;

            // Sum up size of this process across all previous devices
            uint32_t startID = 0;
            for(size_t i = 0; i < d; i++) {
                const auto splitShape = p->getShape().split(d, splitDimension, runtime.getNumDevices(), 32);
                startID += static_cast<uint32_t>(splitShape.getFlattenedSize());
            }

            // Mask with event sink ID
            assert((startID % 32) == 0);
            return eventSinkID | startID;
        });


    return {neuronStartIDReg};
}
//----------------------------------------------------------------------------
void EventChannel::genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                           Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t, 
                           const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    assert(state.size() == 1);

    // Write to router and advance state by 32
    // **YUCK** not unrolling friendly
    env.getCodeGenerator().csrw(Common::CSR::MASTER_EVENT_ID_BASE, *state[0]);
    env.getCodeGenerator().csrw(Common::CSR::MASTER_EVENT_BITFIELD, *spikeMaskReg);
    env.getCodeGenerator().addi(*state[0], *state[0], 32);

    // **TODO** record
}
//----------------------------------------------------------------------------
void EventChannel::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // **TODO** record
}
}
