#pragma once

// Standard C++ includes
#include <optional>
#include <string>
#include <vector>

// Frontend includes
#include "frontend/frontend_export.h"

//----------------------------------------------------------------------------
// Frontend::Shape
//----------------------------------------------------------------------------
namespace Frontend
{
class FRONTEND_EXPORT Shape
{
public:
    Shape() = default;
    Shape(const Shape &shape) = default;

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

    bool isScalar() const;
    size_t getFlattenedSize() const;
    size_t getFirst() const{ return m_Dims.front(); }
    size_t &getFirst() { return m_Dims.front(); }
    size_t getLast() const{ return m_Dims.back(); }
    size_t &getLast(){ return m_Dims.back(); }

    //! Return the shape obtained by splitting numSplits ways along the specified dimensions
    std::vector<Shape> split(std::optional<size_t> splitDimension, size_t numSplits) const;

    //! Return shape with last dimension padded
    Shape padLast(size_t multiple) const;

    //! Return slice of shape dimensions
    Shape slice(size_t start, ptrdiff_t stop = 0) const;

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------
    bool operator == (const Shape &other) const{ return (other.getDims() == getDims()); }
    bool operator != (const Shape &other) const{ return (other.getDims() != getDims()); }

    size_t operator[](size_t i) const{ return m_Dims.at(i); }
    size_t &operator[](size_t i) { return m_Dims.at(i); }

    //------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------
    static const Shape one;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Dims;
};
}
