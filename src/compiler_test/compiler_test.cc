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

// RISC-V assembler includes
#include "compiler/event_container.h"
#include "compiler/generate_process_group.h"
#include "compiler/model.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
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

    Model model;

    // Input spike source
    const auto *inputSpikes = model.addEventContainer(inputShape);
    const auto *input = model.addSpikeInputProcess(inputSpikes);

    // Hidden neurons
    const auto *hiddenV = model.addVariable(hiddenShape, GeNN::Type::S10_5);
    const auto *hiddenI = model.addVariable(hiddenShape, GeNN::Type::S10_5);
    const auto *hiddenRefracTime = model.addVariable(hiddenShape, GeNN::Type::Int16);
    const auto *hiddenSpikes = model.addEventContainer(hiddenShape);
    const auto *hidden = model.addNeuronUpdateProcess(
        "V = (Alpha * V) + I;\n"
        "I = 0.0h5;\n"
        "if (RefracTime > 0) {\n"
        "   RefracTime -= 1;\n"
        "}\n"
        "if(V >= VThresh && RefracTime <= 0) {\n"
        "   //Spike();\n"
        "   V -= VThresh;\n"
        "   RefracTime = TauRefrac;\n"
        "}\n",
        {{"Alpha", model.addParameter(std::exp(-1.0 / 20.0), GeNN::Type::S10_5)},
         {"VThresh", model.addParameter(0.61, GeNN::Type::S10_5)},
         {"TauRefrac", model.addParameter(5, GeNN::Type::Int16)}},
        {{"V", hiddenV}, {"I", hiddenI}, {"RefracTime", hiddenRefracTime}},
        {{"Spike", hiddenSpikes}});
    
    // Output neurons
    const auto *outputV = model.addVariable(outputShape, GeNN::Type::S9_6);
    const auto *outputI = model.addVariable(outputShape, GeNN::Type::S9_6);
    const auto *outputVSum = model.addVariable(outputShape, GeNN::Type::S9_6);
    const auto *outputBias = model.addVariable(outputShape, GeNN::Type::S9_6);
    const auto *output = model.addNeuronUpdateProcess(
        "V = (Alpha * V) + I + Bias;\n"
        "I = 0.0h6;\n"
        "VSum += V;\n",
        {{"Alpha", model.addParameter(std::exp(-1.0 / 20.0), GeNN::Type::S9_6)}},
        {{"V", outputV}, {"VSum", outputVSum}, {"I", outputI}, {"Bias", outputBias}});
    
    // Input->Hidden event propagation
    const auto *inputHiddenWeight = model.addVariable(inputHiddenShape, GeNN::Type::S10_5);
    const auto *inputHidden = model.addEventPropagationProcess(inputSpikes, inputHiddenWeight, hiddenI);

    // Hidden->Output event propagation
    const auto *hiddenOutputWeight = model.addVariable(hiddenOutputShape, GeNN::Type::S9_6);
    const auto *hiddenOutput = model.addEventPropagationProcess(hiddenSpikes, hiddenOutputWeight, outputI);

    // Group processes
    const auto *neuronUpdateProcesses = model.addProcessGroup({input, hidden, output});
    const auto *synapseUpdateProcesses = model.addProcessGroup({inputHidden, hiddenOutput});

    const auto code = generateSimulationKernel(synapseUpdateProcesses, neuronUpdateProcesses, 20, true);

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
