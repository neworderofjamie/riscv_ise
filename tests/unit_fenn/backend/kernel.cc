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
// Tests
//--------------------------------------------------------------------------
class EventSinkIDTest : public testing::TestWithParam<std::tuple<size_t, size_t, bool>>
{
};
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
    ASSERT_EQ(kernel->getNumPopulationIDBits(), (32 - Utils::clz(std::get<1>(GetParam()) - 1)));

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

INSTANTIATE_TEST_SUITE_P(Kernel,
                         EventSinkIDTest,
                         testing::Values(std::make_tuple(10, 2, true),                  // Small population - event id base needs padding
                                         std::make_tuple(32, 4, true),                  // Fine
                                         std::make_tuple(1024, 10, true),               // Fine
                                         std::make_tuple((1 << 24) - 1, 1, true),       // Whole keyspace goes to one population - weird but maybe ok
                                         std::make_tuple((1 << 24) - 1, 100, false)));  // Not enough keyspace - should fail