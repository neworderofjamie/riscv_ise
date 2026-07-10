#include "fenn/backend/events.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/isa.h"
#include "fenn/common/logging.h"

// FeNN assembler includes
#include "fenn/assembler/assembler_utils.h"

// FeNN backend includes
#include "fenn/backend/kernel.h"
#include "fenn/backend/model.h"
#include "fenn/backend/process.h"
#include "fenn/backend/runtime.h"

using namespace FeNN;
using namespace FeNN::Backend;

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
std::unique_ptr<Frontend::ArrayBase> EventSinkImplementation::createBitArray(const Frontend::Shape &deviceShape, Frontend::DeviceBase &device) const
{
    // Event containers are implemented as word-aligned bitfields so divide and pad last axis
    auto wordAlignedShape = deviceShape;
    wordAlignedShape.getLast() = ::Common::Utils::ceilDivide(wordAlignedShape.getLast(), 32);

    // Create BRAM array
    return static_cast<DeviceFeNN&>(device).createBRAMArray(CompilerFrontend::Type::Uint32, wordAlignedShape);
}
//----------------------------------------------------------------------------
Assembler::ScalarRegisterPtr EventSinkImplementation::genBitArrayPreamble(
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    std::optional<uint32_t> numTimesteps, bool hasTime, const Frontend::Shape &shape,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes, 
    AddFieldFn addField) const
{
    // Add field associated with this event sink and load it's address
    const auto reg = addField(c);

    // If there are multiple timesteps, multiply timestep by stride and add to register
    // **TODO** currently this just handles providing entire simulation kernel worth of event data or
    // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
    if (hasTime) {
        // Check there is a buffer entry for each timestep with one extra
        // **NOTE** variables get read from timestep and written to timestep + 1 so extra buf
        if(shape.getFirst() < (numTimesteps.value() + 1)) {
            throw std::runtime_error("Events need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
        }

        // reg = stride * (time + 1)
        ALLOCATE_SCALAR(STmp);
        c.addi(*STmp, *timeReg, 1);
        c.mul(*STmp, *STmp, *numEventBytes);
        c.add(*reg, *reg, *STmp);
    }

    return reg;
}
//----------------------------------------------------------------------------
void EventSinkImplementation::genBitArrayEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterPtr spikeMaskReg, 
                                              uint32_t r, Assembler::ScalarRegisterPtr stateReg) const
{
    env.getCodeGenerator().sw(*spikeMaskReg, *stateReg, 4 * r);
}
//----------------------------------------------------------------------------
void EventSinkImplementation::genBitArrayIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                                   Assembler::ScalarRegisterPtr stateReg) const
{
    c.addi(*stateReg, *stateReg, 4 * numUnrolls);
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceBuffer
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSourceBuffer::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                                    Frontend::DeviceBase &device) const
{
    assert(false);
    return nullptr;
}
//----------------------------------------------------------------------------
void EventSourceBuffer::generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime &runtime, const KernelImplementation &kernel, 
                                          Assembler::ScalarRegisterPtr preIndReg, Assembler::ScalarRegisterPtr spikeReturnReg, Assembler::Label jumpTable,
                                          Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const
{
    // **TODO** loop through merged processes, get their buffers etc etc
    assert(false);
    /*// Generate archetype code and populate merged fields
    Assembler::CodeGenerator archetypeCodeGenerator;
    const auto sharedRegisters = generateArchetypeCode(mergedProcess, runtime, kernel, mergedFields,
                                                       SFieldBase, timeReg, numTimesteps, archetypeCodeGenerator,
                                                       c, scalarRegisterAllocator, vectorRegisterAllocator);

    // Load fieldBase
    c.li(*SFieldBase, fieldBase);

    // Calculate and load fieldEnd
    fieldBase += (mergedProcess.getMerged().size() * mergedFields.getSize());
    c.li(*SFieldBaseEnd, fieldBase);

    // Generate loop over merged groups
    auto groupLoop = c.L();
    {
        // Insert generated code to simulate archetype
        c += archetypeCodeGenerator;

        // Advance to next group's fields
        c.addi(*SFieldBase, *SFieldBase, mergedFields.getSize());

        // Keep looping
        c.bne(*SFieldBase, *SFieldBaseEnd, groupLoop);
    }
    */
}
//----------------------------------------------------------------------------
void EventSourceBuffer::generateArchetypeEventLoop() const
{
    // **NOTE** this prototol assumes there is a timestamp at the indexed position in the merged field

    // Read timestamp from start of buffer
    // If time == timestamp
    //     while true
    //         Increment index
    //         Load index
    //         if top bit == 1 // timestamp
    //             return  
    //         else
    //             set spike register to 1st
    //             jalr to handler
    //             if 2nd top bit == 1
    //                 set spike register to 2nd
    //                 jalr to handler
    //             
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkBuffer
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSinkBuffer::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                                  Frontend::DeviceBase &device) const
{
    LOGI_FENN_BACKEND << "Creating event sink buffer '" << getName() << "' array in BRAM";

    return createBitArray(deviceShape, device);
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventSinkBuffer::genPreamble(
    const Model&, const KernelImplementation&,
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    std::optional<uint32_t> numTimesteps, bool hasTime, size_t,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes,
    AddScalarConstantFn, AddFieldFn addField) const
{
    // Generate preamble required for bitarray storage and add register to state
    return {genBitArrayPreamble(c, scalarRegisterAllocator, numTimesteps, hasTime, getShape(),
                                timeReg, numEventBytes, addField)};
}
//----------------------------------------------------------------------------
void EventSinkBuffer::genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator&,
                              Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // Store spike mask to address stored in state
    assert(state.size() == 1);
    genBitArrayEmit(env, spikeMaskReg, r, state[0]);
}
//----------------------------------------------------------------------------
void EventSinkBuffer::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                   const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    assert(state.size() == 1);
    genBitArrayIncrement(c, numUnrolls, state[0]);
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannel
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventChannel::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                               Frontend::DeviceBase &device) const
{
    if(shouldRecord()) {
        LOGI_FENN_BACKEND << "Creating event channel buffer '" << getName() << "' array in BRAM";

        return createBitArray(deviceShape, device);
    }
    else {
        return nullptr;
    }
}
//----------------------------------------------------------------------------
void EventChannel::generateEventLoop(const Frontend::Merged<Frontend::EventSource>&, const Runtime &runtime, const KernelImplementation &kernel, 
                                     Assembler::ScalarRegisterPtr preIndReg, Assembler::ScalarRegisterPtr spikeReturnReg, Assembler::Label jumpTable,
                                     Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const
{
    // Wait for all events from last timestep to be communicated
    Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, runtime.getNumDevices());

    // **TODO** these can't be allocated here - they need to be scoped around whole nightmare
    ALLOCATE_SCALAR(SSpikeBuffer);
    ALLOCATE_SCALAR(SSpikeBufferStart);
    ALLOCATE_SCALAR(SSpikeBufferEnd);

    // Load start and end of this timestep's spike buffer
    // **NOTE** because all event channels will be merged together, we ignore the merged event sources here
    c.csrr(*SSpikeBufferEnd, Common::CSR::SLAVE_EVENT_ADDRESS);
    c.li(*SSpikeBufferStart, 32 * 4096);
    c.mv(*SSpikeBuffer, *SSpikeBufferStart);

    // While (spikeBuffer != spikeBufferEnd
    auto spikeLoopEnd = Assembler::createLabel();
    auto spikeLoop = c.L();
    c.beq(*SSpikeBuffer, *SSpikeBufferEnd, spikeLoopEnd);
    {
        // Load spike from buffer
        c.lw(*preIndReg, *SSpikeBuffer);

        {
            // Extract event sink ID
            // **TODO** these were multiplied by 4 to obtain bytes in order to save an instruction
            ALLOCATE_SCALAR(SEventSinkID);
            c.srli(*SEventSinkID, *preIndReg, kernel.getNumNeuronIDBits());

            // Jump to correct population handler, storing return address in register
            c.jalr(*spikeReturnReg, *SEventSinkID, c.getAddress(jumpTable).value());
        }

        // Loop until spikes are processed
        c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);
        c.j_(spikeLoop);
    }
    c.L(spikeLoopEnd);

    // Reset router slave to start writing at beginning of spike buffer
    c.csrw(Common::CSR::SLAVE_EVENT_ADDRESS, *SSpikeBufferStart);

    // Wait for all routers to be reset so it's safe for other process groups to start SENDING new events
    Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, runtime.getNumDevices());
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventChannel::genPreamble(
    const Model &model, const KernelImplementation &kernel,
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    std::optional<uint32_t> numTimesteps, bool hasTime, size_t numDevices,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes,
    AddScalarConstantFn addScalarConstant, AddFieldFn addField) const
{
    auto sharedThis = std::dynamic_pointer_cast<const Frontend::EventSink>(shared_from_this());
    const uint32_t eventSinkID = kernel.getEventSinkIDBase(sharedThis);

    // Add scalar constant to hold start ID of event channel
    auto neuronStartIDReg = addScalarConstant(
        c,
        [&model, &sharedThis, eventSinkID, numDevices](size_t d, auto p)
        {
            const auto splitDimension = model.getStateData(sharedThis).splitDimension;

            // Sum up size of this process across all previous devices
            uint32_t startID = 0;
            for(size_t i = 0; i < d; i++) {
                const auto splitShape = p->getShape().getSplit(d, splitDimension, numDevices, 32);
                startID += static_cast<uint32_t>(splitShape.getFlattenedSize());
            }

            // Mask with event sink ID
            assert((startID % 32) == 0);
            return eventSinkID | startID;
        });
    
    // If we should record, also generate preamble required
    // for bitarray storage and add register to state
    if(shouldRecord()) {
        return {neuronStartIDReg, 
                genBitArrayPreamble(c, scalarRegisterAllocator, numTimesteps, hasTime, getShape(),
                                    timeReg, numEventBytes, addField)};
    }
    else {
        return {neuronStartIDReg};
    }
    
    
}
//----------------------------------------------------------------------------
void EventChannel::genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator&,
                           Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                           const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // If we should record, generate code to write to bitarray
    if(shouldRecord()) {
        assert(state.size() == 2);
        genBitArrayEmit(env, spikeMaskReg, r, state[1]);
    }
    else {
         assert(state.size() == 1);
    }

    // Write to router and advance state by 32
    // **YUCK** not unrolling friendly
    env.getCodeGenerator().csrw(Common::CSR::MASTER_EVENT_ID_BASE, *state[0]);
    env.getCodeGenerator().csrw(Common::CSR::MASTER_EVENT_BITFIELD, *spikeMaskReg);
    env.getCodeGenerator().addi(*state[0], *state[0], 32);

}
//----------------------------------------------------------------------------
void EventChannel::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // If we should record, generate code to increment bitarray
    if(shouldRecord()) {
        assert(state.size() == 2);
        genBitArrayIncrement(c, numUnrolls, state[1]);
    }
}
}
