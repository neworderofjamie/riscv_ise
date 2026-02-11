#include "model/parameter.h"

// GeNN includes
#include "gennUtils.h"

using namespace GeNN::Utils;

//----------------------------------------------------------------------------
// Model::Parameter
//----------------------------------------------------------------------------
namespace Model
{
void Parameter::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    updateHash(getType(), hash);
}
}