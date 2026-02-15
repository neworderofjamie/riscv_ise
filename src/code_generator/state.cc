#include "code_generator/state.h"

// Model includes
#include "model/graph.h"
#include "model/process.h"
#include "model/process_group.h"

//----------------------------------------------------------------------------
// CodeGenerator::State
//----------------------------------------------------------------------------
namespace CodeGenerator
{
State::State(const GraphVector &graphs)
:   m_Graphs(graphs)
{
    // Loop through graphs
    for(const auto &g : getGraphs()) {
        // Loop through all process groups in graph
        const auto processGroups = g->getAllProcessGroups();
        for (const auto &pg : processGroups) {
            // Loop through processes in group
            for (const auto &p : pg->getProcesses()) {
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