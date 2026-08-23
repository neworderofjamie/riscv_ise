#include "fenn/backend/events.h"

// Standard C++ includes
#include <numeric>

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
#include "fenn/backend/utils.h"

using namespace FeNN;
using namespace FeNN::Backend;

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Frontend::State::ShapeStride EventSinkImplementation::getBitArrayShapeStride(const std::vector<size_t> &shape, std::optional<size_t> splitDimension, 
                                                                             uint32_t indexDimensions, size_t deviceIndex, size_t numDevices) const
{
    // Event containers are implemented as word-aligned bitfields so divide and pad last axis
    // **TODO** this is only true with 1D shape. With multi-dimensional shape, padding needs to be applied 
    // **TODO** correct solution may be to treat bool datatype as 1 bit in a uint32 for vectorised code
    auto wordAlignedShape = shape;
    wordAlignedShape.back() = ::Common::Utils::ceilDivide(wordAlignedShape.back(), 32);

    // If a split dimension is specified, split this dimension of word-aligned shape appropriately
    if (splitDimension.has_value()) {
        wordAlignedShape[splitDimension.value()] = Utils::getSplitDimension(wordAlignedShape, deviceIndex,
                                                                            splitDimension.value(), numDevices, 1);
    }

    // Calculate strides
    std::vector<size_t> strides = Frontend::Shape::getStride(wordAlignedShape,
                                                             CompilerFrontend::Type::Uint32.getSize());

    // Return tuple of shape and strides
    return std::make_tuple(wordAlignedShape, strides);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSinkImplementation::createBitArray(const std::vector<size_t> &shape, std::optional<size_t> splitDimension,
                                                                             uint32_t indexDimensions, size_t numDevices, Frontend::DeviceBase &device) const
{
    // Get shape and stride 
    auto [deviceShape, strides] = getBitArrayShapeStride(shape, splitDimension, indexDimensions, 
                                                         device.getDeviceIndex(), numDevices);

    // Create BRAM array
    return static_cast<DeviceFeNN&>(device).createBRAMArray(CompilerFrontend::Type::Uint32, deviceShape, strides);
}
//----------------------------------------------------------------------------
Assembler::ScalarRegisterPtr EventSinkImplementation::genBitArrayPreamble(
    Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
    std::optional<uint32_t> numTimesteps, bool hasTime, const std::vector<size_t> &shape,
    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes, 
    AddFieldFn addField) const
{
    // Add field associated with this event sink and load it's address
    const auto reg = addField(c);

    // If there are multiple timesteps, multiply timestep by stride and add to register
    // **TODO** currently this just handles providing entire simulation kernel worth of event data or
    // recording variables for entire simulation - extend to ring-buffer recording
    if (hasTime) {
        // Check there is a buffer entry for each timestep
        if(shape.front() < numTimesteps.value()) {
            throw std::runtime_error("Events need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
        }

        // reg = stride * time 
        ALLOCATE_SCALAR(STmp);
        c.mul(*STmp, *timeReg, *numEventBytes);
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
std::unique_ptr<Frontend::ArrayBase> EventSourceBuffer::createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                    size_t numDevices, const Frontend::Model &model, Frontend::DeviceBase &device) const
{
    // Get array shape and strides and create BRAM array with this shape and stride
    auto [shape, strides] = getArrayShapeStride(splitDimension, indexDimensions, device.getDeviceIndex(), numDevices, model);
    return static_cast<DeviceFeNN&>(device).createBRAMArray(CompilerFrontend::Type::Uint16, shape, strides);
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride EventSourceBuffer::getArrayShapeStride(std::optional<size_t>, uint32_t,
                                                                    size_t, size_t, const Frontend::Model&) const{
    // Check we have enough bits to encode events from all device
    // **NOTE** because event sources are never sliced this is correct
    if (std::accumulate(getShape().cbegin(), getShape().cend(), 1, std::multiplies<size_t>()) >= 32768) {
        throw std::runtime_error("EventSourceBuffer can only deliver events from less than 32768 sources per-device");
    }

    // Array shape and strides are irrespective of split - an array of maxEvents uint16s are instantiated on all devices
    return std::make_tuple(std::vector<size_t>{getMaxEvents()}, 
                           std::vector<size_t>{CompilerFrontend::Type::Uint16.getSize()});
}
//----------------------------------------------------------------------------
uint32_t EventSourceBuffer::generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime&, 
                                              const KernelImplementation&, MergedFields &mergedFields, 
                                              Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                              Assembler::ScalarRegisterPtr spikeReturnReg, std::optional<uint32_t>,
                                              const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, uint32_t> &eventSourceAddresses,
                                              uint32_t &fieldBase, Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const
{
    // Allocate base register
    uint32_t scalarRegisterMask = 0;
    ALLOCATE_SCALAR_AND_MASK(SFieldBase);

    // Generate archetype code and populate merged fields
    Assembler::CodeGenerator archetypeCodeGenerator;
    generateArchetypeEventLoop(mergedFields, SFieldBase, timeReg, preIndReg, spikeReturnReg, eventSourceAddresses, 
                               archetypeCodeGenerator, scalarRegisterAllocator, scalarRegisterMask);

    // Load fieldBase
    c.li(*SFieldBase, fieldBase);

    // Calculate fieldEnd and load if required
    fieldBase += (mergedEventSource.getMerged().size() * mergedFields.getSize());
    Assembler::ScalarRegisterPtr fieldBaseEndReg;
    if(mergedEventSource.getMerged().size() > 1) {
        fieldBaseEndReg = scalarRegisterAllocator.getRegister("SFieldBaseEnd = X");
        Assembler::ScalarRegisterAllocator::updateMask(*fieldBaseEndReg, scalarRegisterMask);
        c.li(*fieldBaseEndReg, fieldBase);
    }

    // Generate loop over merged event sources
    auto groupLoop = c.L();
    {
        // Insert generated code to simulate archetype
        c += archetypeCodeGenerator;

        // If a loop is required
        if(mergedEventSource.getMerged().size() > 1) {
            // Advance to next group's fields
            c.addi(*SFieldBase, *SFieldBase, mergedFields.getSize());

            // Keep looping
            c.bne(*SFieldBase, *fieldBaseEndReg, groupLoop);
        }
    }

    return scalarRegisterMask;
}
//----------------------------------------------------------------------------
void EventSourceBuffer::generateArchetypeEventLoop(MergedFields &mergedFields, 
                                                   Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                                   Assembler::ScalarRegisterPtr preIndReg, Assembler::ScalarRegisterPtr spikeReturnReg, 
                                                   const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, uint32_t> &eventSourceAddresses, 
                                                   Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t &scalarRegisterMask) const
{
    // Add field to hold buffer
    const uint32_t bufferFieldOffset = mergedFields.addField<EventSourceBuffer>(
        [](const Frontend::DeviceBase &d, auto e)
        { 
            return d.getArray(e); 
        });

    // Add field containing address to jump to 
    // **OPTIMISE** if all labels are the same (/there is only one) no need for a label
    const uint32_t labelFieldOffset = mergedFields.addField<EventSourceBuffer>(
        [eventSourceAddresses](size_t, auto e)
        {
            return eventSourceAddresses.at(e);
        });

    // **NOTE** we don't REALLY need SBufferStart all the time, could reload at end
    ALLOCATE_SCALAR_AND_MASK(SBufferStart);
    ALLOCATE_SCALAR_AND_MASK(SBuffer);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceHandler);

    // Load buffer start address and handler address
    c.lw(*SBufferStart, *fieldBaseReg, bufferFieldOffset);
    c.lw(*SEventSourceHandler, *fieldBaseReg, labelFieldOffset);
    
    {
        ALLOCATE_SCALAR(STmp);

        // Load offset from start of buffer
        c.lw(*STmp, *SBufferStart);

        // Add offset to buffer start
        c.add(*SBuffer, *SBufferStart, *STmp);
    }

    // Load first half-word from buffer
    // **NOTE** add 4 to skip 4 bytes holding offset
    c.lhu(*preIndReg, *SBuffer, 4);

    // Extract time from lower 31 bits of event
    // **NOTE** we assume this is a time
    {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, 0x7FFF);
        c.and_(*preIndReg, *preIndReg, *STmp);
    }

    // If word doesn't match current timestep, goto end
    auto noSpikes = Assembler::createLabel();
    c.bne(*preIndReg, *timeReg, noSpikes);

    {
        // Build time mask
        ALLOCATE_SCALAR_AND_MASK(STimeMask);
        c.li(*STimeMask, 1 << 15);
        
        // Advance buffer pointer
        c.addi(*SBuffer, *SBuffer, 2);

        auto spikeLoopStart = c.L();
        auto spikeLoopEnd = Assembler::createLabel();
        {
            // Load next word from buffer and increment pointer
            // **NOTE** add 4 to skip 4 bytes holding offset
            c.lhu(*preIndReg, *SBuffer, 4);
            c.addi(*SBuffer, *SBuffer, 2);

            // If we have hit the next timestamp, goto spikeLoopEnd
            {
                ALLOCATE_SCALAR(STmp);
                c.and_(*STmp, *preIndReg, *STimeMask);
                c.bne(*STmp, Common::Reg::X0, spikeLoopEnd);
            }

            // Jump to event source handler, storing return address in register provides
            c.jalr(*spikeReturnReg, *SEventSourceHandler);

            // Goto spike loop start
            c.j_(spikeLoopStart);
        }

        c.L(spikeLoopEnd);

        {
            ALLOCATE_SCALAR(STmp);
            
            // Subtract 2 from buffer to counteract
            // **NOTE** this is better than adding 2 AFTER branch as it hides stall EVERY spike
            c.addi(*SBuffer, *SBuffer, -2);

            // Get updated offset
            c.sub(*STmp, *SBuffer, *SBufferStart);

            // Store it back to start of buffer
            c.sw(*STmp, *SBufferStart);
        }
    }

    c.L(noSpikes);
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkBuffer
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSinkBuffer::createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                  size_t numDevices, const Frontend::Model&, Frontend::DeviceBase &device) const
{
    LOGI_FENN_BACKEND << "Creating event sink buffer '" << getName() << "' array in BRAM";

    return createBitArray(getShape(), splitDimension, indexDimensions, numDevices, device);
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride EventSinkBuffer::getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                  size_t deviceIndex, size_t numDevices, const Frontend::Model&) const
{
    return getBitArrayShapeStride(getShape(), splitDimension, indexDimensions, numDevices, deviceIndex);
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventSinkBuffer::genPreamble(
    const Runtime&, const KernelImplementation&, Assembler::CodeGenerator &c, 
    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, const std::string&,
    std::optional<uint32_t> numTimesteps, bool hasTime, Assembler::ScalarRegisterPtr timeReg, 
    Assembler::ScalarRegisterPtr numEventBytes, AddScalarConstantFn, AddFieldFn addField) const
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
// FeNN::Backend::EventChannelSource
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventChannelSource::createArray(std::optional<size_t>, uint32_t, size_t,
                                                                     const Frontend::Model&, Frontend::DeviceBase&) const
{
    return nullptr;
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride EventChannelSource::getArrayShapeStride(std::optional<size_t>, uint32_t,
                                                                     size_t, size_t, const Frontend::Model&) const
{
    // **YUCK** optional
    return std::make_tuple(std::vector<size_t>{}, std::vector<size_t>{});
}
//----------------------------------------------------------------------------
uint32_t EventChannelSource::generateEventLoop(const Frontend::Merged<Frontend::EventSource>&, const Runtime &runtime, 
                                               const KernelImplementation &kernel, MergedFields&, 
                                               Assembler::ScalarRegisterPtr, Assembler::ScalarRegisterPtr preIndReg, 
                                               Assembler::ScalarRegisterPtr spikeReturnReg, std::optional<uint32_t> jumpTableAddress, 
                                               const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, uint32_t>&,
                                               uint32_t&, Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const
{
    // Wait for all events from last timestep to be communicated
    Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, runtime.getNumDevices());

    uint32_t scalarRegisterMask = 0;
    ALLOCATE_SCALAR_AND_MASK(SSpikeBuffer);
    ALLOCATE_SCALAR_AND_MASK(SSpikeBufferEnd);

    // Load start and end of this timestep's spike buffer
    // **NOTE** because all event channels will be merged together, we ignore the merged event sources here
    c.csrr(*SSpikeBuffer, Common::CSR::SLAVE_EVENT_START_ADDRESS);
    c.csrr(*SSpikeBufferEnd, Common::CSR::SLAVE_EVENT_END_ADDRESS);

    // If neuron ID mask doesn't fit in an immediate, load it into register
    const uint32_t neuronIDMask = (1 << kernel.getNumNeuronIDBits()) - 1;
    Assembler::ScalarRegisterPtr neuronIDMaskReg;
    if(!Common::inSBit(neuronIDMask, 12)) {
        neuronIDMaskReg = scalarRegisterAllocator.getRegister("SNeuronIDMaskReg = X");
        c.li(*neuronIDMaskReg, neuronIDMask);
        Assembler::ScalarRegisterAllocator::updateMask(*neuronIDMaskReg, scalarRegisterMask);
    }

    // While (spikeBuffer != spikeBufferEnd
    auto spikeLoopEnd = Assembler::createLabel();
    auto spikeLoop = c.L();

    c.beq(*SSpikeBuffer, *SSpikeBufferEnd, spikeLoopEnd);
    {
        // Load spike from buffer and advance
        c.lw(*preIndReg, *SSpikeBuffer);
        c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

        {
            // Extract event sink ID
            // **TODO** these were multiplied by 4 to obtain bytes in order to save an instruction
            ALLOCATE_SCALAR(SEventSinkID);
            c.srli(*SEventSinkID, *preIndReg, kernel.getNumNeuronIDBits());

            // AND neuron ID with mask
            if(Common::inSBit(neuronIDMask, 12)) {
                c.andi(*preIndReg, *preIndReg, neuronIDMask);
            }
            else {
                c.and_(*preIndReg, *preIndReg, *neuronIDMaskReg);
            }

            // Jump to correct population handler, storing return address in register
            c.jalr(*spikeReturnReg, *SEventSinkID, jumpTableAddress.value());
        }

        // Loop until spikes are processed
        c.j_(spikeLoop);
    }
    c.L(spikeLoopEnd);
    return scalarRegisterMask;
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannelSink
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventChannelSink::createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                   size_t numDevices, const Frontend::Model&, Frontend::DeviceBase &device) const
{
    if(shouldRecord()) {
        LOGI_FENN_BACKEND << "Creating event channel buffer '" << getName() << "' array in BRAM";

        return createBitArray(getShape(), splitDimension, indexDimensions, numDevices, device);
    }
    else {
        // **TODO** check doesn't have time - that would be weird!
        return nullptr;
    }
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride EventChannelSink::getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                   size_t deviceIndex, size_t numDevices, const Frontend::Model &model) const
{
    if(shouldRecord()) {
        return getBitArrayShapeStride(getShape(), splitDimension, indexDimensions, numDevices, deviceIndex);
    }
    else {
        return std::make_tuple(std::vector<size_t>{}, std::vector<size_t>{});
    }
}
//----------------------------------------------------------------------------
std::vector<Assembler::ScalarRegisterPtr> EventChannelSink::genPreamble(
    const Runtime &runtime, const KernelImplementation &kernel, Assembler::CodeGenerator &c,
    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, const std::string &name,
    std::optional<uint32_t> numTimesteps, bool hasTime, Assembler::ScalarRegisterPtr timeReg, 
    Assembler::ScalarRegisterPtr numEventBytes, AddScalarConstantFn addScalarConstant, AddFieldFn addField) const
{

    // Add scalar constant to hold start ID of event channel
    auto neuronStartIDReg = addScalarConstant(
        c,
        [&kernel, &runtime, &name](size_t d, auto p)
        {
            const auto &eventSink = p->getOutputEventSinks().at(name);
            const auto state = eventSink.getUnderlying();
            const uint32_t eventSinkID = kernel.getEventSinkIDBase(state);
            const size_t numSlicedDimensions = eventSink.getShape().size();

            // Sum up size of this process across all previous devices
            uint32_t startID = 0;
            for(size_t i = 0; i < d; i++) {
                // Get shape of array storing this state on device
                const auto shape = std::get<0>(runtime.getDeviceArrayShapeStrides(state, d));

                // Multiply together last dimensions of shape (determined based on sliced shape)
                startID += static_cast<uint32_t>(std::accumulate(shape.rbegin(), shape.rbegin() + numSlicedDimensions, 
                                                                 1, std::multiplies<size_t>()));
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
void EventChannelSink::genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator&,
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
void EventChannelSink::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                                    const std::vector<Assembler::ScalarRegisterPtr> &state) const
{
    // If we should record, generate code to increment bitarray
    if(shouldRecord()) {
        assert(state.size() == 2);
        genBitArrayIncrement(c, numUnrolls, state[1]);
    }
}
}
