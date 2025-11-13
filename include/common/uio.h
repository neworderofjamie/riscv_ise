#pragma once

// Standard C++ includes
#include <string>

//----------------------------------------------------------------------------
// UIO
//----------------------------------------------------------------------------
class UIO
{
public:
    UIO(const std::string &uioName);
    ~UIO();
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    template<typename T>
    T *getData(){ return reinterpret_cast<T*>(m_Data); }
    
    template<typename T>
    const T *getData() const{ return reinterpret_cast<const T*>(m_Data); }
        
    size_t getSize() const{ return m_Size; }
    
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    void *m_Data;
    size_t m_Size;
};
