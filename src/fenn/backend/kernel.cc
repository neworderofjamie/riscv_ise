#include "fenn/backend/kernel.h"

// Common include
#include "common/utils.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN model include
#include "model/process_group.h"

// Assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/register_allocator.h"

using namespace FeNN::Assembler;
using namespace FeNN::Common;

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleGraph
//----------------------------------------------------------------------------
namespace
{
bool arePerformanceCountersRequired(const Model::ProcessGroupVector &processGroups)
{
    return std::any_of(processGroups.cbegin(), processGroups.cend(),
                       [](const auto &p){ return p->getPerformanceCounter(); });
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleKernel
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void SimpleKernel::generateCode(Assembler::CodeGenerator &codeGenerator,
                                Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    auto &c = codeGenerator;

    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getProcessGroups())) {
        c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);
    }

    // Visit process groups
    for (const auto &p : getProcessGroups()) {
        /*CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
                                     scalarRegisterAllocator, model,
                                     m_UseDRAMForWeights, m_KeepParamsInRegisters,
                                     m_NeuronUpdateRoundingMode);*/
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopKernel
//----------------------------------------------------------------------------
void SimulationLoopKernel::generateCode(Assembler::CodeGenerator &codeGenerator,
                                        Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    auto &c = codeGenerator;

    // Register allocation
    ALLOCATE_SCALAR(STime);
    ALLOCATE_SCALAR(STimeEnd);

    // Labels
    auto timeLoop = createLabel();

    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getTimestepProcessGroups()) 
       || arePerformanceCountersRequired(getBeginProcessGroups())
       || arePerformanceCountersRequired(getEndProcessGroups())) 
    {
        c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);
    }

    // Set timestep range and load ready flag pointer
    c.li(*STime, 0);
    c.li(*STimeEnd, getNumTimesteps());

    // Visit begin process group
    for (const auto &p : getBeginProcessGroups()) {

        //CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
        //                             scalarRegisterAllocator, model,
        //                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
        //                             m_NeuronUpdateRoundingMode);
    }

    // Loop over time
    c.L(timeLoop);
    {
        // Visit timestep process group
        for (const auto &p : getTimestepProcessGroups()) {
            //CodeGeneratorVisitor visitor(p, STime, numTimesteps, c, vectorRegisterAllocator,
            //                             scalarRegisterAllocator, model,
            //                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
            //                             m_NeuronUpdateRoundingMode);
        }

        c.addi(*STime, *STime, 1);
        c.bne(*STime, *STimeEnd, timeLoop);
    }

    // Visit end process group
    for (const auto &p : getEndProcessGroups()) {
        //CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
        //                             scalarRegisterAllocator, model,
        //                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
        //                             m_NeuronUpdateRoundingMode);
    }
}

}
