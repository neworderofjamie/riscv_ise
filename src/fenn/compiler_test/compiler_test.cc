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

// Third party includes
#include <CLI11.hpp>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// Common include
#include "common/logging.h"
#include "common/utils.h"

// Frontend includes
#include "frontend/process_group.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/app_utils.h"
#include "fenn/common/logging.h"

// FeNN disassembler include
#include "fenn/disassembler/disassembler.h"

// FeNN backend includes
#include "fenn/backend/events.h"
#include "fenn/backend/kernel.h"
#include "fenn/backend/process.h"
#include "fenn/backend/runtime_hw.h"
#include "fenn/backend/runtime_sim.h"
#include "fenn/backend/variable.h"

using namespace CompilerFrontend;
using namespace FeNN;
using namespace Frontend;

void recordSpikes(const std::string &filename, ArrayBase *spikeArray,
                  size_t numNeurons, size_t numTimesteps)
{
    const uint32_t *spikeRecording = spikeArray->getHostPointer<uint32_t>();

    const size_t numSpikeWords = ::Common::Utils::ceilDivide(numNeurons, 32);
    std::ofstream spikeFile(filename);
    for(size_t t = 0; t < numTimesteps; t++) {
        FeNN::Common::AppUtils::writeSpikes(spikeFile, spikeRecording, t, numSpikeWords);
        spikeRecording += numSpikeWords;
    }
}

template<typename T>
void copyAndPush(const std::vector<T> &data, std::shared_ptr<const State> state, Runtime *runtime)
{
    // Get array
    for(auto *a : runtime->getArrays(state)) {
        assert(a->getSizeBytes() == (data.size() * sizeof(T)));

        // Copy data to array host pointer
        std::copy(data.cbegin(), data.cend(), a->getHostPointer<T>());
    }

    // Push to device
    runtime->pushStateToDevice(state);
}
void loadAndPush(const std::string &filename, std::shared_ptr<const State> state, Runtime *runtime)
{
    // Load data from file
    const auto data = FeNN::Common::AppUtils::loadBinaryData<uint8_t>(filename);

    // Get array
    for(auto *a : runtime->getArrays(state)) {
        assert(a->getSizeBytes() == data.size());

        // Copy data to array host pointer
        std::copy(data.cbegin(), data.cend(), a->getHostPointer());
    }

    // Push to device
    runtime->pushStateToDevice(state);
}

void zeroAndPush(std::shared_ptr<const State> state, Runtime *runtime)
{
    // Memset all arrays
    for(auto *a : runtime->getArrays(state)) {
        a->memsetHostPointer(0);
    }

    // Push to device
    runtime->pushStateToDevice(state);
}

/*std::pair<uint64_t, uint64_t> readPerfCounter(std::shared_ptr<const PerformanceCounter> perfCounter, Runtime &runtime)
{
    // Get array
    auto *array = runtime.getArray(perfCounter);

    // Pull
    array->pullFromDevice();

    const uint64_t *hostData = array->getHostPointer<uint64_t>();
    return std::make_pair(hostData[0], hostData[1]);
}*/

int main(int argc, char** argv)
{
    bool device = false;
    bool shouldDisassemble = true;
    // record = false;
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
    //app.add_flag("-r,--record", record, "Whether spikes should be recorded?");

    CLI11_PARSE(app, argc, argv);

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    ::Common::Logging::init(logSeverity, logSeverity, 
                            &consoleAppender, &consoleAppender);
    FeNN::Common::Logging::init(logSeverity, logSeverity, logSeverity, logSeverity, logSeverity,
                                &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender);

    constexpr size_t numTimesteps = 79;
    constexpr size_t maxSpikesPerExample = 301;
    const Shape inputShape{{28 * 28}};
    const Shape hiddenShape{{128}};
    const Shape hiddenShapeTime{{numTimesteps + 1, hiddenShape[0]}};
    const Shape outputShape{{10}};
    const Shape outputShapeTime{{numTimesteps + 1, outputShape[0]}};

    
    // Input spikes
    const auto inputSpikes = Backend::EventSourceBuffer::create(inputShape, maxSpikesPerExample, "inputSpikes");

    // Hidden neurons
    const auto hiddenV = Backend::Variable::create(hiddenShapeTime, Type::S10_5Sat, "hiddenV");
    const auto hiddenI = Backend::Variable::create(hiddenShape, Type::S10_5Sat, "hiddenI");
    const auto hiddenRefracTime = Backend::Variable::create(hiddenShape, Type::Int16, "hiddenRefracTime");
    const auto hiddenSpikes = Backend::EventChannel::create(hiddenShape, "hiddenSpikes");
    const auto hidden = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I;\n"
        "I = 0.0;\n"
        "if (RefracTime > 0) {\n"
        "   RefracTime -= 1;\n"
        "}\n"
        "else if(V >= 0.61) {\n"
        "   Spike();\n"
        "   V -= 0.61;\n"
        "   RefracTime = 5;\n"
        "}\n",
        {{"V", Sliced<Variable>(hiddenV, true)}, {"I", Sliced<Variable>(hiddenI)}, {"RefracTime", Sliced<Variable>(hiddenRefracTime)}},
        {{"Spike", Sliced<EventSink>(hiddenSpikes)}},
        Type::S10_5Sat, "hidden");

    // Output neurons
    const auto outputV = Backend::Variable::create(outputShape, Type::S9_6Sat, "outputV");
    const auto outputI = Backend::Variable::create(outputShape, Type::S9_6Sat, "outputI");
    const auto outputVAvg = Backend::Variable::create(outputShape, Type::S9_6Sat, "outputVAvg");
    const auto outputBias = Backend::Variable::create(outputShape, Type::S9_6Sat, "outputBias");
    const auto output = Backend::NeuronUpdateProcess::create(
        "V = (" + std::to_string(std::exp(-1.0 / 20.0)) + " * V) + I + Bias;\n"
        "I = 0.0h6;\n"
        "VAvg += (" + std::to_string(1.0 / (numTimesteps / 2)) + " * V);\n",
        {{"V", Sliced<Variable>(outputV)}, {"VAvg", Sliced<Variable>(outputVAvg)}, 
         {"I", Sliced<Variable>(outputI)}, {"Bias", Sliced<Variable>(outputBias)}},
        {}, Type::S9_6Sat, "output");


    // Connect input spikes to hidden
    const auto inputHiddenWeight = Backend::Variable::create(Frontend::Shape({inputShape[0], hiddenShape[0]}), Type::S10_5Sat, "inputHiddenWeight");
    const auto inputHidden = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(inputSpikes),
                                                                           inputHiddenWeight,
                                                                           Sliced<Variable>(hiddenI),
                                                                           "inputHidden");

    // Connect hidden spikes to output
    const auto hiddenOutputWeight = Backend::Variable::create(Frontend::Shape({hiddenShape[0], 32}), Type::S10_5Sat, "hiddenOutputWeight");
    const auto hiddenOutput = Backend::DenseEventPropagationProcess::create(Sliced<EventSource>(hiddenSpikes),
                                                                            hiddenOutputWeight,
                                                                            Sliced<Variable>(outputI),
                                                                            "hiddenOutput");

    // Output zero
    //const auto zeroOutputSum = MemsetProcess::create(outputVAvg);

    // Group processes
    const auto neuronUpdateProcesses = ProcessGroup::create({hidden, output}, time, "neuronUpdateProcesses");
    const auto synapseUpdateProcesses = ProcessGroup::create({inputHidden, hiddenOutput}, time, "synapseUpdateProcesses");
    //const auto zeroProcesses = ProcessGroup::create({zeroOutputSum}, time);

    const auto kernel = Backend::SimulationLoopKernel::create(numTimesteps, {synapseUpdateProcesses, neuronUpdateProcesses}/*,
                                                              {zeroProcesses}*/);
    
    std::vector<std::shared_ptr<const Frontend::Kernel>> kernels{kernel};
    std::unique_ptr<Backend::Runtime> runtime;
    if (device) {
        runtime = std::make_unique<Backend::RuntimeHW>(kernels, 1);
    }
    else {
        runtime = std::make_unique<Backend::RuntimeSim>(kernels, 1);
    }

   
    if(shouldDisassemble) {
        const auto &code = runtime->getKernelCode(kernel);
        for(size_t i = 0; i < code.size(); i++){
            try {
                std::cout << i * 4 << ": ";
                Disassembler::disassemble(std::cout, code[i]);
            }
            catch(const std::runtime_error&) {
                std::cout << "Unsupported";
            }
            std::cout << std::endl;
        }
    }
   

    // Allocate memory for model
    runtime->allocate();

    // Load weights
    // **TODO** AppUtils
    loadAndPush("mnist_in_hid.bin", inputHiddenWeight, runtime.get());
    loadAndPush("mnist_hid_out.bin", hiddenOutputWeight, runtime.get());
    loadAndPush("mnist_bias.bin", outputBias, runtime.get());

    // Zero remaining state
    zeroAndPush(hiddenV, runtime.get());
    zeroAndPush(hiddenI, runtime.get());
    zeroAndPush(hiddenRefracTime, runtime.get());
    zeroAndPush(outputV, runtime.get());
    zeroAndPush(outputV, runtime.get());
    zeroAndPush(outputVAvg, runtime.get());

    /*if(time) {
        zeroAndPush(neuronUpdatePerfCounter, runtime);
        zeroAndPush(synapseUpdatePerfCounter, runtime);
        zeroAndPush(zeroPerfCounter, runtime);
    }*/

    // Load data
    const auto mnistSpikes = FeNN::Common::AppUtils::loadBinaryData<uint16_t>("mnist_spikes.bin");
    const auto mnistLabels = FeNN::Common::AppUtils::loadBinaryData<int16_t>("mnist_labels.bin");

    // Loop through examples
    auto inputSpikeArrays = runtime->getArrays(inputSpikes);
    auto hiddenSpikeArrays = runtime->getArrays(hiddenSpikes);
    auto outputVAvgArrays = runtime->getArrays(outputVAvg);
    auto outputVAvgHostPtr = outputVAvgArrays[0]->getHostPointer<int16_t>();

    size_t numCorrect = 0;
    for (size_t i = 0; i < 1; i++) {
        // Copy data to array host pointer
        std::copy_n(mnistSpikes.data() + (maxSpikesPerExample * i),
                    maxSpikesPerExample,
                    inputSpikeArrays[0]->getHostPointer<uint16_t>());
        inputSpikeArrays[0]->pushToDevice();

        // Classify
        runtime->run(kernel);

        // If we're recording, write input and hidden spikes to file
        //if(record) {
        //    recordSpikes("mnist_input_spikes_" + std::to_string(i) + ".csv", inputSpikeArray,
        //                 inputShape.getNumNeurons(), numTimesteps);
        //    recordSpikes("mnist_hidden_spikes_" + std::to_string(i) + ".csv", hiddenSpikeArray,
        //                 hiddenShape.getNumNeurons(), numTimesteps);
        //}

        // Copy copy of output V sum from device
        outputVAvgArrays[0]->pullFromDevice();

        // Determine if output is correct
        const auto classification = std::distance(outputVAvgHostPtr, std::max_element(outputVAvgHostPtr, outputVAvgHostPtr + 10));
        if (classification == mnistLabels[i]) {
            numCorrect++;
        }
    }

    // Pull recorded spikes and voltages from device
    /*runtime->pullStateFromDevice(hiddenSpikes);
    runtime->pullStateFromDevice(hiddenSpikes);
    runtime->pullStateFromDevice(hidden1V);
    runtime->pullStateFromDevice(hidden2V);

    // Record spikes
    recordSpikes("compiler_test_hidden1_spikes.csv", runtime->getArrays(hidden1Spikes)[0], hidden1Shape.getFlattenedSize(), numTimesteps);
    recordSpikes("compiler_test_hidden2_spikes.csv", runtime->getArrays(hidden2Spikes)[0], hidden2Shape.getFlattenedSize(), numTimesteps);

    // Record voltages
    const int16_t *vRecordingData1 = runtime->getArrays(hidden1V)[0]->getHostPointer<int16_t>();
    const int16_t *vRecordingData2 = runtime->getArrays(hidden2V)[0]->getHostPointer<int16_t>();
    std::ofstream voltages("compiler_test_voltages.csv");
    for(uint32_t t = 0; t < numTimesteps; t++) {
        for(uint32_t n = 0; n < hidden1Shape[0]; n++) {
            voltages << *vRecordingData1++ << ", ";
        }
        for(uint32_t n = 0; n < 32; n++) {
            voltages << *vRecordingData2++;
            if(n != (32 - 1)) {
                voltages << ", ";
            }
        }
        voltages << std::endl;
    }*/
    std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / double(numExamples)) << "%)" << std::endl;

    // If timing is enabled
    //if(time) {
    //    // Read performance counters
    //    auto [neuronUpdateCycles, neuronUpdateInstructions] = readPerfCounter(neuronUpdatePerfCounter, runtime);
    //    auto [synapseUpdateCycles, synapseUpdateInstructions] = readPerfCounter(synapseUpdatePerfCounter, runtime);
    //    auto [zeroCycles, zeroInstructions] = readPerfCounter(zeroPerfCounter, runtime);

    //    // Print
    //    std::cout << "Neuron update " << neuronUpdateCycles << " cycles, " << neuronUpdateInstructions << " instruction (" << (double)neuronUpdateInstructions / neuronUpdateCycles << ")" << std::endl;
    //    std::cout << "Synapse update " << synapseUpdateCycles << " cycles, " << synapseUpdateInstructions << " instruction (" << (double)synapseUpdateInstructions / synapseUpdateCycles << ")" << std::endl;
    //    std::cout << "Zero " << zeroCycles << " cycles, " << zeroInstructions << " instruction (" << (double)zeroInstructions / zeroCycles << ")" << std::endl;
    //}
    //std::cout << duration.count() << " seconds" << std::endl;

    return 0;
}
