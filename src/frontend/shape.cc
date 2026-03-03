#include "frontend/shape.h"

// Standard C++ includes
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

// Standard C includes
#include <cassert>

// Common includes
#include "common/utils.h"

//----------------------------------------------------------------------------
// Frontend::Shape
//----------------------------------------------------------------------------
namespace Frontend
{
const Shape Shape::one = Shape(1);
const Shape Shape::zero = Shape({});
//----------------------------------------------------------------------------
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
bool Shape::isScalar() const
{
    return std::all_of(m_Dims.cbegin(), m_Dims.cend(), [](size_t i){ return i == 1; });
}
//----------------------------------------------------------------------------
size_t Shape::getFlattenedSize() const
{
    return std::accumulate(m_Dims.cbegin(), m_Dims.cend(), 1, std::multiplies<size_t>());
}
//----------------------------------------------------------------------------
std::vector<Shape> Shape::split(std::optional<size_t> splitDimension, size_t numSplits) const
{
    // If a split dimension is specified
    std::vector<Shape> shapeSplits;
    shapeSplits.reserve(numSplits);
    if(splitDimension.has_value()) {
        // Get size of dimension to split along
        const size_t originalSplitDimSize = m_Dims.at(splitDimension.value());
        assert(originalSplitDimSize > numSplits);

        // Calculate rounded-up split size. If split dimension cannot 
        // be EVENLY split, we want the last split to be just a little 
        // smaller rather than leaving a tiny split at the end
        const size_t ceilSplitSize = Common::Utils::ceilDivide(originalSplitDimSize,
                                                               numSplits);

        // Loop through splits, build shape and add to splits
        size_t remainingSplitDimSize = originalSplitDimSize;
        for(size_t i = 0; i < numSplits; i++) {
            Shape splitShape(*this);
            splitShape[splitDimension.value()] =((remainingSplitDimSize > ceilSplitSize) 
                                                  ? ceilSplitSize : remainingSplitDimSize);
            shapeSplits.push_back(splitShape);
        }
    }
    // Otherwise, populate shape splits with numSplits copies of shape
    else {
        std::fill_n(std::back_inserter(shapeSplits), numSplits, *this);
    }

    return shapeSplits;
}
//----------------------------------------------------------------------------
Shape Shape::padLast(size_t multiple) const
{
    Shape paddedShape(*this);

    // Pad last dimension to multiplies of 32
    paddedShape.getLast() = ::Common::Utils::padSize(paddedShape.getLast(), multiple);
    return paddedShape;
}
//----------------------------------------------------------------------------
Shape Shape::slice(size_t start, ptrdiff_t stop) const
{
    // Stop can be relative to start (positive) or to end (negative) so unify
    const size_t stopAbsolute = (stop > 0) ? static_cast<size_t>(stop) : (getNumDims() + stop);

    // Check start and stop are valid
    assert(start < getNumDims());
    assert(stopAbsolute < getNumDims());

    // If result slices away all dimensions, use size of 1
    if (start == stopAbsolute) {
        return Shape(1);
    }
    // Otherwise, assign slice of dimensions
    else {
        std::vector<size_t> slicedDims(getDims().cbegin() + start, 
                                       getDims().cbegin() + stopAbsolute);
        return Shape(slicedDims);
    }
}
}