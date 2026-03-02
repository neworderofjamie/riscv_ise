#include "frontend/model.h"

// Common includes
#include "common/utils.h"

// Model includes
#include "frontend/kernel.h"
#include "frontend/process.h"
#include "frontend/process_group.h"

//----------------------------------------------------------------------------
// Frontend::Model
//----------------------------------------------------------------------------
namespace Frontend
{
Model::Model(const KernelVector &kernels)
:   m_Kernels(kernels)
{
    // Loop through kernels
    for(const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Loop through processes in group
            for (const auto &p : g->getProcesses()) {
                // Loop through all state associated with this process
                for (const auto &s : p->getAllState()) {
                    m_StateProcesses[s].second.push_back(p);
                }
            }
        }
    }

    // Loop through all model state
    for (auto &s : m_StateProcesses) {
        // Get dimensionality of state's shape
        const size_t numDims = s.first->getShape().getNumDims();
        if(numDims > 32) {
            throw std::runtime_error("State '" + s.first->getName() + "' has a shape with more than 32 dimensions");
        }

        // Start with all memory spaces being compatible
        uint32_t compatibleSplitDimensions = (1 << numDims) - 1;

        // Loop through all processes using this state and update this compatibility
        for (const auto &p : s.second.second) {
            p->updateCompatibleSplitDimensions(s.first, compatibleSplitDimensions);
        }
        
        // If this cannot be split
        if(compatibleSplitDimensions == 0) {
            s.second.first = std::nullopt;
        }
        // Otherwise, count leading-zeros to pick highest compatible dimension
        // to split on (this maximises the number of contiguous sections of data)
        else {
            s.second.first = 31 - ::Common::Utils::clz(compatibleSplitDimensions);
        }
    }
}
}