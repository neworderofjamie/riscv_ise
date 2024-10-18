// Standard C++ includes
#include <fstream>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
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
                                   bool simulate)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
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
            Label loop;

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

int main()
{
    constexpr uint32_t numTimesteps = 100;
    constexpr bool simulate = true;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
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
                                   voltageRecordingPtr, spikeRecordingPtr, readyFlagPtr, simulate);

    // Dump to coe file
    //AppUtils::dumpCOE("lif.coe", code);

    if(simulate) {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV(code, scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
        
        // Run!
        riscV.run();
        
        // Get pointers to scalar and vector memories
        const auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();
        
        const uint32_t *spikeRecordingData = reinterpret_cast<const uint32_t*>(scalarData + spikeRecordingPtr);
        const int16_t *vRecordingData = vectorData + (voltageRecordingPtr / 2);

        // Record spikes and voltages
        recordSpikes(spikeRecordingData, numTimesteps);
        recordV(vRecordingData, numTimesteps);
    }
    else {
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
        LOGI << "Done";
        // Record spikes
        recordSpikes(reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + spikeRecordingPtr), 
                     numTimesteps);
    }
   
    

}
