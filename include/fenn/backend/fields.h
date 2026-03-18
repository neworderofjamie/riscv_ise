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
class Process;
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
    using FieldValue = std::variant<int32_t, uint32_t>;

    template<typename P = Frontend::Process>
    using GetFieldConstantFunc = std::function<FieldValue(size_t, std::shared_ptr<const P>)>;

    template<typename P = Frontend::Process>
    using GetFieldPointerFunc = std::function<Frontend::ArrayBase*(const Frontend::DeviceBase&, 
                                                                   std::shared_ptr<const P>)>;

    template<typename P = Frontend::Process>
    using GetFieldValueFunc = std::variant<GetFieldConstantFunc<P>, GetFieldPointerFunc<P>>;

    MergedFields() : m_NextFieldOffset(0)
    {}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    template<typename P>
    uint32_t addField(GetFieldConstantFunc<P> getFieldConstantFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_Fields.emplace_back(m_NextFieldOffset, 
                              [getFieldConstantFn]
                              (size_t d, std::shared_ptr<const Frontend::Process> p)
                              {
                                  return getFieldConstantFn(d, std::static_pointer_cast<const P>(p));
                              });

        // Update next field offset
        m_NextFieldOffset += fieldSize;

        // Return offset of new fiel,d
        return m_Fields.back().first;
    }

    template<typename P>
    uint32_t addField(GetFieldPointerFunc<P> getFieldPointerFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_Fields.emplace_back(m_NextFieldOffset, 
                              [getFieldPointerFn]
                              (const Frontend::DeviceBase &d, std::shared_ptr<const Frontend::Process> p)
                              {
                                  return getFieldPointerFn(d, std::static_pointer_cast<const P>(p));
                              });

        // Update next field offset
        m_NextFieldOffset += fieldSize;

        // Return offset of new fiel,d
        return m_Fields.back().first;
    }

    const auto &getFields() const{ return m_Fields; }
    uint32_t getSize() const{ return m_NextFieldOffset; }

private:
    

    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    uint32_t m_NextFieldOffset;
    std::vector<std::pair<uint32_t, GetFieldValueFunc<const Frontend::Process>>> m_Fields;
};
}

