// Standard C++ includes
#include <unordered_set>

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

class EventSourceTest : public testing::Test
{
protected:
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
};
}

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST_P(EventSinkIDTest, EventSinkIDAllocation)
{
    // Build shape from first parameter
    const Shape shape{{std::get<0>(GetParam())}};

    // Populate vector with second parameter number of populations of this size
    std::vector<std::shared_ptr<const Process>> processes;
    processes.reserve(std::get<1>(GetParam()));
    for(size_t p = 0; p < std::get<1>(GetParam()); p++) {
        const auto v = Backend::Variable::create(shape, Type::S2_13Sat);
        const auto i = Backend::Variable::create(shape, Type::S2_13Sat);
        const auto spikes = Backend::EventSinkBuffer::create(shape);
        processes.push_back(Backend::NeuronUpdateProcess::create(
            "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
            "if(V >= 1.0) {\n"
            "   Spike();\n"
            "   V = 0.0;\n"
            "}\n",
            {{"V", Sliced<Variable>(v)}, {"I", Sliced<Variable>(i)}},
            {{"Spike", Sliced<EventSink>(spikes)}},
            Type::S2_13));
    }

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create(processes);

    // Create simple kernel
    std::shared_ptr<Backend::SimpleKernel> kernel;
    try {
        kernel = Backend::SimpleKernel::create({neuronUpdateProcesses});
    }
    catch(const std::runtime_error &e) {
        // If test SHOULD suceed, rethrow exception
        if(std::get<2>(GetParam())) {
            throw e;
        }
        // Otherwise, exit cleanly
        else {
            return;
        }
    }
    
    // Check allocated number of bits
    ASSERT_EQ(kernel->getNumNeuronIDBits(), (32 - Utils::clz(std::max(32ull, std::get<0>(GetParam())) - 1)));
    ASSERT_EQ(kernel->getNumPopulationIDBits(), (32 - Utils::clz((4 * std::get<1>(GetParam())) - 1)));

    // Loop through allocated event sink IDs
    std::unordered_set<uint32_t> ids;
    for(const auto &e : kernel->getEventSinkIDs()) {
        // Add to set
        ids.insert(e.second);

        // Get event ID base for this event sink
        const uint32_t eventIDBase = kernel->getEventSinkIDBase(e.first);

        // Check that base event ID doens't have lower bits set
        ASSERT_EQ(eventIDBase & 0x1F, 0);

        // Check that it matches the ID shifted up
        ASSERT_EQ(eventIDBase, e.second << kernel->getNumNeuronIDBits());
    }

    // Check that a unique ID has been allocated for each 
    EXPECT_EQ(ids.size(), std::get<1>(GetParam()));
}
//--------------------------------------------------------------------------
TEST_F(EventSourceTest, MultipleEventSourceProcessGroups)
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

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({pre1, pre2});
    const auto eventPropagation1Processes = ProcessGroup::create({pre1Post});
    const auto eventPropagation2Processes = ProcessGroup::create({pre2Post});

    // Create kernel
    EXPECT_THROW(Backend::SimpleKernel::create({neuronUpdateProcesses, eventPropagation1Processes, eventPropagation2Processes}),
                 std::runtime_error);
}
//--------------------------------------------------------------------------
TEST_F(EventSourceTest, EventSourceMerging)
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

    // Check that the event chanels and event source buffers have been seperated
    ASSERT_EQ(kernel->getMergedEventSources().size(), 2);

    // Check there are two event sources in each merged group
    const auto &firstMergedGroup = kernel->getMergedEventSources().at(0).getMerged();
    const auto &secondMergedGroup = kernel->getMergedEventSources().at(1).getMerged();
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
//--------------------------------------------------------------------------
TEST_F(EventSourceTest, EventSourceProcesses)
{
    // Create 2 presynaptic neuron update 
    const auto [pre1, pre1EventChannel] = createPre(64);
    const auto [pre2, pre2EventChannel] = createPre(128);

    // Create several postsynaptic neuron update processes
    const auto [post1, post1Target] = createPost(64);
    const auto [post2, post2Target] = createPost(96);
    const auto [post3, post3Target] = createPost(128);
    const auto [post4, post4Target] = createPost(160);

    // Connect pre1 to post 1
    const auto pre1Post1Weight = Backend::Variable::create(Frontend::Shape({64, 64}), Type::S2_13Sat);
    const auto pre1Post1 = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre1EventChannel),
                                                                         pre1Post1Weight,
                                                                         Sliced<Variable>(post1Target));

    // Connect pre2 to post 2-4
    const auto pre2Post2Weight = Backend::Variable::create(Frontend::Shape({128, 96}), Type::S2_13Sat);
    const auto pre2Post2 = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre2EventChannel),
                                                                         pre2Post2Weight,
                                                                         Sliced<Variable>(post2Target));
    const auto pre2Post3Weight = Backend::Variable::create(Frontend::Shape({128, 128}), Type::S2_13Sat);
    const auto pre2Post3 = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre2EventChannel),
                                                                         pre2Post3Weight,
                                                                         Sliced<Variable>(post3Target));
    const auto pre2Post4Weight = Backend::Variable::create(Frontend::Shape({128, 160}), Type::S2_13Sat);
    const auto pre2Post4 = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(pre2EventChannel),
                                                                         pre2Post4Weight,
                                                                         Sliced<Variable>(post4Target));

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({pre1, pre2, post1, post2, post3, post4});
    const auto eventPropagationProcesses = ProcessGroup::create({pre1Post1, pre2Post2, pre2Post3, pre2Post4});

    // Create kernel
    const auto kernel = Backend::SimpleKernel::create({neuronUpdateProcesses, eventPropagationProcesses});

    // Check there are only two event sources
    ASSERT_EQ(kernel->getEventSourceProcesses().size(), 2);

    // Check pre1EventChannel is connected to one process
    const auto &pre1EventChannelProcesses = kernel->getEventSourceProcesses().at(pre1EventChannel);
    ASSERT_EQ(pre1EventChannelProcesses.size(), 1);
    ASSERT_NE(std::find(pre1EventChannelProcesses.cbegin(), pre1EventChannelProcesses.cend(), pre1Post1), pre1EventChannelProcesses.cend());

    // Check pre2EventChannel is connected to there processes
    const auto &pre2EventChannelProcesses = kernel->getEventSourceProcesses().at(pre2EventChannel);
    ASSERT_EQ(pre2EventChannelProcesses.size(), 3);
    ASSERT_NE(std::find(pre2EventChannelProcesses.cbegin(), pre2EventChannelProcesses.cend(), pre2Post2), pre2EventChannelProcesses.cend());
    ASSERT_NE(std::find(pre2EventChannelProcesses.cbegin(), pre2EventChannelProcesses.cend(), pre2Post3), pre2EventChannelProcesses.cend());
    ASSERT_NE(std::find(pre2EventChannelProcesses.cbegin(), pre2EventChannelProcesses.cend(), pre2Post4), pre2EventChannelProcesses.cend());

}

//--------------------------------------------------------------------------
// Test suites
//--------------------------------------------------------------------------
INSTANTIATE_TEST_SUITE_P(Kernel,
                         EventSinkIDTest,
                         testing::Values(std::make_tuple(10, 2, true),                  // Small population - event id base needs padding
                                         std::make_tuple(32, 4, true),                  // Fine
                                         std::make_tuple(1024, 10, true),               // Fine
                                         std::make_tuple((1 << 22) - 1, 1, true),       // Whole keyspace goes to one population - weird but maybe ok
                                         std::make_tuple((1 << 22) - 1, 100, false)));  // Not enough keyspace - should fail