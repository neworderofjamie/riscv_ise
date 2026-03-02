#include "model/model.h"

// Model includes
#include "model/kernel.h"
#include "model/process.h"
#include "model/process_group.h"

//----------------------------------------------------------------------------
// Model::Model
//----------------------------------------------------------------------------
namespace Model
{
Model::Model(const KernelVector &kernels)
:   m_Kernels(kernels)
{
    // Loop through kernels
    for(const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // **TODO** performance counters
            // Loop through processes in group
            for (const auto &p : g->getProcesses()) {
                // Loop through all state associated with this process
                const auto state = p->getAllState();
                for (const auto &s : state) {
                    m_StateProcesses[s].push_back(p);
                }
            }

        }
    }

    // Loop through all model state
    // **THINK** this kinda only need to be variables
    for (const auto &s : getStateProcesses()) {
        // Start with all memory spaces being compatible
        uint32_t compatibleSplitDimensions = (1 << Ndims) - 1;

        // Loop through all processes using this state
        for (const auto &p : s.second) {
            p->updateCompatibleSplitDimensions(s.first, compatibleSplitDimensions);
        }

        // **TODO** pick best split (highest dimension as slicing here improves contiguousness) and store in state processes
    }
}
}