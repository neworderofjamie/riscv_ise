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
std::unique_ptr<ArrayBase> Variable::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::createArray implementation");
}
}
