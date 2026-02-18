#include "model/variable.h"

// GeNN includes
#include "gennUtils.h"

using namespace GeNN::Utils;

//----------------------------------------------------------------------------
// Model::Variable
//----------------------------------------------------------------------------
namespace Model
{
void Variable::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    updateHash(getType(), hash);
}
}