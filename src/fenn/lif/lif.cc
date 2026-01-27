// Standard C++ includes
#include <fstream>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

std::vector<uint32_t> generateCode(uint32_t numTimesteps, uint32_t inputCurrentVectorPtr, uint32_t inputCurrentScalarPtr,
                                   uint32_t voltageRecordingPtr, uint32_t spikeRecordingPtr, uint32_t readyFlagPtr,
                                   uint32_t startInstRetPtr, uint32_t endInstRetPtr, uint32_t startCyclePtr, uint32_t endCyclePtr, bool simulate)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Enable performance counters
            // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
            c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);
            
            // Generate code to copy vector of currents from scalar memory to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       inputCurrentScalarPtr, inputCurrentVectorPtr, 1u);

            // Register allocation
            ALLOCATE_SCALAR(SIBuffer);
            ALLOCATE_SCALAR(SVBuffer);
            ALLOCATE_SCALAR(SVBufferEnd);
            ALLOCATE_SCALAR(SSpikeBuffer);
            ALLOCATE_VECTOR(VAlpha);
            ALLOCATE_VECTOR(VV);
            ALLOCATE_VECTOR(VVReset);
            ALLOCATE_VECTOR(VVThresh);
            ALLOCATE_VECTOR(VI);

            // Labels
            auto loop = createLabel();

            // Write instructions retired and cycle counts to memory
            AssemblerUtils::generatePerformanceCountWrite(c, scalarRegisterAllocator,
                                                            CSR::MCYCLE, CSR::MCYCLEH, startCyclePtr);
            AssemblerUtils::generatePerformanceCountWrite(c, scalarRegisterAllocator,
                                                            CSR::MINSTRET, CSR::MINSTRETH, startInstRetPtr);

            // Load pointers
            c.li(*SIBuffer, inputCurrentVectorPtr);
            c.li(*SSpikeBuffer, spikeRecordingPtr);
            c.li(*SVBuffer, voltageRecordingPtr);
            c.li(*SVBufferEnd, voltageRecordingPtr + (numTimesteps * 64));

            // alpha = e^(-1/20)
            c.vlui(*VAlpha, 7792);
    
            // v = 0
            c.vlui(*VV, 0);
    
            // v_thresh = 1
            c.vlui(*VVThresh, 8192);

            // v_reset = 0
            c.vlui(*VVReset, 0);
    
            // i = vmem[0..32]
            c.vloadv(*VI, *SIBuffer);

            // Loop over time
            c.L(loop);
            {
                // Register allocation
                ALLOCATE_SCALAR(SSpike);

                // v *= alpha
                c.vmul(13, *VV, *VV, *VAlpha);
    
                // v += i
                c.vadd(*VV, *VV, *VI);
    
                // spike = VV >= VThres
                c.vtge(*SSpike, *VV, *VVThresh);
    
                // v = spk ? v_reset : v
                c.vsel(*VV, *SSpike, *VVReset);

                // Store spike vector to buffer
                c.sw(*SSpike, *SSpikeBuffer);
                c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);
        
                //vmem[a...a+32] = v
                c.vstore(*VV, *SVBuffer);
                c.addi(*SVBuffer, *SVBuffer, 64);
    
                // While x2 (address) < x1 (count), goto loop
                c.bne(*SVBuffer, *SVBufferEnd, loop);
            }

            // Write instructions retired and cycle counts to memory
            AssemblerUtils::generatePerformanceCountWrite(c, scalarRegisterAllocator,
                                                            CSR::MCYCLE, CSR::MCYCLEH, endCyclePtr);
            AssemblerUtils::generatePerformanceCountWrite(c, scalarRegisterAllocator,
                                                            CSR::MINSTRET, CSR::MINSTRETH, endInstRetPtr);
        });
}

void recordSpikes(const volatile uint32_t *spikeRecordingData, uint32_t numTimesteps)
{
    std::ofstream spikes("lif_spikes.csv");
    for(uint32_t t = 0; t < numTimesteps; t++) {
        AppUtils::writeSpikes(spikes, spikeRecordingData + t, t, 1);
        
    }
}

void recordV(const int16_t *vRecordingData, uint32_t numTimesteps)
{
    // Record spikes and voltages
    std::ofstream voltages("lif_voltages.csv");
    for(uint32_t t = 0; t < numTimesteps; t++) {
        for(uint32_t n = 0; n < 32; n++) {
            voltages << *vRecordingData++;
            if(n != (32 - 1)) {
                voltages << ", ";
            }
        }
        voltages << std::endl;
    }
}

int main(int argc, char** argv)
{
    uint32_t numTimesteps = 100;
    bool device = false;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    CLI::App app{"LIF neuron simulation"};
    app.add_option("-t,--timesteps", numTimesteps, "Number of timesteps to simulate");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector arrays
    const uint32_t inputCurrentVectorPtr = AppUtils::allocateVectorAndZero(32, vectorInitData);
    const uint32_t voltageRecordingPtr = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t inputCurrentScalarPtr = AppUtils::allocateScalarAndZero(2 * 32, scalarInitData);
    const uint32_t spikeRecordingPtr = AppUtils::allocateScalarAndZero(numTimesteps * 4, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t startInstRetPtr = AppUtils::allocateScalarAndZero(8, scalarInitData);
    const uint32_t endInstRetPtr = AppUtils::allocateScalarAndZero(8, scalarInitData);
    const uint32_t startCyclePtr = AppUtils::allocateScalarAndZero(8, scalarInitData);
    const uint32_t endCyclePtr = AppUtils::allocateScalarAndZero(8, scalarInitData);

    // Copy increasing input currents into scalar memory
    std::vector<int16_t> test{0, 26, 53, 79, 106, 132, 159, 185, 211, 238, 264, 291, 317, 344, 370, 396, 423, 449,
                              476, 502, 529, 555, 581, 608, 634, 661, 687, 713, 740, 766, 793, 819};
    std::memcpy(scalarInitData.data() + inputCurrentScalarPtr, test.data(), test.size() * 2);

    // Dump initial data to coe file
    std::vector<uint32_t> wordData(scalarInitData.size() / 4);
    std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
    //AppUtils::dumpCOE("lif_data.coe", wordData);

    // Generate code
    const auto code = generateCode(numTimesteps, inputCurrentVectorPtr, inputCurrentScalarPtr,
                                   voltageRecordingPtr, spikeRecordingPtr, 
                                   readyFlagPtr, startInstRetPtr, endInstRetPtr, startCyclePtr, endCyclePtr, 
                                   !device);

    // Dump to coe file
    //AppUtils::dumpCOE("lif.coe", code);

    if(device) {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);
        
        LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(code);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        device.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        device.setEnabled(false);
        LOGI << "Done";

        // Read cycle and instruction retired counters
        uint64_t startInstRet;
        uint64_t endInstRet;
        uint64_t startCycles;
        uint64_t endCycles;
        device.memcpyDataFromDevice(reinterpret_cast<uint8_t*>(&startInstRet), startInstRetPtr, 8);
        device.memcpyDataFromDevice(reinterpret_cast<uint8_t*>(&endInstRet), endInstRetPtr, 8);
        device.memcpyDataFromDevice(reinterpret_cast<uint8_t*>(&startCycles), startCyclePtr, 8);
        device.memcpyDataFromDevice(reinterpret_cast<uint8_t*>(&endCycles), endCyclePtr, 8);
        LOGI << (endInstRet - startInstRet) << " instructions retired in " << (endCycles - startCycles) << " cycles" << std::endl;

        // Record spikes
        recordSpikes(reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + spikeRecordingPtr), 
                     numTimesteps);
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);

        // Run!
        riscV.run();
        
        // Get pointers to scalar and vector memories
        const auto *scalarData = riscV.getScalarDataMemory().getData();
        const auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
        
        // Read performance counters
        const uint64_t *startInstRetData = reinterpret_cast<const uint64_t*>(scalarData + startInstRetPtr);
        const uint64_t *endInstRetData = reinterpret_cast<const uint64_t*>(scalarData + endInstRetPtr);
        const uint64_t *startCyclesData = reinterpret_cast<const uint64_t*>(scalarData + startCyclePtr);
        const uint64_t *endCyclesData = reinterpret_cast<const uint64_t*>(scalarData + endCyclePtr);
        LOGI << (endInstRetData[0] - startInstRetData[0]) << " instructions retired in " << (endCyclesData[0] - startCyclesData[0]) << " cycles" << std::endl;
        
        // Record spikes and voltages
        const uint32_t *spikeRecordingData = reinterpret_cast<const uint32_t*>(scalarData + spikeRecordingPtr);
        const int16_t *vRecordingData = vectorData + (voltageRecordingPtr / 2);
        recordSpikes(spikeRecordingData, numTimesteps);
        recordV(vRecordingData, numTimesteps);
    }
   
    

}
