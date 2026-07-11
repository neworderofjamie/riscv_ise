// Google test includes
#include "gtest/gtest.h"

// Standard C++ includes
#include <stdexcept>

// Model includes
#include "frontend/events.h"
#include "frontend/model.h"
#include "frontend/kernel.h"
#include "frontend/process.h"
#include "frontend/process_group.h"
#include "frontend/shape.h"

using namespace CompilerFrontend;
using namespace Frontend;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(Model, StateProcess)
{
    const Shape inputShape(28 * 28);
    const Shape hiddenShape(128);
    const Shape outputShape(10);

    // Input spikes
    const auto inputSpikes = EventSourceBuffer::create(inputShape);

    // Hidden neurons
    const double alpha = std::exp(-1.0 / 20.0);
    const auto hiddenV = Variable::create(hiddenShape, Type::Float);
    const auto hiddenI = Variable::create(hiddenShape, Type::Float);
    const auto hiddenSpikes = EventChannel::create(hiddenShape);
    const auto hidden = NeuronUpdateProcess::create(
        "V = (" + std::to_string(alpha) + " * V) + I;\n"
        "I = 0.0;\n"
        "if(V >= 1.0) {\n"
        "   Spike();\n"
        "   V -= 1.0h5;\n"
        "}\n",
        {{"V", Sliced<Variable>(hiddenV)}, {"I", Sliced<Variable>(hiddenI)}},
        {{"Spike", Sliced<EventSink>(hiddenSpikes)}},
        Type::Float);

    // Output neurons
    const auto outputV = Variable::create(outputShape, Type::Float);
    const auto outputI = Variable::create(outputShape, Type::Float);
    const auto output = NeuronUpdateProcess::create(
        "V = (" + std::to_string(alpha) + " * V) + I;\n"
        "I = 0.0;\n",
        {{"V", Sliced<Variable>(outputV)}, {"I", Sliced<Variable>(outputI)}}, {},
        Type::Float);

    // Input->Hidden event propagation
    const auto inputHidden = EventPropagationProcess::create(inputSpikes, hiddenI);

    // Hidden->Output event propagation
    const auto hiddenOutput = EventPropagationProcess::create(hiddenSpikes, outputI);

    const auto neuronUpdateProcesses = ProcessGroup::create({hidden, output});
    const auto synapseUpdateProcesses = ProcessGroup::create({inputHidden, hiddenOutput});

    // Define kernel
    const auto kernel = SimulationLoopKernel::create(100, {neuronUpdateProcesses, synapseUpdateProcesses});

    // Build model
    Model model({kernel});

    const auto &inputSpikesData = model.getStateData(inputSpikes);
    EXPECT_EQ(inputSpikesData.splitDimension, 0);
    ASSERT_EQ(inputSpikesData.processes.size(), 1);
    EXPECT_EQ(inputSpikesData.processes[0], inputHidden);

    const auto &hiddenVStateData = model.getStateData(hiddenV);
    EXPECT_EQ(hiddenVStateData.splitDimension, 0);
    ASSERT_EQ(hiddenVStateData.processes.size(), 1);
    EXPECT_EQ(hiddenVStateData.processes[0], hidden);

    const auto &hiddenIStateData = model.getStateData(hiddenI);
    EXPECT_EQ(hiddenIStateData.splitDimension, 0);
    ASSERT_EQ(hiddenIStateData.processes.size(), 2);
    EXPECT_NE(std::find(hiddenIStateData.processes.cbegin(), hiddenIStateData.processes.cend(), inputHidden), hiddenIStateData.processes.cend());
    EXPECT_NE(std::find(hiddenIStateData.processes.cbegin(), hiddenIStateData.processes.cend(), hidden), hiddenIStateData.processes.cend());

    const auto &hiddenSpikesStateData = model.getStateData(hiddenSpikes);
    EXPECT_EQ(hiddenSpikesStateData.splitDimension, 0);
    ASSERT_EQ(hiddenSpikesStateData.processes.size(), 2);
    EXPECT_NE(std::find(hiddenSpikesStateData.processes.cbegin(), hiddenSpikesStateData.processes.cend(), hidden), hiddenSpikesStateData.processes.cend());
    EXPECT_NE(std::find(hiddenSpikesStateData.processes.cbegin(), hiddenSpikesStateData.processes.cend(), hiddenOutput), hiddenSpikesStateData.processes.cend());

    const auto &outputVStateData = model.getStateData(outputV);
    EXPECT_EQ(outputVStateData.splitDimension, 0);
    ASSERT_EQ(outputVStateData.processes.size(), 1);
    EXPECT_EQ(outputVStateData.processes[0], output);

    const auto &outputIStateData = model.getStateData(outputI);
    EXPECT_EQ(outputIStateData.splitDimension, 0);
    ASSERT_EQ(outputIStateData.processes.size(), 2);
    EXPECT_NE(std::find(outputIStateData.processes.cbegin(), outputIStateData.processes.cend(), output), outputIStateData.processes.cend());
    EXPECT_NE(std::find(outputIStateData.processes.cbegin(), outputIStateData.processes.cend(), hiddenOutput), outputIStateData.processes.cend());
}
//--------------------------------------------------------------------------
TEST(MergedModel, MergingDifferentProcessTypes)
{
    // Create a bunch of different processes
    auto variable = Variable::create(20, Type::Uint32);
    auto rngInitProcess = RNGInitProcess::create(variable);
    auto memsetProcess = MemsetProcess::create(variable);
    auto neuronUpdateProcess = NeuronUpdateProcess::create(
        "V *= 0.9\n",
        {{"V", Sliced<Variable>(variable)}}, {});
    auto eventPropagationProcess = EventPropagationProcess::create(
        EventSourceBuffer::create(20), variable);

    // Group together
    auto processGroup = ProcessGroup::create({rngInitProcess, memsetProcess, neuronUpdateProcess, eventPropagationProcess});

    // Define kernel
    const auto kernel = SimpleKernel::create({processGroup});

    // Build model
    Model model({kernel});

    // Check merging has resulted in 4 seperate merged process groups with one process in each
    const auto &mergedProcessGroups = model.getMergedProcessGroups().at(processGroup).getMergedProcesses();
    EXPECT_EQ(mergedProcessGroups.size(), 4);
    for(const auto &m : mergedProcessGroups) {
        EXPECT_EQ(m.getMerged().size(), 1);
    }


}