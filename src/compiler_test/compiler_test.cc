// Standard C++ includes
#include <bitset>
#include <chrono>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>

// Standard C includes
#include <cassert>
#include <cmath>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// GeNN includes
#include "type.h"

// RISC-V utils include
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"
#include "common/utils.h"

// RISC-V backend includes
#include "backend/backend_fenn_sim.h"

// RISC-V assembler includes
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

// **HACK**
void disassemble(std::ostream &os, uint32_t inst);

int main(int argc, char** argv)
{
    const Shape inputShape{{28 * 28}};
    const Shape hiddenShape{{128}};
    const Shape outputShape{{10}};
    const Shape inputHiddenShape{{28 * 28, 128}};
    const Shape hiddenOutputShape{{128, 10}};

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    bool device = false;
    size_t numExamples = 10000;

    CLI::App app{"Latency MNIST inference"};
    app.add_option("-n,--num-examples", numExamples, "How many examples to simulate");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    

    CLI11_PARSE(app, argc, argv);


    // Input spikes
    const auto inputSpikes = EventContainer::create(inputShape);

    // Hidden neurons
    const auto hiddenV = Variable::create(hiddenShape, GeNN::Type::S10_5);
    const auto hiddenI = Variable::create(hiddenShape, GeNN::Type::S10_5);
    const auto hiddenRefracTime = Variable::create(hiddenShape, GeNN::Type::Int16);
    const auto hiddenSpikes = EventContainer::create(hiddenShape);
    const auto hidden = NeuronUpdateProcess::create(
        "V = (Alpha * V) + I;\n"
        "I = 0.0h5;\n"
        "if (RefracTime > 0) {\n"
        "   RefracTime -= 1;\n"
        "}\n"
        "if(V >= VThresh && RefracTime <= 0) {\n"
        "   Spike();\n"
        "   V -= VThresh;\n"
        "   RefracTime = TauRefrac;\n"
        "}\n",
        {{"Alpha", Parameter::create(std::exp(-1.0 / 20.0), GeNN::Type::S10_5)},
         {"VThresh", Parameter::create(0.61, GeNN::Type::S10_5)},
         {"TauRefrac", Parameter::create(5, GeNN::Type::Int16)}},
        {{"V", hiddenV}, {"I", hiddenI}, {"RefracTime", hiddenRefracTime}},
        {{"Spike", hiddenSpikes}});
    
    // Output neurons
    const auto outputV = Variable::create(outputShape, GeNN::Type::S9_6);
    const auto outputI = Variable::create(outputShape, GeNN::Type::S9_6);
    const auto outputVSum = Variable::create(outputShape, GeNN::Type::S9_6);
    const auto outputBias = Variable::create(outputShape, GeNN::Type::S9_6);
    const auto output = NeuronUpdateProcess::create(
        "V = (Alpha * V) + I + Bias;\n"
        "I = 0.0h6;\n"
        "VSum += V;\n",
        {{"Alpha", Parameter::create(std::exp(-1.0 / 20.0), GeNN::Type::S9_6)}},
        {{"V", outputV}, {"VSum", outputVSum}, {"I", outputI}, {"Bias", outputBias}});
    
    // Input->Hidden event propagation
    const auto inputHiddenWeight = Variable::create(inputHiddenShape, GeNN::Type::S10_5);
    const auto inputHidden = EventPropagationProcess::create(inputSpikes, inputHiddenWeight, hiddenI);

    // Hidden->Output event propagation
    const auto hiddenOutputWeight = Variable::create(hiddenOutputShape, GeNN::Type::S9_6);
    const auto hiddenOutput = EventPropagationProcess::create(hiddenSpikes, hiddenOutputWeight, outputI);

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({hidden, output});
    const auto synapseUpdateProcesses = ProcessGroup::create({inputHidden, hiddenOutput});

    BackendFeNNSim backend;
    
    auto state = backend.createState();

    // Generate fields required for process groups
    //ProcessFields processFields;
    //addFields(synapseUpdateProcesses, bramAllocator, processFields);
    //addFields(neuronUpdateProcesses, bramAllocator, processFields);

    // 1) Visit process groups and build fields
    //MemoryAllocatorVisitor memoryAllocatorVisitor(memoryAllocator);
    //model.visitComponents(memoryAllocatorVisitor);

    // Generate kernel
    const auto code = backend.generateSimulationKernel(synapseUpdateProcesses, neuronUpdateProcesses, 
                                                       1000, true, state.get());

    for(uint32_t i: code) {
        try {
            disassemble(std::cout, i);
        }
        catch(const std::runtime_error&) {
            std::cout << "Unsupported";
        }
        std::cout << std::endl;
    }
    return 0;
}
