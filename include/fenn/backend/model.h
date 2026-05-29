#pragma once

// Frontend includes
#include "frontend/model.h"

// Forward declarations
namespace Frontend
{
class EventSink;
class GraphVector;
class ProcessGroup;
class State;
}

namespace boost::uuids::detail
{
class sha1;
}

//----------------------------------------------------------------------------
// FeNN::Backend::MemSpace
//----------------------------------------------------------------------------
//! Memory spaces supported by FeNN
/*! These are in DESCENDING order of preference */
namespace FeNN::Backend
{
enum class MemSpace : uint32_t
{
    DRAM        = (1 << 0),
    URAM        = (1 << 1),
    BRAM        = (1 << 2),
    LLM         = (1 << 3),
    URAM_LLM    = (1 << 4)
};


inline bool operator & (MemSpace a, MemSpace b)
{
    return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

inline MemSpace operator &= (MemSpace a, MemSpace b)
{
    return static_cast<MemSpace>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline MemSpace operator | (MemSpace a, MemSpace b)
{
    return static_cast<MemSpace>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
class Model : public Frontend::Model
{
public:
    using StateMemSpaceCompatibility = std::unordered_map<std::shared_ptr<const Frontend::State>, 
                                                          MemSpace>;

    Model(const KernelVector &kernels);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    MemSpace getStateMemSpace(std::shared_ptr<const Frontend::State> state, bool useDRAMForWeights) const;

    uint32_t getEventSinkIDBase(std::shared_ptr<const Frontend::EventSink> eventSink) const;
    
    auto getEventSourceProcessGroup() const{ return m_EventSourceProcessGroup; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    StateMemSpaceCompatibility m_StateCompatibleMemSpaces;

    std::shared_ptr<const Frontend::ProcessGroup> m_EventSourceProcessGroup;

    std::unordered_map<std::shared_ptr<const Frontend::EventSink>, uint32_t> m_EventSinkIDs;

    uint32_t m_NumNeuronIDBits;
    uint32_t m_NumPopulationIDBits;
};
}
