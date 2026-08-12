#include "frontend/variable.h"

// Standard C++ includes
#include <memory>

// Frontend includes
#include "frontend/runtime.h"

//----------------------------------------------------------------------------
// Frontend::Variable
//----------------------------------------------------------------------------
namespace Frontend
{
void Variable::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    CompilerFrontend::Type::updateHash(getType(), hash);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> Variable::createArray(std::optional<size_t>, uint32_t, 
                                                 size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride Variable::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                 size_t, const Model&, const DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::getArrayShapeStride implementation");
}
}
