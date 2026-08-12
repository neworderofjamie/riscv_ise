#include "fenn/backend/utils.h"

// Standard C++ includes
#include <numeric>

// Standard C includes
#include <cassert>
#include <cmath>

//----------------------------------------------------------------------------
// FeNN::Backend::Utils
//----------------------------------------------------------------------------
namespace FeNN::Backend::Utils
{
size_t getSplitDimension(const std::vector<size_t> &shape, size_t split, size_t splitDimension, size_t numSplits, size_t splitGranularity)
{
    assert(split < numSplits);

    // Get size of dimension to split along
    const size_t originalSplitDimSize = shape.at(splitDimension);
    assert(originalSplitDimSize > numSplits);

    if(numSplits == 1) {
        return originalSplitDimSize;
    }
    else {
        // **NOTE** dimensions ABOVE split will only contain time/presynaptic neuron index not neuron ID

        // Multiply together size of dimensions 'below' split
        size_t numElementsPerSplitDim = 1;
        for (size_t i = splitDimension + 1; i < shape.size(); i++) {
            numElementsPerSplitDim *= shape[i];
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
}
}