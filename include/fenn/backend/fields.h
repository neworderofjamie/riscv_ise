#pragma once

// Standard C++ includes
#include <functional>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

// Forward declarations
namespace Frontend
{
class ArrayBase;
class DeviceBase;
class ModelComponent;
class Process;
class State;
}

//----------------------------------------------------------------------------
// FeNN::Backend::MergedFields
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class MergedFields
{
public:
    using FieldValue = std::variant<int32_t, uint32_t>;

    template<typename M = Frontend::ModelComponent>
    using GetFieldConstantFunc = std::function<FieldValue(size_t, std::shared_ptr<const M>)>;

    template<typename M = Frontend::ModelComponent>
    using GetFieldPointerFunc = std::function<Frontend::ArrayBase*(const Frontend::DeviceBase&, 
                                                                   std::shared_ptr<const M>)>;

    template<typename M = Frontend::ModelComponent>
    using GetFieldValueFunc = std::variant<GetFieldConstantFunc<M>, GetFieldPointerFunc<M>>;

    MergedFields() : m_NextFieldOffset(0)
    {}

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    template<typename M>
    uint32_t addField(GetFieldConstantFunc<M> getFieldConstantFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_Fields.emplace_back(m_NextFieldOffset, 
                              [getFieldConstantFn]
                              (size_t d, std::shared_ptr<const Frontend::ModelComponent> m)
                              {
                                  return getFieldConstantFn(d, std::dynamic_pointer_cast<const M>(m));
                              });

        // Update next field offset
        m_NextFieldOffset += fieldSize;

        // Return offset of new fiel,d
        return m_Fields.back().first;
    }

    template<typename M>
    uint32_t addField(GetFieldPointerFunc<M> getFieldPointerFn, uint32_t fieldSize = 4)
    {
        // Gather state from all merged processes and assign to field
        m_Fields.emplace_back(m_NextFieldOffset, 
                              [getFieldPointerFn]
                              (const Frontend::DeviceBase &d, std::shared_ptr<const Frontend::ModelComponent> m)
                              {
                                  return getFieldPointerFn(d, std::dynamic_pointer_cast<const M>(m));
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
    std::vector<std::pair<uint32_t, GetFieldValueFunc<Frontend::ModelComponent>>> m_Fields;
};
}

