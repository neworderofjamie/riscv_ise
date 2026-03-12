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
                    m_StateData[s].processes.push_back(p);
                }
            }
        }
    }

    // Loop through all model state
    std::unordered_map<std::shared_ptr<const Frontend::State>, uint32_t> compatibleSplitDimensions;
    for (auto &s : m_StateData) {
        // Get dimensionality of state's shape
        const size_t numDims = s.first->getShape().getNumDims();
        if(numDims > 32) {
            throw std::runtime_error("State '" + s.first->getName() + "' has a shape with more than 32 dimensions");
        }

        // Start with all memory spaces being compatible
        uint32_t stateCompatibleSplitDimensions = (1 << numDims) - 1;

        // Loop through all processes using this state and update this compatibility
        for (const auto &p : s.second.processes) {
            p->updateCompatibleSplitDimensions(s.first, stateCompatibleSplitDimensions);
        }
        
        // Add to map
        compatibleSplitDimensions.try_emplace(s.first, stateCompatibleSplitDimensions);
    }

    // Loop through kernels
    for(const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Loop through processes in group and allow them to constrain split dimensions
            for (const auto &p : g->getProcesses()) {
                p->constrainSplitDimensions(compatibleSplitDimensions);
            }
        }
    }

    // Loop through all model state
    for (auto &s : m_StateData) {
        // If this state cannot be spli
        const uint32_t stateCompatibleSplitDimensions = compatibleSplitDimensions.at(s.first);
        if(stateCompatibleSplitDimensions == 0) {
            s.second.splitDimension = std::nullopt;
        }
        // Otherwise, count leading-zeros to pick highest compatible dimension
        // to split on (this maximises the number of contiguous sections of data)
        else {
            s.second.splitDimension = 31 - ::Common::Utils::clz(stateCompatibleSplitDimensions);
        }
    }
}
//----------------------------------------------------------------------------
const Model::StateData &Model::getStateData(std::shared_ptr<const Frontend::State> state) const
{
    return m_StateData.at(state);
}
}