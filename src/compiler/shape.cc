#include "compiler/shape.h"

// Standard C++ includes
#include <iterator>
#include <sstream>

//----------------------------------------------------------------------------
// Shape
//----------------------------------------------------------------------------
std::string Shape::toString() const
{
    std::ostringstream shapeStream;
    shapeStream << "(";
    std::copy(m_Dims.cbegin(), m_Dims.cend(),
              std::ostream_iterator<std::string>(shapeStream, ", "));
    shapeStream << ")";
    return shapeStream.str();
}
//----------------------------------------------------------------------------
size_t Shape::getNumNeurons() const
{
    if(m_Dims.size() == 1) {
        return m_Dims[0];
    }
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
    if(m_Dims.size() == 2) {
        return m_Dims[0];
    }
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
    if(m_Dims.size() == 2) {
        return m_Dims[2];
    }
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