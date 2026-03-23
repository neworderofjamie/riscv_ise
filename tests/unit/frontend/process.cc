// Google test includes
#include "gtest/gtest.h"

// Standard C++ includes
#include <stdexcept>

// Model includes
#include "frontend/event_container.h"
#include "frontend/process.h"
#include "frontend/shape.h"

using namespace CompilerFrontend;
using namespace Frontend;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(NeuronProcess, HasVariablesOrOutputEvents)
{
    EXPECT_THROW(
        {
            NeuronUpdateProcess::create(
                "int x = 12;\n",
                {}, {});
        },
        std::runtime_error);
}

TEST(NeuronProcess, VarOutputEventShapesMatch)
{
    const Shape shape(50);
    const Shape diffShape({30, 12});
    const Shape sliceShape({10, 50});
    
    // Create some variables
    const auto var = Variable::create(shape, Type::S10_5Sat);
    const auto varSameShape = Variable::create(shape, Type::Int32);
    const auto varDiffShape = Variable::create(diffShape, Type::Float);
    const auto varSliceShape = Variable::create(sliceShape, Type::S8_7);
    
    // Create some event containers
    const auto eventContainer = EventContainer::create(shape);
    const auto eventContainerDiffShape = EventContainer::create(diffShape);
    const auto eventContainerSliceShape = EventContainer::create(sliceShape);
    
    // Everything matches
    NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSameShape)}}, 
        {{"Spike", Sliced<EventContainer>(eventContainer)}});
    
    // Mismatched variable
    EXPECT_THROW(
        {
            NeuronUpdateProcess::create(
                "A = B * 0.5;\n"
                "Spike();\n",
                {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varDiffShape)}}, 
                {{"Spike", Sliced<EventContainer>(eventContainer)}});
        },
        std::runtime_error);
    
    // Mismatched output event
    EXPECT_THROW(
        {
            NeuronUpdateProcess::create(
                "A = B * 0.5;\n"
                "Spike();\n",
                {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varDiffShape)}}, 
                {{"Spike", Sliced<EventContainer>(eventContainerDiffShape)}});
        },
        std::runtime_error);
 
    // Sliced variable
    NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSliceShape, true)}}, 
        {{"Spike", Sliced<EventContainer>(eventContainer)}});
    
    // Sliced output event
    NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSameShape)}}, 
        {{"Spike", Sliced<EventContainer>(eventContainerSliceShape, true)}});
}
