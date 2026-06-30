#pragma once

// Standard C++ includes
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

// GeNN includes
#include "transpiler/token.h"

// Compiler includes
#include "compiler/compiler_export.h"
#include "compiler/model_component.h"

// Forward declarations
class EventContainer;
class Parameter;
class Variable;

using VariablePtr = std::shared_ptr<const Variable>;
using VariablePtrBackendState = std::variant<VariablePtr, int>;
using EventContainerMap = std::unordered_map<std::string, std::shared_ptr<EventContainer>>;
using ParameterMap = std::unordered_map<std::string, std::shared_ptr<const Parameter>>;
using VariableMap = std::unordered_map<std::string, VariablePtr>;


//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
class COMPILER_EXPORT NeuronUpdateProcess : public AcceptableModelComponent<NeuronUpdateProcess, Process>
{
public:
    NeuronUpdateProcess(Private, const std::string &code, const ParameterMap &parameters, 
                        const VariableMap &variables, const EventContainerMap &outputEvents,
                        const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getParameters() const{ return m_Parameters; }
    const auto &getVariables() const{ return m_Variables; }
    const auto &getOutputEvents() const{ return m_OutputEvents; }

    const auto &getTokens() const{ return m_Tokens; }

    size_t getNumNeurons() const{ return m_NumNeurons; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const ParameterMap &parameters, 
                                                       const VariableMap &variables, const EventContainerMap &outputEvents = {},
                                                       const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, parameters, variables, outputEvents, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ParameterMap m_Parameters;
    VariableMap m_Variables;
    EventContainerMap m_OutputEvents;

    std::vector<GeNN::Transpiler::Token> m_Tokens;

    size_t m_NumNeurons;
};



//----------------------------------------------------------------------------
// EventPropagationProcessBase
//----------------------------------------------------------------------------
class COMPILER_EXPORT EventPropagationProcessBase : public Process
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getInputEvents() const { return m_InputEvents; }

    size_t getNumSourceNeurons() const { return m_NumSourceNeurons; }

protected:
    EventPropagationProcessBase(std::shared_ptr<const EventContainer> inputEvents,
                                const std::string &name);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const EventContainer> m_InputEvents;

    size_t m_NumSourceNeurons;
};


//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
class COMPILER_EXPORT EventPropagationProcess : public AcceptableModelComponent<EventPropagationProcess, EventPropagationProcessBase>
{
public:
    EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                            VariablePtr weight, VariablePtr target,
                            size_t numSparseConnectivityBits, size_t numDelayBits,
                            const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getWeight() const{ return m_Weight; }
    const auto getTarget() const{ return m_Target; }

    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }
    size_t getMaxRowLength() const{ return m_MaxRowLength; }

    size_t getNumSparseConnectivityBits() const{ return m_NumSparseConnectivityBits; }
    size_t getNumDelayBits() const{ return m_NumDelayBits; }
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const EventContainer> inputEvents, 
                                                           VariablePtr weight, VariablePtr target, 
                                                           size_t numSparseConnectivityBits = 0,
                                                           size_t numDelayBits = 0,
                                                           const std::string &name = "")
    {
        return std::make_shared<EventPropagationProcess>(Private(), inputEvents, weight, target, 
                                                         numSparseConnectivityBits, numDelayBits, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtr m_Weight;
    VariablePtr m_Target;
    
    size_t m_NumTargetNeurons;
    size_t m_MaxRowLength;

    size_t m_NumSparseConnectivityBits;
    size_t m_NumDelayBits;
};


//----------------------------------------------------------------------------
// STDPEventPropagationProcess
//----------------------------------------------------------------------------
class COMPILER_EXPORT STDPEventPropagationProcess : public AcceptableModelComponent< STDPEventPropagationProcess, EventPropagationProcessBase>
{
public:
    STDPEventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                                VariablePtr x, VariablePtr preTimeSinceLastSpike,
                                VariablePtr target, VariablePtr vPre, VariablePtr cPre,
                                float thetaX, float a, float b, 
                                float alpha, float beta, 
                                float jPlus, float jMinus, 
                                float thetaV,float thetaLowUp, 
                                float thetaLowDown, float thetaHighUp, 
                                float thetaHighDown,
                                const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getX() const { return m_X; }
    const auto getTarget() const { return m_Target; }

    const auto getPreTimeSinceLastSpike() const{ return m_PreTimeSinceLastSpike; }

    const auto getVPre() const { return m_VPre; }
    const auto getCPre() const { return m_CPre; }


    float getThetaX() const {return m_ThetaX;}
    float getA() const {return m_A;}
    float getB() const {return m_B;}
    float getAlpha() const {return m_Alpha;}
    float getBeta() const {return m_Beta;}

    float getJPlus() const {return m_JPlus;}
    float getJMinus() const {return m_JMinus;}

    float getThetaV() const {return m_ThetaV;}
    float getThetaLowUp() const {return m_ThetaLowUp;}
    float getThetaLowDown() const {return m_ThetaLowDown;}
    float getThetaHighUp() const {return m_ThetaHighUp;}
    float getThetaHighDown() const {return m_ThetaHighDown;}

    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }
    size_t getMaxRowLength() const{ return m_MaxRowLength; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<STDPEventPropagationProcess> create(std::shared_ptr<const EventContainer> inputEvents,
                                                               VariablePtr x, VariablePtr preTimeSinceLastSpike, 
                                                               VariablePtr target, VariablePtr vPre, VariablePtr cPre,
                                                               float thetaX, float a, float b, 
                                                               float alpha, float beta, 
                                                               float jPlus, float jMinus, 
                                                               float thetaV,float thetaLowUp, 
                                                               float thetaLowDown, float thetaHighUp, 
                                                               float thetaHighDown,
                                                               const std::string &name)
    {
        return std::make_shared<STDPEventPropagationProcess>(Private(), inputEvents, 
                                                            x, preTimeSinceLastSpike, target, vPre, cPre,
                                                            thetaX, a, b, 
                                                            alpha, beta, 
                                                            jPlus, jMinus, 
                                                            thetaV,thetaLowUp, 
                                                            thetaLowDown, thetaHighUp, 
                                                            thetaHighDown,
                                                            name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtr m_X;
    VariablePtr m_Target;

    VariablePtr m_PreTimeSinceLastSpike;

    VariablePtr m_VPre;
    VariablePtr m_CPre;

    float m_ThetaX;
    float m_A;
    float m_B;
    float m_Alpha;
    float m_Beta;

    float m_JPlus;
    float m_JMinus;

    float m_ThetaV;
    float m_ThetaLowUp;
    float m_ThetaLowDown;
    float m_ThetaHighUp;
    float m_ThetaHighDown;


    size_t m_NumTargetNeurons;
    size_t m_MaxRowLength;
};




//----------------------------------------------------------------------------
// RNGInitProcess
//----------------------------------------------------------------------------
class COMPILER_EXPORT RNGInitProcess : public AcceptableModelComponent<RNGInitProcess, Process>
{
public:
    RNGInitProcess(Private, VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSeed() const{ return m_Seed; }
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<RNGInitProcess> create(VariablePtr seed, const std::string &name = "")
    {
        return std::make_shared<RNGInitProcess>(Private(), seed, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Variable> m_Seed;
};

//----------------------------------------------------------------------------
// MemsetProcess
//----------------------------------------------------------------------------
//! Process for memsetting variables
class COMPILER_EXPORT MemsetProcess : public AcceptableModelComponent<MemsetProcess, Process>
{
public:
    MemsetProcess(Private, VariablePtrBackendState target, const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getTarget() const{ return m_Target; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<MemsetProcess> create(VariablePtrBackendState target, const std::string &name = "")
    {
        return std::make_shared<MemsetProcess>(Private(), target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtrBackendState m_Target;
};

//----------------------------------------------------------------------------
// BroadcastProcess
//----------------------------------------------------------------------------
//! Process for 'broadcasting' one scalar array across multiple vector lanes
class COMPILER_EXPORT BroadcastProcess : public AcceptableModelComponent<BroadcastProcess, Process>
{
public:
    BroadcastProcess(Private, VariablePtr source, VariablePtrBackendState target, const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSource() const{ return m_Source; }
    const auto getTarget() const{ return m_Target; }
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<BroadcastProcess> create(VariablePtr source, VariablePtrBackendState target,
                                                    const std::string &name = "")
    {
        return std::make_shared<BroadcastProcess>(Private(), source, target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtr m_Source;
    VariablePtrBackendState m_Target;
};