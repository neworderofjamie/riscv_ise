#pragma once

// Standard C++ includes
#include <vector>

// FeNN backend includes
#include "fenn/backend/backend_export.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Utils
//----------------------------------------------------------------------------
namespace FeNN::Backend::Utils
{
FENN_BACKEND_EXPORT size_t getSplitDimension(const std::vector<size_t> &shape, size_t split, size_t splitDimension,
                                             size_t numSplits, size_t splitGranularity);
}