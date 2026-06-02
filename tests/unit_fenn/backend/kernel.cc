// Google test includes
#include "gtest/gtest.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// Frontend includes
#include "frontend/process_group.h"

// FeNN backend includes
#include "fenn/backend/events.h"
#include "fenn/backend/kernel.h"
#include "fenn/backend/process.h"
#include "fenn/backend/variable.h"

using namespace CompilerFrontend;
using namespace Frontend;
using namespace FeNN;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(Kernel, EventSinkIDAllocation)
{
    const Shape hidden1Shape{{32}};
    const Shape hidden2Shape{{30}};

    // Hidden neurons
    const auto hidden1V = Backend::Variable::create(hidden1Shape, Type::S2_13Sat);
    const auto hidden1I = Backend::Variable::create(hidden1Shape, Type::S2_13Sat);
    const auto hidden1Spikes = Backend::EventSinkBuffer::create(hidden1Shape);
    const auto hidden1 = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
        "if(V >= 1.0) {\n"
        "   Spike();\n"
        "   V = 0.0;\n"
        "}\n",
        {{"V", Sliced<Variable>(hidden1V, true)}, {"I", Sliced<Variable>(hidden1I)}}, 
        {{"Spike", Sliced<EventSink>(hidden1Spikes, true)}},
        Type::S2_13);

    // Hidden neurons
    const auto hidden2V = Backend::Variable::create(hidden2Shape, Type::S2_13Sat);
    const auto hidden2I = Backend::Variable::create(hidden2Shape, Type::S2_13Sat);
    const auto hidden2Spikes = Backend::EventSinkBuffer::create(hidden2Shape);
    const auto hidden2 = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
        "if(V >= 0.8) {\n"
        "   Spike();\n"
        "   V = 0.0;\n"
        "}\n",
        {{"V", Sliced<Variable>(hidden2V, true)}, {"I", Sliced<Variable>(hidden2I)}}, 
        {{"Spike", Sliced<EventSink>(hidden2Spikes, true)}},
        Type::S2_13);

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({hidden1, hidden2});

    // Create simple kernel
    const auto kernel = Backend::SimpleKernel::create({neuronUpdateProcesses});
    
}