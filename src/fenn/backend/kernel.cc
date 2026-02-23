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

using namespace FeNN;

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
void SimpleKernel::generateCode(Assembler::CodeGenerator &c,
                                Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                GenerateProcessGroupFn generateProcessGroup) const
{
    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getProcessGroups())) {
        c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
    }

    // Visit process groups
    for (const auto &p : getProcessGroups()) {
        generateProcessGroup(p, c, scalarRegisterAllocator, vectorRegisterAllocator);
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopKernel
//----------------------------------------------------------------------------
void SimulationLoopKernel::generateCode(Assembler::CodeGenerator &c,
                                        Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                        Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                        GenerateProcessGroupFn generateProcessGroup) const
{
    // Register allocation
    ALLOCATE_SCALAR(STime);
    ALLOCATE_SCALAR(STimeEnd);

    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getTimestepProcessGroups()) 
       || arePerformanceCountersRequired(getBeginProcessGroups())
       || arePerformanceCountersRequired(getEndProcessGroups())) 
    {
        c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
    }

    // Set timestep range and load ready flag pointer
    c.li(*STime, 0);
    c.li(*STimeEnd, getNumTimesteps());

    // Visit begin process group
    for (const auto &p : getBeginProcessGroups()) {
        generateProcessGroup(p, c, scalarRegisterAllocator, vectorRegisterAllocator);
    }

    // Loop over time
    auto timeLoop = c.L();
    {
        // Visit timestep process group
        for (const auto &p : getTimestepProcessGroups()) {
            generateProcessGroup(p, c, scalarRegisterAllocator, vectorRegisterAllocator);
        }

        c.addi(*STime, *STime, 1);
        c.bne(*STime, *STimeEnd, timeLoop);
    }

    // Visit end process group
    for (const auto &p : getEndProcessGroups()) {
        generateProcessGroup(p, c, scalarRegisterAllocator, vectorRegisterAllocator);
    }
}

}
