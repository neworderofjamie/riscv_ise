#pragma once

// Standard C++ includes
#include <string>
#include <vector>

// Compiler includes
#include "model/model_export.h"

//----------------------------------------------------------------------------
// Shape
//----------------------------------------------------------------------------
class MODEL_EXPORT Shape
{
public:
    Shape(const std::vector<size_t> &dims) : m_Dims(dims)
    {}
    Shape(size_t dims) : m_Dims{{dims}}
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
    size_t getFlattenedSize() const;

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------
    bool operator == (const Shape &other) const{ return (other.getDims() == getDims()); }
    bool operator != (const Shape &other) const{ return (other.getDims() != getDims()); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Dims;
};