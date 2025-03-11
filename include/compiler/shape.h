#pragma once

// Standard C++ includes
#include <string>
#include <vector>

//----------------------------------------------------------------------------
// Shape
//----------------------------------------------------------------------------
class Shape
{
public:
    Shape(const std::vector<size_t> &dims) : m_Dims(dims)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getDims() const{ return m_Dims; }
    std::string toString() const;

    size_t getNumNeurons() const;
    size_t getNumSourceNeurons() const;
    size_t getNumTargetNeurons() const;
    size_t getBatchSize() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Dims;
};