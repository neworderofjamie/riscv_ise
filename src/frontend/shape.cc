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
size_t getFlattenedSize(const std::vector<size_t> &shape)
{
    // I am fairly certain that this is a bad code smell! anything that does this should be operating on stride
    assert(false);
    return std::accumulate(shape.cbegin(), shape.cend(), 1, std::multiplies<size_t>());
}
//----------------------------------------------------------------------------
bool isScalar(const std::vector<size_t> &shape)
{
 return std::all_of(shape.cbegin(), shape.cend(), [](size_t i){ return i == 1; });
}
}
