#pragma once

// Standard C++ includes
#include <string>
#include <vector>

// Compiler includes
#include "model/model_export.h"

//----------------------------------------------------------------------------
// Model::Shape
//----------------------------------------------------------------------------
namespace Model
{
class MODEL_EXPORT Shape
{
public:
    Shape() = default;
    Shape(const std::vector<size_t> &dims) : m_Dims(dims)
    {}
    Shape(size_t dims) : m_Dims{{dims}}
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getDims() const{ return m_Dims; }
    size_t getNumDims() const{ return m_Dims.size(); }
    std::string toString() const;

    const bool isScalar() const;
    size_t getFlattenedSize() const;
    size_t getFirst() const{ return m_Dims.front(); }
    size_t getLast() const{ return m_Dims.back(); }

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------
    bool operator == (const Shape &other) const{ return (other.getDims() == getDims()); }
    bool operator != (const Shape &other) const{ return (other.getDims() != getDims()); }

    size_t operator[](size_t i) const{ return m_Dims.at(i); }
    size_t &operator[](size_t i) { return m_Dims.at(i); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Dims;
};
}