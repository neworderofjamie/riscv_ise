#include "frontend/shape.h"

// Standard C++ includes
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>


//----------------------------------------------------------------------------
// Frontend::Shape
//----------------------------------------------------------------------------
namespace Frontend::Shape
{
std::string toString(const std::vector<size_t> &shape)
{
    std::ostringstream shapeStream;
    shapeStream << "(";
    std::copy(shape.cbegin(), shape.cend(),
              std::ostream_iterator<size_t>(shapeStream, ", "));
    shapeStream << ")";
    return shapeStream.str();
}
//----------------------------------------------------------------------------
bool isScalar(const std::vector<size_t> &shape)
{
 return std::all_of(shape.cbegin(), shape.cend(), [](size_t i){ return i == 1; });
}
//----------------------------------------------------------------------------
std::vector<size_t> getStride(const std::vector<size_t> &shape, size_t elementSize)
{
    // Reserve stride to match shape
    std::vector<size_t> strides;
    strides.reserve(shape.size());

    // Loop through axes
    size_t stride = elementSize;
    for(size_t i = shape.size(); i-- > 0;) {
        strides.push_back(stride);
        stride *= shape[i];
    }

    // Now reverse order of strides
    std::reverse(strides.begin(), strides.end());

    return strides;
}
}
