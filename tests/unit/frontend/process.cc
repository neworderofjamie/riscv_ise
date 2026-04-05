// Google test includes
#include "gtest/gtest.h"

// Standard C++ includes
#include <stdexcept>

// Model includes
#include "frontend/events.h"
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
    
    // Create some event channels
    const auto eventChannel = EventChannel::create(shape);
    const auto eventChannelDiffShape = EventChannel::create(diffShape);
    const auto eventChannelSliceShape = EventChannel::create(sliceShape);
    
    // Everything matches
    NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSameShape)}}, 
        {{"Spike", Sliced<EventSink>(eventChannel)}});
    
    // Mismatched variable
    EXPECT_THROW(
        {
            NeuronUpdateProcess::create(
                "A = B * 0.5;\n"
                "Spike();\n",
                {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varDiffShape)}}, 
                {{"Spike", Sliced<EventSink>(eventChannel)}});
        },
        std::runtime_error);
    
    // Mismatched output event
    EXPECT_THROW(
        {
            NeuronUpdateProcess::create(
                "A = B * 0.5;\n"
                "Spike();\n",
                {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varDiffShape)}}, 
                {{"Spike", Sliced<EventSink>(eventChannelDiffShape)}});
        },
        std::runtime_error);
 
    // Sliced variable
    auto slicedVarProcess = NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSliceShape, true)}}, 
        {{"Spike", Sliced<EventSink>(eventChannel)}});
    
    EXPECT_EQ(slicedVarProcess->getShape(), Shape(50));

    // Sliced output event
    auto slicedOutputProcess = NeuronUpdateProcess::create(
        "A = B * 0.5;\n"
        "Spike();\n",
        {{"A", Sliced<Variable>(var)}, {"V", Sliced<Variable>(varSameShape)}}, 
        {{"Spike", Sliced<EventSink>(eventChannelSliceShape, true)}});

    EXPECT_EQ(slicedOutputProcess->getShape(), Shape(50));
}

TEST(NeuronProcess, LiteralExtract)
{
    // Create neuron update with lots of literals
    auto process = NeuronUpdateProcess::create(
        "int x = 12;\n"
        "x += (int)12u;\n"
        "x += (int)5.0;\n"
        "x += (int)5.2f;\n"
        "x += (int)12.0d;\n"
        "V = x;\n",
        {{"V", Sliced<Variable>(Variable::create(Shape{50}, Type::Int32))}});

    const auto &literals = process->getLiterals();
    ASSERT_EQ(literals.size(), 5);
    EXPECT_EQ(literals[0], std::make_tuple(Type::Int32, Type::NumericValue(12), 3));
    EXPECT_EQ(literals[1], std::make_tuple(Type::Uint32, Type::NumericValue(12u), 10));
    
    EXPECT_EQ(std::get<0>(literals[2]), Type::Float);
    EXPECT_FLOAT_EQ(std::get<1>(literals[2]).cast<float>(), 5.0f);
    EXPECT_EQ(std::get<2>(literals[2]), 17);

    EXPECT_EQ(std::get<0>(literals[3]), Type::Float);
    EXPECT_FLOAT_EQ(std::get<1>(literals[3]).cast<float>(), 5.2f);
    EXPECT_EQ(std::get<2>(literals[3]), 24);

    EXPECT_EQ(std::get<0>(literals[4]), Type::Double);
    EXPECT_DOUBLE_EQ(std::get<1>(literals[4]).cast<double>(), 12.0);
    EXPECT_EQ(std::get<2>(literals[4]), 31);

}
