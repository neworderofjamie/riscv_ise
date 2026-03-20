#include "frontend/variable.h"

//----------------------------------------------------------------------------
// Frontend::Variable
//----------------------------------------------------------------------------
namespace Frontend
{
void Variable::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    CompilerFrontend::Type::updateHash(getType(), hash);
}
}
