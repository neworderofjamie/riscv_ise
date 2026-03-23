// Google test includes
#include "gtest/gtest.h"

// Standard C++ includes
#include <stdexcept>

// Model includes
#include "frontend/event_container.h"
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
    const auto inputSpikes = EventContainer::create(inputShape);

    // Hidden neurons
    const double alpha = std::exp(-1.0 / 20.0);
    const auto hiddenV = Variable::create(hiddenShape, Type::Float);
    const auto hiddenI = Variable::create(hiddenShape, Type::Float);
    const auto hiddenSpikes = EventContainer::create(hiddenShape, 1);
    const auto hidden = NeuronUpdateProcess::create(
        "V = (" + std::to_string(alpha) + " * V) + I;\n"
        "I = 0.0;\n"
        "if(V >= 1.0) {\n"
        "   Spike();\n"
        "   V -= 1.0h5;\n"
        "}\n",
        {{"V", Sliced<Variable>(hiddenV)}, {"I", Sliced<Variable>(hiddenI)}},
        {{"Spike", Sliced<EventContainer>(hiddenSpikes)}},
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
}