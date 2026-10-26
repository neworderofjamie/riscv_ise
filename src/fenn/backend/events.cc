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
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
size_t getNumNeuronIDBits(const std::vector<size_t> &shape, bool record)
{
    // Starting at either first of second dimension, depending on whether recording is enabled,
    // Sum up the number of bits required to represent 
    auto shapeBegin = record ? (shape.cbegin() + 1) : shape.cbegin();
    return std::accumulate(shapeBegin, shape.cend(), size_t{0}, 
                           [](size_t acc, size_t v){ return acc + ::Common::Utils::getNumBits(v); });
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Frontend::State::ShapeStride EventSinkImplementation::getBitArrayShapeStride(const std::vector<size_t> &shape, std::optional<size_t> splitDimension, 
                                                                             uint32_t, size_t deviceIndex, size_t numDevices) const
{
    // **TODO** current approach only works with 1D shape. With multi-dimensional shape, padding needs to be applied 
    assert(shape.size() == 2);

    // Copy shape
    auto wordAlignedShape = shape;

    // If a split dimension is specified, split this dimension of word-aligned shape appropriately
    // **NOTE** we operate on bits here with a 32-bit granularity
    if (splitDimension.has_value()) {
        const size_t absSplitDim = wordAlignedShape.size() - 1 - splitDimension.value();
        wordAlignedShape[absSplitDim] = Utils::getSplitDimension(wordAlignedShape, deviceIndex,
                                                                 absSplitDim, numDevices, 32);
    }

    // Word align final dimension
    wordAlignedShape.back() = ::Common::Utils::ceilDivide(wordAlignedShape.back(), 32);

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
EventSourceBuffer::EventSourceBuffer(Private, const std::vector<size_t> &shape, size_t maxEvents, const std::string &name)
:   Frontend::EventSourceBuffer(Private(), shape, maxEvents, name),
    m_LongEvents(std::accumulate(shape.cbegin(), shape.cend(), 1, std::multiplies<size_t>()) >= 32768)
{
    LOGI_FENN_BACKEND << "Event source buffer '" << getName() << "' uses " << (m_LongEvents ? "long" : "short") << " events";
}
//----------------------------------------------------------------------------
void EventSourceBuffer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    // Superclass
    Frontend::EventSourceBuffer::updateMergeHash(hash);

    // Event source buffers which use long format event cannot be merged with this which use short format
    ::Common::Utils::updateHash(m_LongEvents, hash);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventSourceBuffer::createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                                    size_t numDevices, const Frontend::Model &model, Frontend::DeviceBase &device) const
{
    using namespace CompilerFrontend::Type;

    // Get array shape and strides and create BRAM array with this shape and stride
    auto [shape, strides] = getArrayShapeStride(splitDimension, indexDimensions, device.getDeviceIndex(), numDevices, model);
    return static_cast<DeviceFeNN&>(device).createBRAMArray(m_LongEvents ? Uint32 : Uint16, shape, strides);
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride EventSourceBuffer::getArrayShapeStride(std::optional<size_t>, uint32_t,
                                                                    size_t, size_t, const Frontend::Model&) const
{
    using namespace CompilerFrontend::Type;

    // Array shape and strides are irrespective of split - an array of maxEvents uint32 or uint16 are instantiated on all devices
    return std::make_tuple(std::vector<size_t>{getMaxEvents()}, 
                           std::vector<size_t>{(m_LongEvents ? Uint32.getSize() : Uint16.getSize())});
}
//----------------------------------------------------------------------------
uint32_t EventSourceBuffer::generateURAMEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime&, 
                                                  const KernelImplementation &kernel, MergedFields &mergedFields, Assembler::ScalarRegisterPtr timeReg, 
                                                  uint32_t eventSinkSourceTableAddress, uint32_t &fieldBase, 
                                                  Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                  ProcessEventFn processEvent) const
{
    // Call helper to generate generic merged structure with URAM event loop inside
    return generateEventLoop(mergedEventSource, mergedFields, fieldBase, c, 
                             scalarRegisterAllocator,
                             [this, eventSinkSourceTableAddress, processEvent, timeReg,
                              &kernel, &mergedFields, &scalarRegisterAllocator]
                             (Assembler::CodeGenerator &archetypeCodeGenerator, Assembler::ScalarRegisterPtr fieldBaseReg, 
                                                   uint32_t &scalarRegisterMask)
                             {
                                generateArchetypeURAMEventLoop(kernel, mergedFields, fieldBaseReg, timeReg,
                                                               eventSinkSourceTableAddress, archetypeCodeGenerator, 
                                                               scalarRegisterAllocator, scalarRegisterMask, processEvent);

                             });
}
//----------------------------------------------------------------------------
uint32_t EventSourceBuffer::generateDRAMEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime&, 
                                                  const KernelImplementation &kernel, MergedFields &mergedFields, Assembler::ScalarRegisterPtr timeReg, 
                                                  Assembler::ScalarRegisterPtr rowBufferAReg, Assembler::ScalarRegisterPtr rowBufferBReg, 
                                                  uint32_t eventSinkSourceTableAddress, uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                                                  Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                  DMARowFn dmaRow, DMAWaitFn dmaWait, ProcessDMAEventFn processEvent) const
{
    // Call helper to generate generic merged structure with URAM event loop inside
    return generateEventLoop(mergedEventSource, mergedFields, fieldBase, c, 
                             scalarRegisterAllocator,
                             [this, dmaRow, dmaWait, eventSinkSourceTableAddress, processEvent, rowBufferAReg, 
                              rowBufferBReg, timeReg, &kernel, &mergedFields, &scalarRegisterAllocator]
                             (Assembler::CodeGenerator &archetypeCodeGenerator, Assembler::ScalarRegisterPtr fieldBaseReg, 
                                                   uint32_t &scalarRegisterMask)
                             {
                                generateArchetypeDRAMEventLoop(kernel, mergedFields, fieldBaseReg, timeReg,
                                                               rowBufferAReg, rowBufferBReg, eventSinkSourceTableAddress, 
                                                               archetypeCodeGenerator, scalarRegisterAllocator, 
                                                               scalarRegisterMask, dmaRow, dmaWait, processEvent);

                             });
}
//----------------------------------------------------------------------------
uint32_t EventSourceBuffer::generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, MergedFields &mergedFields, 
                                              uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                              GenerateArchetypeFn generateArchetype) const
{
    // Allocate base register
    uint32_t scalarRegisterMask = 0;
    ALLOCATE_SCALAR_AND_MASK(SFieldBase);

    // Generate archetype code and populate merged fields
    Assembler::CodeGenerator archetypeCodeGenerator;
    generateArchetype(archetypeCodeGenerator, SFieldBase, scalarRegisterMask);

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
void EventSourceBuffer::generateArchetypeURAMEventLoop(const KernelImplementation &kernel, MergedFields &mergedFields, 
                                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg, 
                                                       uint32_t, Assembler::CodeGenerator &c,
                                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                       uint32_t &scalarRegisterMask, ProcessEventFn processEvent) const
{
    // Add field to hold buffer
    const uint32_t bufferFieldOffset = mergedFields.addField<EventSourceBuffer>(
        [](const Frontend::DeviceBase &d, auto e)
        { 
            return d.getArray(e); 
        });

    // Add field containing start offsets (in bytes) into event source process table
    const uint32_t eventSourceProcessStartOffset = mergedFields.addField<EventSourceBuffer>(
        [&kernel](size_t, auto e) -> uint32_t
        {
            return (16 * std::get<0>(kernel.getEventSourceProcessTableOffsets().at(e)));
        });

    // Add field containing end offsets (in bytes) into event source process table
    const uint32_t eventSourceProcessEndOffset = mergedFields.addField<EventSourceBuffer>(
        [&kernel](size_t, auto e) -> uint32_t
        {
            return (16 * std::get<1>(kernel.getEventSourceProcessTableOffsets().at(e)));
        });

    // **NOTE** we don't REALLY need SBufferStart all the time, could reload at end
    ALLOCATE_SCALAR_AND_MASK(SBufferStart);
    ALLOCATE_SCALAR_AND_MASK(SBuffer);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessStartOffset);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessEndOffset);
    ALLOCATE_SCALAR_AND_MASK(SPreIndReg);

    // Load buffer start address and handler address
    c.lw(*SBufferStart, *fieldBaseReg, bufferFieldOffset);
    c.lw(*SEventSourceProcessStartOffset, *fieldBaseReg, eventSourceProcessStartOffset);
    c.lw(*SEventSourceProcessEndOffset, *fieldBaseReg, eventSourceProcessEndOffset);
    
    {
        ALLOCATE_SCALAR(STmp);

        // Load offset from start of buffer
        c.lw(*STmp, *SBufferStart);

        // Add offset to buffer start
        c.add(*SBuffer, *SBufferStart, *STmp);
    }

    // If events should be 32 bit
    // **TODO** refactor so structure is common
    if (m_LongEvents) {
        // Load first word from buffer
        // **NOTE** add 4 to skip 4 bytes holding offset
        c.lw(*SPreIndReg, *SBuffer, 4);

        // Extract time from lower 31 bits of event
        // **NOTE** we assume this is a time
        {
            ALLOCATE_SCALAR(STmp);
            c.li(*STmp, 0x7FFFFFFF);
            c.and_(*SPreIndReg, *SPreIndReg, *STmp);
        }

        // If word doesn't match current timestep, goto end
        auto noSpikes = Assembler::createLabel();
        c.bne(*SPreIndReg, *timeReg, noSpikes);

        {
            // Build time mask
            ALLOCATE_SCALAR_AND_MASK(STimeMask);
            c.li(*STimeMask, 1 << 31);

            // Advance buffer pointer
            c.addi(*SBuffer, *SBuffer, 4);

            auto spikeLoopStart = c.L();
            auto spikeLoopEnd = Assembler::createLabel();
            {
                // Load next word from buffer and increment pointer
                // **NOTE** add 4 to skip 4 bytes holding offset
                c.lw(*SPreIndReg, *SBuffer, 4);
                c.addi(*SBuffer, *SBuffer, 4);

                // If we have hit the next timestamp, goto spikeLoopEnd
                {
                    ALLOCATE_SCALAR(STmp);
                    c.and_(*STmp, *SPreIndReg, *STimeMask);
                    c.bne(*STmp, Common::Reg::X0, spikeLoopEnd);
                }
                
                // Loop over event propagation processes
                {
                    auto processLoop = c.L();

                    // Process event
                    processEvent(c, SEventSourceProcessStartOffset, SPreIndReg);

                    // Advance pointer
                    c.addi(*SEventSourceProcessStartOffset, *SEventSourceProcessStartOffset, 16);

                    // Loop if there are more processes
                    c.bne(*SEventSourceProcessStartOffset, *SEventSourceProcessEndOffset, processLoop);
                }

                // Reset loop start
                // **YUCK** doing this first would cause a stall as processEvents starts with a load from here
                c.lw(*SEventSourceProcessStartOffset, *fieldBaseReg, eventSourceProcessStartOffset);

                // Goto spike loop start
                c.j_(spikeLoopStart);
            }

            c.L(spikeLoopEnd);

            {
                ALLOCATE_SCALAR(STmp);

                // Subtract 4 from buffer to counteract
                // **NOTE** this is better than adding 4 AFTER branch as it hides stall EVERY spike
                c.addi(*SBuffer, *SBuffer, -4);

                // Get updated offset
                c.sub(*STmp, *SBuffer, *SBufferStart);

                // Store it back to start of buffer
                c.sw(*STmp, *SBufferStart);
            }
        }

        c.L(noSpikes);
    }
    // Otherwise, if 16 bit is sufficient
    else {
        // Load first half-word from buffer
        // **NOTE** add 4 to skip 4 bytes holding offset
        c.lhu(*SPreIndReg, *SBuffer, 4);

        // Extract time from lower 15 bits of event
        // **NOTE** we assume this is a time
        {
            ALLOCATE_SCALAR(STmp);
            c.li(*STmp, 0x7FFF);
            c.and_(*SPreIndReg, *SPreIndReg, *STmp);
        }

        // If word doesn't match current timestep, goto end
        auto noSpikes = Assembler::createLabel();
        c.bne(*SPreIndReg, *timeReg, noSpikes);

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
                c.lhu(*SPreIndReg, *SBuffer, 4);
                c.addi(*SBuffer, *SBuffer, 2);

                // If we have hit the next timestamp, goto spikeLoopEnd
                {
                    ALLOCATE_SCALAR(STmp);
                    c.and_(*STmp, *SPreIndReg, *STimeMask);
                    c.bne(*STmp, Common::Reg::X0, spikeLoopEnd);
                }

                // Loop over event propagation processes
                {
                    auto processLoop = c.L();

                    // Process event
                    processEvent(c, SEventSourceProcessStartOffset, SPreIndReg);

                    // Advance pointer
                    c.addi(*SEventSourceProcessStartOffset, *SEventSourceProcessStartOffset, 16);

                    // Loop if there are more processes
                    c.bne(*SEventSourceProcessStartOffset, *SEventSourceProcessEndOffset, processLoop);
                }

                // Reset loop start
                c.lw(*SEventSourceProcessStartOffset, *fieldBaseReg, eventSourceProcessStartOffset);

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
}
//----------------------------------------------------------------------------
void EventSourceBuffer::generateArchetypeDRAMEventLoop(const KernelImplementation &kernel, MergedFields &mergedFields, 
                                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                                       Assembler::ScalarRegisterPtr rowBufferAReg, Assembler::ScalarRegisterPtr rowBufferBReg,  
                                                       uint32_t, Assembler::CodeGenerator &c, 
                                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                       uint32_t &scalarRegisterMask, DMARowFn dmaRow, 
                                                       DMAWaitFn dmaWait, ProcessDMAEventFn processEvent) const
{
    assert(!m_LongEvents);

    // Add field to hold buffer
    const uint32_t bufferFieldOffset = mergedFields.addField<EventSourceBuffer>(
        [](const Frontend::DeviceBase &d, auto e)
        { 
            return d.getArray(e); 
        });

    // Add field containing start offsets (in bytes) into event source process table
    const uint32_t eventSourceProcessStartOffset = mergedFields.addField<EventSourceBuffer>(
        [&kernel](size_t, auto e) -> uint32_t
        {
            return (16 * std::get<0>(kernel.getEventSourceProcessTableOffsets().at(e)));
        });

    // Add field containing end offsets (in bytes) into event source process table
    const uint32_t eventSourceProcessEndOffset = mergedFields.addField<EventSourceBuffer>(
        [&kernel](size_t, auto e) -> uint32_t
        {
            return (16 * std::get<1>(kernel.getEventSourceProcessTableOffsets().at(e)));
        });

    // **NOTE** we don't REALLY need SBufferStart all the time, could reload at end
    ALLOCATE_SCALAR_AND_MASK(SBufferStart);
    ALLOCATE_SCALAR_AND_MASK(SBuffer);
    ALLOCATE_SCALAR_AND_MASK(SPrevEventSourceProcessOffset);
    ALLOCATE_SCALAR_AND_MASK(SNextEventSourceProcessOffset);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessEndOffset);
    ALLOCATE_SCALAR_AND_MASK(SPreIndReg);

    // Load buffer start address and handler address
    c.lw(*SBufferStart, *fieldBaseReg, bufferFieldOffset);
    c.lw(*SPrevEventSourceProcessOffset, *fieldBaseReg, eventSourceProcessStartOffset);
    c.lw(*SEventSourceProcessEndOffset, *fieldBaseReg, eventSourceProcessEndOffset);
    
    {
        ALLOCATE_SCALAR(STmp);

        // Load offset from start of buffer
        c.lw(*STmp, *SBufferStart);

        // Add offset to buffer start
        c.add(*SBuffer, *SBufferStart, *STmp);
    }

    // Load first half-word from buffer
    // **NOTE** add 4 to skip 4 bytes holding offset
    c.lhu(*SPreIndReg, *SBuffer, 4);

    // Extract time from lower 15 bits of event
    // **NOTE** we assume this is a time
    {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, 0x7FFF);
        c.and_(*SPreIndReg, *SPreIndReg, *STmp);
    }

    // If word doesn't match current timestep, goto end
    auto noSpikes = Assembler::createLabel();
    auto lastProcess = Assembler::createLabel();
    auto nextProcess = Assembler::createLabel();
    c.bne(*SPreIndReg, *timeReg, noSpikes);

    // Build time mask
    ALLOCATE_SCALAR_AND_MASK(STimeMask);
    c.li(*STimeMask, 1 << 15);

    // Advance buffer pointer
    c.addi(*SBuffer, *SBuffer, 2);

    
    // Prefetch
    {
        ALLOCATE_SCALAR(SEventSinkID);

        // Load next word from buffer and increment pointer
        // **NOTE** add 4 to skip 4 bytes holding offset
        c.lhu(*SPreIndReg, *SBuffer, 4);
        c.addi(*SBuffer, *SBuffer, 2);

        // Start DMA to prefetch first row into buffer A
        dmaRow(c, SPrevEventSourceProcessOffset, SPreIndReg, rowBufferAReg);

        // Get offset of next process to DMA
        c.addi(*SNextEventSourceProcessOffset, *SPrevEventSourceProcessOffset, 16);
    }

    auto spikeLoop = c.L(); 
    {
        // If there are more processes associated with event source, jump to continue handling
        c.bne(*SNextEventSourceProcessOffset, *SEventSourceProcessEndOffset, nextProcess);

        // Load next word from buffer and increment pointer
        // **NOTE** add 4 to skip 4 bytes holding offset
        c.lhu(*SPreIndReg, *SBuffer, 4);
        c.addi(*SBuffer, *SBuffer, 2);
        
        // If we have hit the next timestamp, handle final process
        {
            ALLOCATE_SCALAR(STmp);
            c.and_(*STmp, *SPreIndReg, *STimeMask);
            c.bne(*STmp, Common::Reg::X0, lastProcess);
        }

        // Reset loop start
        c.lw(*SNextEventSourceProcessOffset, *fieldBaseReg, eventSourceProcessStartOffset);

        c.L(nextProcess);

        // Wait for previous DMA to complete
        dmaWait(c);

        // Start DMA to fetch next row into buffer B while this one is being processed
        dmaRow(c, SNextEventSourceProcessOffset, SPreIndReg, rowBufferBReg);

        // Process event
        processEvent(c, SPrevEventSourceProcessOffset /*rowBufferAReg*/);
        
        // Update previous event source process start offset
        c.mv(*SPrevEventSourceProcessOffset, *SNextEventSourceProcessOffset);

        // Go onto next process
        c.addi(*SNextEventSourceProcessOffset, *SNextEventSourceProcessOffset, 16);

        // Swap buffers
        {
            ALLOCATE_SCALAR(STmp);
            c.mv(*STmp, *rowBufferAReg);
            c.mv(*rowBufferAReg, *rowBufferBReg);
            c.mv(*rowBufferBReg, *STmp);
        }
        c.j_(spikeLoop);
    }

    {
        ALLOCATE_SCALAR(STmp);

        c.L(lastProcess);

        // Wait for previous DMA to complete
        dmaWait(c);

        // Process event
        processEvent(c, SPrevEventSourceProcessOffset/*, rowBufferAReg*/);
    
        // Subtract 2 from buffer to counteract
        // **NOTE** this is better than adding 2 AFTER branch as it hides stall EVERY spike
        c.addi(*SBuffer, *SBuffer, -2);

        // Get updated offset
        c.sub(*STmp, *SBuffer, *SBufferStart);

        // Store it back to start of buffer
        c.sw(*STmp, *SBufferStart);
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
    return getBitArrayShapeStride(getShape(), splitDimension, indexDimensions, deviceIndex, numDevices);
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
                                                                   size_t deviceIndex, size_t numDevices, const Frontend::Model&) const
{
    if(shouldRecord()) {
        return getBitArrayShapeStride(getShape(), splitDimension, indexDimensions, deviceIndex, numDevices);
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

            const auto &stateData = runtime.getModel()->getStateData(state);

            // Sum up size of this process across all previous devices
            uint32_t startID = 0;
            for(size_t i = 0; i < d; i++) {
                // Copy shape
                std::vector<size_t> shape = state->getShape();

                // If a split dimension is specified, split this dimension appropriately
                if (stateData.splitDimension) {
                    const size_t absSplitDim = shape.size() - 1 - stateData.splitDimension.value();
                    shape[absSplitDim] = Utils::getSplitDimension(shape, i, absSplitDim,
                                                                  runtime.getNumDevices(), 32);
                }

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
//----------------------------------------------------------------------------
size_t EventChannelSink::getNumNeuronIDBits() const
{
    return ::getNumNeuronIDBits(getShape(), shouldRecord());
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
uint32_t EventChannelSource::generateURAMEventLoop(const Frontend::Merged<Frontend::EventSource>&, const Runtime &runtime, 
                                                   const KernelImplementation &kernel, MergedFields&, Assembler::ScalarRegisterPtr, 
                                                   uint32_t eventSinkSourceTableAddress, uint32_t&, Assembler::CodeGenerator &c, 
                                                   Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                   ProcessEventFn processEvent) const
{
    // Wait for all events from last timestep to be communicated
    Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, runtime.getNumDevices());

    uint32_t scalarRegisterMask = 0;
    ALLOCATE_SCALAR_AND_MASK(SPreIndReg);
    ALLOCATE_SCALAR_AND_MASK(SSpikeBuffer);
    ALLOCATE_SCALAR_AND_MASK(SSpikeBufferEnd);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessStartOffset);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessEndOffset);

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
        {
            // Load spike from buffer and advance
            ALLOCATE_SCALAR(SEventSinkID);
            
            c.lw(*SPreIndReg, *SSpikeBuffer);
            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

            // Extract event sink ID
            // **TODO** these were multiplied by 2 to obtain bytes in order to save an instruction
            c.srli(*SEventSinkID, *SPreIndReg, kernel.getNumNeuronIDBits());

            // AND neuron ID with mask
            if(Common::inSBit(neuronIDMask, 12)) {
                c.andi(*SPreIndReg, *SPreIndReg, neuronIDMask);
            }
            else {
                c.and_(*SPreIndReg, *SPreIndReg, *neuronIDMaskReg);
            }

            // Load start and end offset of block of event propagation processes which should be passed this spike
            c.lhu(*SEventSourceProcessStartOffset, *SEventSinkID, eventSinkSourceTableAddress);
            c.lhu(*SEventSourceProcessEndOffset, *SEventSinkID, eventSinkSourceTableAddress + 2);
        }

        // Loop over event propagation processes
        {
            auto processLoop = c.L();

            // Process event
            processEvent(c, SEventSourceProcessStartOffset, SPreIndReg);

            // Advance pointer
            c.addi(*SEventSourceProcessStartOffset, *SEventSourceProcessStartOffset, 16);

            // Loop if there are more events
            c.bne(*SEventSourceProcessStartOffset, *SEventSourceProcessEndOffset, processLoop);
        }

        // Loop until spikes are processed
        c.j_(spikeLoop);
    }
    c.L(spikeLoopEnd);
    return scalarRegisterMask;
}
//----------------------------------------------------------------------------
uint32_t EventChannelSource::generateDRAMEventLoop(const Frontend::Merged<Frontend::EventSource>&, const Runtime &runtime, 
                                                   const KernelImplementation &kernel, MergedFields&, Assembler::ScalarRegisterPtr, 
                                                   Assembler::ScalarRegisterPtr rowBufferAReg, Assembler::ScalarRegisterPtr rowBufferBReg, 
                                                   uint32_t eventSinkSourceTableAddress, uint32_t&, Assembler::CodeGenerator &c, 
                                                   Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                   DMARowFn dmaRow, DMAWaitFn dmaWait, ProcessDMAEventFn processEvent) const
{
    // Wait for all events from last timestep to be communicated
    Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, runtime.getNumDevices());

    uint32_t scalarRegisterMask = 0;
    ALLOCATE_SCALAR_AND_MASK(SPreIndReg);
    ALLOCATE_SCALAR_AND_MASK(SSpikeBuffer);
    ALLOCATE_SCALAR_AND_MASK(SSpikeBufferEnd);
    ALLOCATE_SCALAR_AND_MASK(SPrevEventSourceProcessOffset);
    ALLOCATE_SCALAR_AND_MASK(SNextEventSourceProcessOffset);
    ALLOCATE_SCALAR_AND_MASK(SEventSourceProcessEndOffset);

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

    auto noSpikes = Assembler::createLabel();
    auto lastProcess = Assembler::createLabel();
    auto nextProcess = Assembler::createLabel();

    // If there are no spikes, goto end
    c.beq(*SSpikeBuffer, *SSpikeBufferEnd, noSpikes);
    {
        // Prefetch
        {
            ALLOCATE_SCALAR(SEventSinkID);

            // Load first spike and increment pointer
            c.lw(*SPreIndReg, *SSpikeBuffer);
            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

            // Extract event sink ID
            // **NOTE** these are multiplied by two
            c.srli(*SEventSinkID, *SPreIndReg, kernel.getNumNeuronIDBits());
             
            // AND neuron ID with mask
            if(Common::inSBit(neuronIDMask, 12)) {
                c.andi(*SPreIndReg, *SPreIndReg, neuronIDMask);
            }
            else {
                c.and_(*SPreIndReg, *SPreIndReg, *neuronIDMaskReg);
            }

            // Load start and end offset of block of event propagation processes which should be passed this spike
            c.lhu(*SPrevEventSourceProcessOffset, *SEventSinkID, eventSinkSourceTableAddress);
            c.lhu(*SEventSourceProcessEndOffset, *SEventSinkID, eventSinkSourceTableAddress + 2);
            
            // Start DMA to prefetch first row into buffer A
            dmaRow(c, SPrevEventSourceProcessOffset, SPreIndReg, rowBufferAReg);

            // Get offset of next process to DMA
            c.addi(*SNextEventSourceProcessOffset, *SPrevEventSourceProcessOffset, 16);
        }

        auto spikeLoop = c.L(); 
        {
            ALLOCATE_SCALAR(SEventSinkID);

            // If there are more processes associated with event source, jump to continue handling
            c.bne(*SNextEventSourceProcessOffset, *SEventSourceProcessEndOffset, nextProcess);

            // If last spike has been processed, handle final process
            c.beq(*SSpikeBuffer, *SSpikeBufferEnd, lastProcess);

            // Otherwise, load next spike and increment pointer
            c.lw(*SPreIndReg, *SSpikeBuffer);
            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

            // Extract event sink ID
            // **NOTE** these are multiplied by two
            c.srli(*SEventSinkID, *SPreIndReg, kernel.getNumNeuronIDBits());
             
            // AND neuron ID with mask
            if(Common::inSBit(neuronIDMask, 12)) {
                c.andi(*SPreIndReg, *SPreIndReg, neuronIDMask);
            }
            else {
                c.and_(*SPreIndReg, *SPreIndReg, *neuronIDMaskReg);
            }

            // Load start and end offset of block of event propagation processes which should be passed this spike
            c.lhu(*SNextEventSourceProcessOffset, *SEventSinkID, eventSinkSourceTableAddress);
            c.lhu(*SEventSourceProcessEndOffset, *SEventSinkID, eventSinkSourceTableAddress + 2);
            
            c.L(nextProcess);

            // Wait for previous DMA to complete
            dmaWait(c);

            // Start DMA to fetch next row into buffer B while this one is being processed
            dmaRow(c, SNextEventSourceProcessOffset, SPreIndReg, rowBufferBReg);

            // Process event
            processEvent(c, SPrevEventSourceProcessOffset /*rowBufferAReg*/);
            
            // Update previous event source process start offset
            c.mv(*SPrevEventSourceProcessOffset, *SNextEventSourceProcessOffset);

            // Go onto next process
            c.addi(*SNextEventSourceProcessOffset, *SNextEventSourceProcessOffset, 16);

            // Swap buffers
            {
                ALLOCATE_SCALAR(STmp);
                c.mv(*STmp, *rowBufferAReg);
                c.mv(*rowBufferAReg, *rowBufferBReg);
                c.mv(*rowBufferBReg, *STmp);
            }
            c.j_(spikeLoop);
        }
    }
    c.L(lastProcess);

    // Wait for previous DMA to complete
    dmaWait(c);

    // Process event
    processEvent(c, SPrevEventSourceProcessOffset/*, rowBufferAReg*/);

    
    c.L(noSpikes);
  
    return scalarRegisterMask;
}

//----------------------------------------------------------------------------
// FeNN::Backend::GenX320EventChannelSink
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> GenX320EventChannelSink::createArray(std::optional<size_t>, uint32_t, size_t, 
                                                                          const Frontend::Model&, Frontend::DeviceBase&) const
{
    return nullptr;
}
//----------------------------------------------------------------------------
Frontend::State::ShapeStride GenX320EventChannelSink::getArrayShapeStride(std::optional<size_t>, uint32_t, size_t,
                                                                          size_t, const Frontend::Model&) const
{
    return std::make_tuple(std::vector<size_t>{}, std::vector<size_t>{});
}
}
