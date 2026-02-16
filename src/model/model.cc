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
}
}