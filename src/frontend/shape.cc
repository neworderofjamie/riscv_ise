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
size_t Shape::getSplitDimension(size_t split, size_t splitDimension, size_t numSplits, size_t splitGranularity) const
{
    // Get size of dimension to split along
    const size_t originalSplitDimSize = m_Dims.at(splitDimension);
    assert(originalSplitDimSize > numSplits);

    // **NOTE** dimensions ABOVE split will only contain time/presynaptic neuron index not neuron ID

    // Multiply together size of dimensions 'below' split
    size_t numElementsPerSplitDim = 1;
    for (size_t i = splitDimension + 1; i < getNumDims(); i++) {
        numElementsPerSplitDim *= m_Dims[i];
    }

    // Multiply this by size of split dimension to get total
    const size_t numElementsToSplit = numElementsPerSplitDim * originalSplitDimSize;

    // Lowest Common Multiple of this and 32 is our split granularity
    // as we need splits to be multiples of 32 and we don't want to break dimensions
    const size_t finalSplitGranularity = std::lcm(numElementsPerSplitDim, splitGranularity);

    // Determine size of splits (in terms of these granules)
    const size_t roundedSplitGranules = static_cast<size_t>(std::round(numElementsToSplit / (static_cast<double>(numSplits) * finalSplitGranularity)));

    // Convert into actual sizes
    const size_t roundedSplitSize = (roundedSplitGranules * finalSplitGranularity) / numElementsPerSplitDim;

    return (split < (numSplits - 1)) ? roundedSplitSize : (originalSplitDimSize - roundedSplitSize);
}
//----------------------------------------------------------------------------
Shape Shape::getSplit(size_t split, std::optional<size_t> splitDimension, size_t numSplits, size_t splitGranularity) const
{
    if(splitDimension.has_value()) {
        Shape splitShape = *this;
        splitShape[splitDimension.value()] = getSplitDimension(split, splitDimension.value(), 
                                                               numSplits, splitGranularity);
        return splitShape;
    }
    // Otherwise, return copy
    else {
        return *this;
    }
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
    assert(stopAbsolute <= getNumDims());

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