#pragma once

// Standard C++ includes
#include <functional>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

// Backend includes
#include "frontend/merged_model.h"

// Forward declarations
namespace Frontend
{
class ArrayBase;
class DeviceBase;
class State;
}

//----------------------------------------------------------------------------
// FeNN::Backend::KernelImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class MergedFields
{
    using FieldValue = std::variant<int32_t, uint32_t, Frontend::ArrayBase*>;

    template<typename P>
    using GetFieldValueFunc = std::function<FieldValue(const Frontend::DeviceBase &, 
                                                       std::shared_ptr<const P>)>;

public:
    MergedFields() : m_NextFieldOffset(0)
    {}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    template<typename P>
    uint32_t addField(GetFieldValueFunc<P> getFieldValueFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_FieldOffsets.emplace_back(m_NextFieldOffset, 
                                    [getFieldValueFn](const Frontend::DeviceBase &d, 
                                        std::shared_ptr<const Frontend::Process> p)
                                    {
                                        return getFieldValueFn(d, std::static_pointer_cast<const P>);
                                    });

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
    std::vector<std::pair<uint32_t, GetFieldValueFunc<const Frontend::Process>>> m_FieldOffsets;
};
}

