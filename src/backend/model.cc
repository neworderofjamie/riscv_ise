#include "backend/model.h"

// Plog includes
#include <plog/Log.h>

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

namespace
{
//----------------------------------------------------------------------------
// Visitor
//----------------------------------------------------------------------------
class Visitor : public ModelComponentVisitor
{
public:
    Visitor(const std::vector<std::shared_ptr<const ProcessGroup>> processGroups, 
            Model::ProcessFields &processFields, Model::ProcessGroupFields &processGroupFields,
            Model::StateProcesses &stateProcesses)
    :   m_ProcessFields(processFields), m_ProcessGroupFields(processGroupFields),
        m_StateProcesses(stateProcesses), m_FieldOffset(4)
    {
        // Loop through all process groups and visit
        for(const auto &g : processGroups)  {
            g->accept(*this);
        }
    }

    uint32_t getNumFields() const { return m_FieldOffset / 4; }

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
        if(!m_ProcessGroupFields.get().try_emplace(processGroup, m_CurrentProcessGroupFields).second) {
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
        if(!m_ProcessFields.get().try_emplace(neuronUpdateProcess, m_CurrentProcessFields).second) {
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
        if(!m_ProcessFields.get().try_emplace(eventPropagationProcess, m_CurrentProcessFields).second) {
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
        if(!m_ProcessFields.get().try_emplace(rngInitProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("RNG init process '" + rngInitProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const CopyProcess> copyProcess)
    {
        LOGD << "\tCopy process '" << copyProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());

        // Visit components
        copyProcess->getSource()->accept(*this);
        copyProcess->getTarget()->accept(*this);
        
        // Add back-references in state processes
        m_StateProcesses.get()[copyProcess->getSource()].push_back(copyProcess);
        m_StateProcesses.get()[copyProcess->getTarget()].push_back(copyProcess);

        // Add process fields
        if(!m_ProcessFields.get().try_emplace(copyProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("Copy process '" + copyProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        LOGD << "\tMemset process '" << memsetProcess->getName() << "'";
        assert(m_CurrentProcessFields.empty());

        // Visit 
        memsetProcess->getTarget()->accept(*this);
        
        // Add back-references in state 
        m_StateProcesses.get()[memsetProcess->getTarget()].push_back(memsetProcess);

        // Add process fields
        if(!m_ProcessFields.get().try_emplace(memsetProcess, m_CurrentProcessFields).second) {
            throw std::runtime_error("Memset process '" + memsetProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentProcessFields.clear();
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
    std::reference_wrapper<Model::ProcessFields> m_ProcessFields;
    std::reference_wrapper<Model::ProcessGroupFields> m_ProcessGroupFields;
    std::reference_wrapper<Model::StateProcesses> m_StateProcesses;
    Model::StateFields m_CurrentProcessFields;
    Model::StateFields m_CurrentProcessGroupFields;
    uint32_t m_FieldOffset;
};
}

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
Model::Model(const std::vector<std::shared_ptr<const ProcessGroup>> &processGroups)
:   m_ProcessGroups(processGroups)
{
    // Use visitor to populate process fields and 
    // state processes data structures from process groups
    Visitor visitor(m_ProcessGroups, m_ProcessFields, m_ProcessGroupFields, 
                    m_StateProcesses);

    m_NumFields = visitor.getNumFields();
}
