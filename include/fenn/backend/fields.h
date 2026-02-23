#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Backend includes
#include "backend/merged_model.h"

// Forward declarations
namespace Model
{
class State;
}

//----------------------------------------------------------------------------
// FeNN::Backend::KernelImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class MergedFields
{
public:
    MergedFields() : m_NextFieldOffset(0)
    {}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    template<typename P, typename F>
    uint32_t addPointerField(const ::Backend::MergedProcess &mergedProcess, 
                             F getStateFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_FieldOffsets.emplace_back(m_NextFieldOffset, 
                                    std::move(mergedProcess.gatherState<P>(getStateFn)));

        // Update next field offset
        m_NextFieldOffset += fieldSize;

        // Return offset of new fiel,d
        return m_FieldOffsets.back().first;
    }

    const auto &getFieldOffsets() const{ return m_FieldOffsets; }
    const uint32_t getSize() const{ return m_NextFieldOffset; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    uint32_t m_NextFieldOffset;
    std::vector<std::pair<uint32_t, std::vector<std::shared_ptr<const ::Model::State>>>> m_FieldOffsets;
};
}

