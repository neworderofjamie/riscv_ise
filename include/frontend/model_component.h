#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <string>

// Frontend includes
#include "frontend/frontend_export.h"

// Forward declarations
namespace Frontend
{
class ArrayBase;
class DeviceBase;
class Model;
}

namespace boost::uuids::detail
{
class sha1;
}

// Macros
#define UPDATE_HASH_CLASS_NAME(CLASS_NAME) ::Common::Utils::updateHash(#CLASS_NAME, hash);

//----------------------------------------------------------------------------
// Frontend::ModelComponent
//----------------------------------------------------------------------------
namespace Frontend
{
//! Base class for things that can be added to models
class FRONTEND_EXPORT ModelComponent : public std::enable_shared_from_this<ModelComponent>
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const std::string &getName() const{ return m_Name; }

protected:
    struct Private{ explicit Private() = default; };

    ModelComponent(const std::string &name) : m_Name(name)
    {}

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::string m_Name;
};

//----------------------------------------------------------------------------
// Frontend::State
//----------------------------------------------------------------------------
class FRONTEND_EXPORT State : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const = 0;
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const = 0;

    virtual std::unique_ptr<ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                   size_t numDevices, const Model &model, DeviceBase &device) const = 0;

    virtual std::tuple<std::vector<size_t>, std::vector<size_t>> getArrayShapeStride(std::optional<size_t> splitDimension,
                                                                                     uint32_t indexDimensions, size_t numDevices, 
                                                                                     const Model &model, const DeviceBase &device) const = 0;
protected:
    using ModelComponent::ModelComponent;
};
}
