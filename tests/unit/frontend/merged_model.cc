// Google test includes
#include "gtest/gtest.h"

// Standard C++ includes
#include <stdexcept>

// Model includes
#include "frontend/events.h"
#include "frontend/merged_model.h"
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
TEST(MergedModel, DifferentProcessTypes)
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

    // Merge
    MergedModel mergedModel(model);

    // Check merging has resulted in 4 seperate merged process groups with one process in each
    const auto &mergedProcessGroups = mergedModel.getMergedProcessGroups().at(processGroup);
    EXPECT_EQ(mergedProcessGroups.size(), 4);
    for(const auto &m : mergedProcessGroups) {
        EXPECT_EQ(m.getMerged().size(), 1);
    }


}