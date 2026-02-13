#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Model includes
#include "model/model_component.h"
#include "model/shape.h"

//----------------------------------------------------------------------------
// Model::EventContainer
//----------------------------------------------------------------------------
namespace Model
{
class EventContainer : public AcceptableModelComponent<EventContainer, State>
{
public:
    EventContainer(Private, const Shape &shape, size_t numBufferTimesteps, const std::string &name)
    :   AcceptableModelComponent<EventContainer, State>(name), m_Shape(shape),
        m_NumBufferTimesteps(numBufferTimesteps)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }
    size_t getNumBufferTimesteps() const { return m_NumBufferTimesteps; }

    void updateMergeHash(boost::uuids::detail::sha1 &hash) const;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventContainer> create(const Shape &shape, size_t numBufferTimesteps = 1,
                                                  const std::string &name = "")
    {
        return std::make_shared<EventContainer>(Private(), shape, numBufferTimesteps, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
    size_t m_NumBufferTimesteps;
};
}   // namespace Model