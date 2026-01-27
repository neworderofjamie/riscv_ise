#include "model/shape.h"

// Standard C++ includes
#include <iterator>
#include <numeric>
#include <sstream>

//----------------------------------------------------------------------------
// Shape
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
size_t Shape::getNumNeurons() const
{
    // Non-batched neuron variable (N)
    if(m_Dims.size() == 1) {
        return m_Dims[0];
    }
    // Batched neuron variable (B, N)
    else if(m_Dims.size() == 2) {
        return m_Dims[1];
    }
    else {
        throw std::runtime_error("Unsupported neuron shape: " + toString());
    }
}
//----------------------------------------------------------------------------
size_t Shape::getNumSourceNeurons() const
{
    // Non-batched synapse variable (S, T)
    if(m_Dims.size() == 2) {
        return m_Dims[0];
    }
    // Batched synapse variable (B, S, T)
    else if(m_Dims.size() == 3) {
        return m_Dims[1];
    }
    else {
        throw std::runtime_error("Unsupported synapse shape: " + toString());
    }
}
//----------------------------------------------------------------------------
size_t Shape::getNumTargetNeurons() const
{
    // Non-batched synapse variable (S, T)
    if(m_Dims.size() == 2) {
        return m_Dims[1];
    }
    // Batched synapse variable (B, S, T)
    else if(m_Dims.size() == 3) {
        return m_Dims[2];
    }
    else {
        throw std::runtime_error("Unsupported synapse shape: " + toString());
    }
}
//----------------------------------------------------------------------------
size_t Shape::getBatchSize() const
{
    if(m_Dims.size() > 1) {
        return m_Dims[0];
    }
    else {
        throw std::runtime_error("Unbatched shape: " + toString());
    }
}
//----------------------------------------------------------------------------
size_t Shape::getFlattenedSize() const
{
    return std::accumulate(m_Dims.cbegin(), m_Dims.cend(), 1, std::multiplies<size_t>());
}