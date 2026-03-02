#include "frontend/variable.h"

// GeNN includes
#include "gennUtils.h"

using namespace GeNN;

//----------------------------------------------------------------------------
// Frontend::Variable
//----------------------------------------------------------------------------
namespace Frontend
{
void Variable::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    Type::updateHash(getType(), hash);
}
}