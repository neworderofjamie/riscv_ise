#include "backend/model.h"

// Plog includes
#include <plog/Log.h>

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

// Backend includes
#include "backend/backend_fenn.h"

namespace
{
//----------------------------------------------------------------------------
// Visitor
//----------------------------------------------------------------------------
class Visitor : public ModelComponentVisitor
{
public:
    Visitor(const std::vector<std::shared_ptr<const ProcessGroup>> processGroups, 
            Model::StatefulFields &statefulFields, Model::StateProcesses &stateProcesses,
            const Model::BackendFields &backendFields, uint32_t &fieldOffset)
    :   m_StatefulFields(statefulFields), m_StateProcesses(stateProcesses), 
        m_BackendFields(backendFields), m_FieldOffset(fieldOffset)
    {
        // Loop through all process groups and visit
        for(const auto &g : processGroups)  {
            g->accept(*this);
        }
    }

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const EventContainer> eventContainer)
    {
        // Allocate BRAM for field pointer
        LOGD << "\t\tEvent container '" << eventContainer->getName() << "'";
        if(m_CurrentProcessFields.try_emplace(eventContainer, m_FieldOffset).second) {
            m_FieldOffset += 4;
        }
        else {
            throw std::runtime_error("Event container '" + eventContainer->getName() + "' encountered multiple times in model traversal");
        }
    }

    virtual void visit(std::shared_ptr<const PerformanceCounter> performanceCounter)
    {
        // Allocate BRAM for field pointer
        LOGD << "\tPerformance counter '" << performanceCounter->getName() << "'";
        if(m_CurrentProcessGroupFields.try_emplace(performanceCounter, m_FieldOffset).second) {
            m_FieldOffset += 4;
        }
        else {
            throw std::runtime_error("Performance counter '" + performanceCounter->getName() + "' encountered multiple times in model traversal");
        }
    }

    virtual void visit(std::shared_ptr<const ProcessGroup> processGroup)
    {
        LOGD << "Process group '" << processGroup->getName() << "'";
        assert(m_CurrentProcessGroupFields.empty());

        // Visit performance counter
        if(processGroup->getPerformanceCounter()) {
            processGroup->getPerformanceCounter()->accept(*this);
            m_StateProcesses.get().try_emplace(processGroup->getPerformanceCounter());
        }

        // Visit all the processes
        for(const auto &p : processGroup->getProcesses()) {
            p->accept(*this);
        }

        // Add process group fields
        if(!m_StatefulFields.get().try_emplace(processGroup, m_CurrentProcessGroupFields).second) {
            throw std::runtime_error("Process group '" + processGroup->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current process group fields
        m_CurrentProcessGroupFields.clear();
    }

    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        LOGD << "\tNeuron update process '" << neuronUpdateProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());        
    
        // Visit variables
        for(auto &v : neuronUpdateProcess->getVariables()) {
            v.second->accept(*this);
            m_StateProcesses.get()[v.second].push_back(neuronUpdateProcess);
        }

        // Visit output event containers
        for(auto &e : neuronUpdateProcess->getOutputEvents()) {
            e.second->accept(*this);
            m_StateProcesses.get()[e.second].push_back(neuronUpdateProcess);
        }

        // Add process fields
        if(!m_StatefulFields.get().try_emplace(neuronUpdateProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("Neuron update process '" + neuronUpdateProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        LOGD << "\tEvent propagation process '" << eventPropagationProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());

        // Visit components
        eventPropagationProcess->getInputEvents()->accept(*this);
        eventPropagationProcess->getWeight()->accept(*this);
        eventPropagationProcess->getTarget()->accept(*this);
        
        // Add back-references in state processes
        m_StateProcesses.get()[eventPropagationProcess->getInputEvents()].push_back(eventPropagationProcess);
        m_StateProcesses.get()[eventPropagationProcess->getWeight()].push_back(eventPropagationProcess);
        m_StateProcesses.get()[eventPropagationProcess->getTarget()].push_back(eventPropagationProcess);

        // Add process fields
        if(!m_StatefulFields.get().try_emplace(eventPropagationProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const RNGInitProcess> rngInitProcess)
    {
        LOGD << "\tRNG init process '" << rngInitProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());

        // Visit components
        rngInitProcess->getSeed()->accept(*this);
        
        // Add back-references in state processes
        m_StateProcesses.get()[rngInitProcess->getSeed()].push_back(rngInitProcess);

        // Add process fields
        if(!m_StatefulFields.get().try_emplace(rngInitProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("RNG init process '" + rngInitProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const BroadcastProcess> broadcastProcess)
    {
        LOGD << "\tBroadcast process '" << broadcastProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());

        // Visit source and add back-references in state processes
        broadcastProcess->getSource()->accept(*this);
        m_StateProcesses.get()[broadcastProcess->getSource()].push_back(broadcastProcess);

         // If target is a variable reference
        if(std::holds_alternative<VariablePtr>(broadcastProcess->getTarget())) {
            auto target = std::get<VariablePtr>(broadcastProcess->getTarget());

            // Visit target and add back-references in state processes
            target->accept(*this);    
            m_StateProcesses.get()[target].push_back(broadcastProcess);
        }
        // Otherwise
        else {
            // Check state exists
            const int target = std::get<int>(broadcastProcess->getTarget());
            const auto backendField = m_BackendFields.get().find(target);
            if(backendField != m_BackendFields.get().cend()) {
                auto targetVar = std::static_pointer_cast<const Variable>(std::get<0>(backendField->second));
            
                // **YUCK** duplicate checks - these belong in BroadcastProcess
                if (targetVar->getNumBufferTimesteps() != 1) {
                    throw std::runtime_error("Broadcast process target has more than 1 buffer "
                                             "timestep which isn't currently supported");
                }

                if(targetVar->getShape().getDims().size() != 2) {
                    throw std::runtime_error("Broadcast process currently required 2 dimensional target");
                }

                if(targetVar->getShape().getDims().at(0) != broadcastProcess->getSource()->getShape().getDims().at(0)) {
                    throw std::runtime_error("Broadcast process requires first dimension of source and target to match");
                }

                if (targetVar->getType() != broadcastProcess->getSource()->getType()) {
                    throw std::runtime_error("Broadcast process requires source and target with same shape");
                }
            }
            else {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName() + "' targets non-existent backend state");
            }
        }
        
        
        // Add process fields
        if(!m_StatefulFields.get().try_emplace(broadcastProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("Broadcast process '" + broadcastProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        LOGD << "\tMemset process '" << memsetProcess->getName() << "'";
        
        // If target is a variable reference
        if(std::holds_alternative<VariablePtr>(memsetProcess->getTarget())) {
            auto target = std::get<VariablePtr>(memsetProcess->getTarget());
            assert(m_CurrentProcessFields.empty());

            // Visit 
            target->accept(*this);
            
            // Add back-references in state 
            m_StateProcesses.get()[target].push_back(memsetProcess);

            // Add process fields
            if(!m_StatefulFields.get().try_emplace(memsetProcess, m_CurrentProcessFields).second) {
                throw std::runtime_error("Memset process '" + memsetProcess->getName() + "' encountered multiple times in model traversal");
            }

            // Clear current state fields
            m_CurrentProcessFields.clear();
        }
        // Otherwise
        else {
            // Check state exists
            // **TODO** check buffer timesteps
            const int target = std::get<int>(memsetProcess->getTarget());
            if(m_BackendFields.get().find(target) == m_BackendFields.get().cend()) {
                throw std::runtime_error("Memset process '" + memsetProcess->getName() + "' targets non-existent backend state");
            }

            // **NOTE** no need for back-references - backend fields are global
        }
    }


    virtual void visit(std::shared_ptr<const Variable> variable)
    {
        // Allocate BRAM for field pointer
        LOGD << "\t\tVariable '" << variable->getName() << "'";
        if(m_CurrentProcessFields.try_emplace(variable, m_FieldOffset).second) {
            m_FieldOffset += 4;
        }
        else {
            throw std::runtime_error("Variable '" + variable->getName() + "' encountered multiple times in model traversal");
        }
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<Model::StatefulFields> m_StatefulFields;
    std::reference_wrapper<Model::StateProcesses> m_StateProcesses;
    std::reference_wrapper<const Model::BackendFields> m_BackendFields;
    std::reference_wrapper<uint32_t> m_FieldOffset;
    Model::StateFields m_CurrentProcessFields;
    Model::StateFields m_CurrentProcessGroupFields;
};
}

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
Model::Model(const std::vector<std::shared_ptr<const ProcessGroup>> &processGroups, 
             const BackendFeNN &backend)
:   m_ProcessGroups(processGroups)
{
    // Allocate fields for backend-specific state
    uint32_t fieldOffset = 4;
    const auto backendStateObjects = backend.getRequiredStateObjects(*this);
    std::transform(backendStateObjects.cbegin(), backendStateObjects.cend(),
                   std::inserter(m_BackendFields, m_BackendFields.end()),
                   [&fieldOffset](const auto &s)
                   {
                       const uint32_t oldFieldOffset = fieldOffset;
                       fieldOffset += 4;
                       return std::make_pair(s.first, std::make_tuple(s.second, oldFieldOffset));
                   });

    // Use visitor to populate process fields and 
    // state processes data structures from process groups
    Visitor visitor(m_ProcessGroups, m_StatefulFields, m_StateProcesses, 
                    m_BackendFields, fieldOffset);

    // Calculate total
    m_NumFields = (fieldOffset / 4);
}
