// Google test includes
#include "gtest/gtest.h"

// Common includes
#include "common/utils.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// Frontend includes
#include "frontend/process_group.h"

// FeNN backend includes
#include "fenn/backend/events.h"
#include "fenn/backend/kernel.h"
#include "fenn/backend/process.h"
#include "fenn/backend/runtime_sim.h"
#include "fenn/backend/variable.h"

using namespace ::Common;
using namespace CompilerFrontend;
using namespace Frontend;
using namespace FeNN;

//--------------------------------------------------------------------------
// Anonymous namespace
//--------------------------------------------------------------------------
namespace
{
class EventSinkIDTest : public testing::TestWithParam<std::tuple<size_t, size_t, bool>>
{
};

std::tuple<std::shared_ptr<Backend::NeuronUpdateProcess>, 
    std::shared_ptr<Backend::EventChannel>> createPre(size_t numNeurons)
{
    const Shape shape{{numNeurons}};
    const auto v = Backend::Variable::create(shape, Type::S2_13Sat);
    const auto i = Backend::Variable::create(shape, Type::S2_13Sat);
    const auto eventChannel = Backend::EventChannel::create(shape);
    const auto process = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
        "if(V >= 1.0) {\n"
        "   Spike();\n"
        "   V = 0.0;\n"
        "}\n",
        {{"V", Sliced<Variable>(v)}, {"I", Sliced<Variable>(i)}},
        {{"Spike", Sliced<EventSink>(eventChannel)}},
        Type::S2_13);

    return std::make_tuple(process, eventChannel);
}

std::tuple<std::shared_ptr<Backend::NeuronUpdateProcess>,
    std::shared_ptr<Backend::Variable>> createPost(size_t numNeurons)
{
    const Shape shape{{numNeurons}};
    const auto v = Backend::Variable::create(shape, Type::S2_13Sat);
    const auto i = Backend::Variable::create(shape, Type::S2_13Sat);
    const auto eventBuffer = Backend::EventSinkBuffer::create(shape);
    const auto process = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
        "if(V >= 1.0) {\n"
        "   Spike();\n"
        "   V = 0.0;\n"
        "}\n",
        {{"V", Sliced<Variable>(v)}, {"I", Sliced<Variable>(i)}},
        {{"Spike", Sliced<EventSink>(eventBuffer)}},
        Type::S2_13);

    return std::make_tuple(process, i);
}
}

//--------------------------------------------------------------------------
// Runtime
//--------------------------------------------------------------------------
TEST(Runtime, DifferentProcessTypeMerging)
{
    // Create a bunch of different processes
    auto variable = Backend::Variable::create(20, Type::S2_13Sat);
    auto rngVariable = Backend::Variable::create(Shape({1, 64}), Type::Int16);
    auto rngInitProcess = Backend::RNGInitProcess::create(rngVariable);
    auto memsetProcess = Backend::MemsetProcess::create(variable);
    auto neuronUpdateProcess = Backend::NeuronUpdateProcess::create(
        "V *= 0.9;\n",
        {{"V", Sliced<Variable>(variable)}}, {});
    //auto eventPropagationProcess = Backend::DenseEventPropagationProcess::create(
    //    EventSourceBuffer::create(20), variable);

    // Group together
    auto processGroup = ProcessGroup::create({rngInitProcess, memsetProcess, neuronUpdateProcess/*, eventPropagationProcess*/});

    // Define kernel
    const auto kernel = Backend::SimpleKernel::create({processGroup});


    Backend::RuntimeSim runtime({kernel}, 1);

    // Check merging has resulted in 4 seperate merged process groups with one process in each
    const auto &mergedProcessGroups = runtime.getMergedProcessGroups().at(processGroup).getMergedProcesses();
    EXPECT_EQ(mergedProcessGroups.size(), 3);
    for(const auto &m : mergedProcessGroups) {
        EXPECT_EQ(m.getMerged().size(), 1);
    }
}
//--------------------------------------------------------------------------
TEST(Runtime, EventSourceMerging)
{
    // Create 2 presynaptic neuron update 
    const auto [pre1, pre1EventChannel] = createPre(64);
    const auto [pre2, pre2EventChannel] = createPre(128);

    // Create postsynaptic neuron update process
    const auto [post, postTarget] = createPost(96);

    // Create event propagation processes
    const auto pre1PostWeight = Backend::Variable::create(Frontend::Shape({64, 96}), Type::S2_13Sat);
    const auto pre1Post = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre1EventChannel),
                                                                        pre1PostWeight,
                                                                        Sliced<Variable>(postTarget));

    const auto pre2PostWeight = Backend::Variable::create(Frontend::Shape({128, 96}), Type::S2_13Sat);
    const auto pre2Post = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre2EventChannel),
                                                                        pre2PostWeight,
                                                                        Sliced<Variable>(postTarget));

    const auto pre3EventBuffer = Backend::EventSourceBuffer::create(Frontend::Shape{{96}});
    const auto pre3PostWeight = Backend::Variable::create(Frontend::Shape({96, 96}), Type::S2_13Sat);
    const auto pre3Post = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre3EventBuffer),
                                                                        pre3PostWeight,
                                                                        Sliced<Variable>(postTarget));

    const auto pre4EventBuffer = Backend::EventSourceBuffer::create(Frontend::Shape{{160}});
    const auto pre4PostWeight = Backend::Variable::create(Frontend::Shape({160, 96}), Type::S2_13Sat);
    const auto pre4Post = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre4EventBuffer),
                                                                        pre4PostWeight,
                                                                        Sliced<Variable>(postTarget));

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({pre1, pre2, post});
    const auto eventPropagationProcesses = ProcessGroup::create({pre1Post, pre2Post, pre3Post, pre4Post});

    // Create kernel
    const auto kernel = Backend::SimpleKernel::create({neuronUpdateProcesses, eventPropagationProcesses});

    // Check event propagation process group has been correctly identified
    ASSERT_EQ(kernel->getEventSourceProcessGroup(), eventPropagationProcesses);

    // Create runtime
    Backend::RuntimeSim runtime({kernel}, 1);

    // Check that the event chanels and event source buffers have been seperated
    const auto &mergedEventSource = runtime.getMergedEventSources().at(kernel->getEventSourceProcessGroup());
    ASSERT_EQ(mergedEventSource.size(), 2);

    // Check there are two event sources in each merged group
    const auto &firstMergedGroup = mergedEventSource.at(0).getMerged();
    const auto &secondMergedGroup = mergedEventSource.at(1).getMerged();
    ASSERT_EQ(firstMergedGroup.size(), 2);
    ASSERT_EQ(secondMergedGroup.size(), 2);

    // If first merged group contains 1st event chnanel
    if(std::find(firstMergedGroup.cbegin(), firstMergedGroup.cend(), pre1EventChannel) != firstMergedGroup.cend()) {
        // First merged group should contain 2nd event channel
        ASSERT_NE(std::find(firstMergedGroup.cbegin(), firstMergedGroup.cend(), pre2EventChannel), firstMergedGroup.cend());

        // Second merged group should contain both event buffers
        ASSERT_NE(std::find(secondMergedGroup.cbegin(), secondMergedGroup.cend(), pre3EventBuffer), secondMergedGroup.cend());
        ASSERT_NE(std::find(secondMergedGroup.cbegin(), secondMergedGroup.cend(), pre4EventBuffer), secondMergedGroup.cend());
    }
    // Otherwise
    else {
        // First merged group should contain both event buffers
        ASSERT_NE(std::find(firstMergedGroup.cbegin(), firstMergedGroup.cend(), pre3EventBuffer), firstMergedGroup.cend());
        ASSERT_NE(std::find(firstMergedGroup.cbegin(), firstMergedGroup.cend(), pre4EventBuffer), firstMergedGroup.cend());

        // Second merged group should contain both event channels
        ASSERT_NE(std::find(secondMergedGroup.cbegin(), secondMergedGroup.cend(), pre1EventChannel), secondMergedGroup.cend());
        ASSERT_NE(std::find(secondMergedGroup.cbegin(), secondMergedGroup.cend(), pre2EventChannel), secondMergedGroup.cend());
    }
}