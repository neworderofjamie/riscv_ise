#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// GeNN includes
#include "type.h"

// Model includes
#include "model/model_component.h"
#include "model/model_export.h"
#include "model/shape.h"

//----------------------------------------------------------------------------
// Model::Variable
//----------------------------------------------------------------------------
namespace Model
{
class MODEL_EXPORT Variable : public AcceptableState<Variable>
{
public:
    Variable(Private, const Shape &shape, const GeNN::Type::UnresolvedType &type, 
             size_t numBufferTimesteps, const std::string &name)
    :   AcceptableState<Variable>(name), m_Shape(shape), 
        m_Type(type.resolve({})), m_NumBufferTimesteps(numBufferTimesteps)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getShape() const{ return m_Shape; }
    const auto &getType() const{ return m_Type; }
    size_t getNumBufferTimesteps() const { return m_NumBufferTimesteps; }

    void updateMergeHash(boost::uuids::detail::sha1 &hash) const;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const Shape &shape, const GeNN::Type::UnresolvedType &type, 
                                            size_t numBufferTimesteps = 1, const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, numBufferTimesteps, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
    GeNN::Type::ResolvedType m_Type;
    size_t m_NumBufferTimesteps;
};
}