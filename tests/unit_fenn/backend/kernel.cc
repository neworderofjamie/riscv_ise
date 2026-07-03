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
    const Shape shape{{std::get<0>(GetParam())}};

    std::vector<std::shared_ptr<const Process>> processes;

    for(size_t p = 0; p < std::get<1>(GetParam()); p++) {
        // Hidden neurons
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
    ASSERT_EQ(kernel->getNumNeuronIDBits(), (32 - Utils::clz(std::get<0>(GetParam()) - 1)));
    ASSERT_EQ(kernel->getNumPopulationIDBits(), (32 - Utils::clz(std::get<1>(GetParam()) - 1)));

    /*
    // Get event sink IDs for the two groups and assert that they are different
    const auto hidden1EventSinkID = kernel->getEventSinkIDs().at(hidden1Spikes);
    const auto hidden2EventSinkID = kernel->getEventSinkIDs().at(hidden2Spikes);
    ASSERT_NE(hidden1EventSinkID, hidden2EventSinkID);

    const uint32_t hidden1EventIDBase = kernel->getEventSinkIDBase(hidden1Spikes);
    const uint32_t hidden2EventIDBase = kernel->getEventSinkIDBase(hidden2Spikes);*/   
}

INSTANTIATE_TEST_SUITE_P(Kernel,
                         EventSinkIDTest,
                         testing::Values(std::make_tuple(32, 4, true),
                                         std::make_tuple(1024, 10, true),
                                         std::make_tuple((1 << 24) - 1, 1, true),
                                         std::make_tuple((1 << 24) - 1, 100, false)));