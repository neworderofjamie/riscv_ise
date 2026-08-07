#pragma once

// Standard C++ includes
#include <optional>
#include <string>
#include <vector>

// Frontend includes
#include "frontend/frontend_export.h"

//----------------------------------------------------------------------------
// Frontend::Shape
//----------------------------------------------------------------------------
namespace Frontend::Shape
{
FRONTEND_EXPORT std::string toString(const std::vector<size_t> &shape);
FRONTEND_EXPORT size_t getFlattenedSize(const std::vector<size_t> &shape);
FRONTEND_EXPORT bool isScalar(const std::vector<size_t> &shape);
}
