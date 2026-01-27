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

// RISC-V common include
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/utils.h"

// RISC-V disassembler include
#include "disassembler/disassembler.h"

// RISC-V backend includes
#include "backend/backend_fenn_hw.h"
#include "backend/backend_fenn_sim.h"
#include "backend/model.h"
#include "backend/runtime.h"

// RISC-V compiler includes
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/performance_counter.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"


void recordSpikes(const std::string &filename, ArrayBase *spikeArray,
                  size_t numNeurons, size_t numTimesteps)
{
    spikeArray->pullFromDevice();
    const uint32_t *spikeRecording = spikeArray->getHostPointer<uint32_t>();

    const size_t numSpikeWords = ceilDivide(numNeurons, 32);
    std::ofstream spikeFile(filename);
    for(size_t t = 0; t < numTimesteps; t++) {
        AppUtils::writeSpikes(spikeFile, spikeRecording, t, numSpikeWords);
        spikeRecording += numSpikeWords;
    }
}

void loadAndPush(const std::string &filename, std::shared_ptr<const State> state, Runtime &runtime)
{
    // Load data from file
    const auto data = AppUtils::loadBinaryData<uint8_t>(filename);

    // Get array
    auto *array = runtime.getArray(state);
    assert(array->getSizeBytes() == data.size());

    // Copy data to array host pointer
    std::copy(data.cbegin(), data.cend(), array->getHostPointer());

    // Push to device
    array->pushToDevice();
}

void zeroAndPush(std::shared_ptr<const State> state, Runtime &runtime)
{
    // Get array
    auto *array = runtime.getArray(state);
 
    // Zero
    array->memsetHostPointer(0);

    // Push to device
    array->pushToDevice();
}

std::pair<uint64_t, uint64_t> readPerfCounter(std::shared_ptr<const PerformanceCounter> perfCounter, Runtime &runtime)
{
    // Get array
    auto *array = runtime.getArray(perfCounter);

    // Pull
    array->pullFromDevice();

    const uint64_t *hostData = array->getHostPointer<uint64_t>();
    return std::make_pair(hostData[0], hostData[1]);
}

int main(int argc, char** argv)
{
    constexpr size_t numTimesteps = 79;
    const Shape inputShape{{28 * 28}};
    const Shape hiddenShape{{128}};
    const Shape outputShape{{10}};
    const Shape inputHiddenShape{{28 * 28, 128}};
    const Shape hiddenOutputShape{{128, 10}};

    const size_t numInputSpikeWords = ceilDivide(inputShape.getNumNeurons(), 32);
    const size_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;

    bool device = false;
    bool shouldDisassemble = false;
    bool record = false;
    bool time = false;
    plog::Severity logSeverity = plog::info;
    size_t numExamples = 10000;

    CLI::App app{"Latency MNIST inference"};
    app.add_option("-n,--num-examples", numExamples, "How many examples to simulate");
    app.add_option_function<std::string>("-l,--log-level", 
                                         [&logSeverity](const std::string &l)
                                         {
                                             logSeverity = plog::severityFromString(l.c_str());
                                         });
    app.add_flag("-d,--device", device, "Whether model is run on device rather than simulator");
    app.add_flag("-a,--disassemble", shouldDisassemble, "Whether model disassembled code is printed");
    app.add_flag("-t,--time", time, "Whether performance counters are inserted");
    app.add_flag("-r,--record", record, "Whether spikes should be recorded?");

    CLI11_PARSE(app, argc, argv);

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(logSeverity, &consoleAppender);

    // Input spikes
    const auto inputSpikes = EventContainer::create(inputShape, numTimesteps);

    // Hidden neurons
    const auto hiddenV = Variable::create(hiddenShape, GeNN::Type::S10_5Sat);
    const auto hiddenI = Variable::create(hiddenShape, GeNN::Type::S10_5Sat);
    const auto hiddenRefracTime = Variable::create(hiddenShape, GeNN::Type::Int16);
    const auto hiddenSpikes = EventContainer::create(hiddenShape, record ? numTimesteps : 1);
    const auto hidden = NeuronUpdateProcess::create(
        "V = (Alpha * V) + I;\n"
        "I = 0.0h5;\n"
        "if (RefracTime > 0) {\n"
        "   RefracTime -= 1;\n"
        "}\n"
        "else if(V >= VThresh) {\n"
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
    const auto outputV = Variable::create(outputShape, GeNN::Type::S9_6Sat);
    const auto outputI = Variable::create(outputShape, GeNN::Type::S9_6Sat);
    const auto outputVAvg = Variable::create(outputShape, GeNN::Type::S9_6Sat, 1, "output v avg");
    const auto outputBias = Variable::create(outputShape, GeNN::Type::S9_6Sat);
    const auto output = NeuronUpdateProcess::create(
        "V = (Alpha * V) + I + Bias;\n"
        "I = 0.0h6;\n"
        "VAvg += (VAvgScale * V);\n",
        {{"Alpha", Parameter::create(std::exp(-1.0 / 20.0), GeNN::Type::S9_6)}, 
         {"VAvgScale", Parameter::create(1.0 / (numTimesteps / 2), GeNN::Type::S9_6)}},
        {{"V", outputV}, {"VAvg", outputVAvg}, {"I", outputI}, {"Bias", outputBias}});

    // Input->Hidden event propagation
    const auto inputHiddenWeight = Variable::create(inputHiddenShape, GeNN::Type::S10_5);
    const auto inputHidden = EventPropagationProcess::create(inputSpikes, inputHiddenWeight, hiddenI);

    // Hidden->Output event propagation
    const auto hiddenOutputWeight = Variable::create(hiddenOutputShape, GeNN::Type::S9_6);
    const auto hiddenOutput = EventPropagationProcess::create(hiddenSpikes, hiddenOutputWeight, outputI);

    // Output zero
    const auto zeroOutputSum = MemsetProcess::create(outputVAvg);

    // Performance counters
    const auto neuronUpdatePerfCounter = PerformanceCounter::create();
    const auto synapseUpdatePerfCounter = PerformanceCounter::create();
    const auto zeroPerfCounter = PerformanceCounter::create();

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({hidden, output}, time ? neuronUpdatePerfCounter : nullptr);
    const auto synapseUpdateProcesses = ProcessGroup::create({inputHidden, hiddenOutput}, time ? synapseUpdatePerfCounter : nullptr);
    const auto zeroProcesses = ProcessGroup::create({zeroOutputSum}, time ? zeroPerfCounter : nullptr);

    std::unique_ptr<BackendFeNN> backend;
    if (device) {
        backend = std::make_unique<BackendFeNNHW>();
    }
    else {
        backend = std::make_unique<BackendFeNNSim>();
    }

    // Build model from process groups we want to simulate
    Model model({synapseUpdateProcesses, neuronUpdateProcesses, zeroProcesses}, *backend);

    // Generate kernel
    const auto code = backend->generateSimulationKernel({synapseUpdateProcesses, neuronUpdateProcesses},
                                                        {zeroProcesses}, {}, numTimesteps, model);
    if(shouldDisassemble) {
        for(size_t i = 0; i < code.size(); i++){
            try {
                std::cout << i * 4 << ": ";
                disassemble(std::cout, code[i]);
            }
            catch(const std::runtime_error&) {
                std::cout << "Unsupported";
            }
            std::cout << std::endl;
        }
    }
    Runtime runtime(model, *backend);

    // Set instructions
    runtime.setInstructions(code);

    // Allocate memory for model
    runtime.allocate();

    // Load weights
    // **TODO** AppUtils
    loadAndPush("mnist_in_hid.bin", inputHiddenWeight, runtime);
    loadAndPush("mnist_hid_out.bin", hiddenOutputWeight, runtime);
    loadAndPush("mnist_bias.bin", outputBias, runtime);

    // Zero remaining state
    zeroAndPush(hiddenV, runtime);
    zeroAndPush(hiddenI, runtime);
    zeroAndPush(hiddenRefracTime, runtime);
    zeroAndPush(outputV, runtime);
    zeroAndPush(outputI, runtime);
    zeroAndPush(outputVAvg, runtime);

    if(time) {
        zeroAndPush(neuronUpdatePerfCounter, runtime);
        zeroAndPush(synapseUpdatePerfCounter, runtime);
        zeroAndPush(zeroPerfCounter, runtime);
    }

    // Load data
    const auto mnistSpikes = AppUtils::loadBinaryData<uint32_t>("mnist_spikes.bin");
    const auto mnistLabels = AppUtils::loadBinaryData<int16_t>("mnist_labels.bin");

    // Loop through examples
    auto *inputSpikeArray = runtime.getArray(inputSpikes);
    auto *hiddenSpikeArray = runtime.getArray(hiddenSpikes);
    auto *outputVAvgArray = runtime.getArray(outputVAvg);
    auto *outputVAvgHostPtr = outputVAvgArray->getHostPointer<int16_t>();
    size_t numCorrect = 0;
    for (size_t i = 0; i < numExamples; i++) {
        // Copy data to array host pointer
        std::copy_n(mnistSpikes.data() + (numInputSpikeArrayWords * i),
                    numInputSpikeArrayWords,
                    inputSpikeArray->getHostPointer<uint32_t>());
        inputSpikeArray->pushToDevice();

        // Classify
        runtime.run();

        // If we're recording, write input and hidden spikes to file
        if(record) {
            recordSpikes("mnist_input_spikes_" + std::to_string(i) + ".csv", inputSpikeArray,
                         inputShape.getNumNeurons(), numTimesteps);
            recordSpikes("mnist_hidden_spikes_" + std::to_string(i) + ".csv", hiddenSpikeArray,
                         hiddenShape.getNumNeurons(), numTimesteps);
        }

        // Copy copy of output V sum from device
        outputVAvgArray->pullFromDevice();

        // Determine if output is correct
        const auto classification = std::distance(outputVAvgHostPtr, std::max_element(outputVAvgHostPtr, outputVAvgHostPtr + 10));
        if (classification == mnistLabels[i]) {
            numCorrect++;
        }
    }

    std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / double(numExamples)) << "%)" << std::endl;

    // If timing is enabled
    if(time) {
        // Read performance counters
        auto [neuronUpdateCycles, neuronUpdateInstructions] = readPerfCounter(neuronUpdatePerfCounter, runtime);
        auto [synapseUpdateCycles, synapseUpdateInstructions] = readPerfCounter(synapseUpdatePerfCounter, runtime);
        auto [zeroCycles, zeroInstructions] = readPerfCounter(zeroPerfCounter, runtime);

        // Print
        std::cout << "Neuron update " << neuronUpdateCycles << " cycles, " << neuronUpdateInstructions << " instruction (" << (double)neuronUpdateInstructions / neuronUpdateCycles << ")" << std::endl;
        std::cout << "Synapse update " << synapseUpdateCycles << " cycles, " << synapseUpdateInstructions << " instruction (" << (double)synapseUpdateInstructions / synapseUpdateCycles << ")" << std::endl;
        std::cout << "Zero " << zeroCycles << " cycles, " << zeroInstructions << " instruction (" << (double)zeroInstructions / zeroCycles << ")" << std::endl;
    }
    //std::cout << duration.count() << " seconds" << std::endl;

    return 0;
}
