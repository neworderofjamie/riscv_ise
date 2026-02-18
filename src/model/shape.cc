#include "model/shape.h"

// Standard C++ includes
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

//----------------------------------------------------------------------------
// Model::Shape
//----------------------------------------------------------------------------
namespace Model
{
std::string Shape::toString() const
{
    std::ostringstream shapeStream;
    shapeStream << "(";
    std::copy(m_Dims.cbegin(), m_Dims.cend(),
              std::ostream_iterator<size_t>(shapeStream, ", "));
    shapeStream << ")";
    return shapeStream.str();
}
//----------------------------------------------------------------------------
const bool Shape::isScalar() const
{
    return std::all_of(m_Dims.cbegin(), m_Dims.cend(), [](size_t i){ return i == 1; });
}
//----------------------------------------------------------------------------
size_t Shape::getFlattenedSize() const
{
    return std::accumulate(m_Dims.cbegin(), m_Dims.cend(), 1, std::multiplies<size_t>());
}
}