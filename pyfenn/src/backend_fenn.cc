// Standard C++ includes
#include <optional>
#include <sstream>

// PyBind11 includes
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// FeNN disassembler includes
#include "fenn/disassembler/disassembler.h"


using namespace pybind11::literals;
using namespace FeNN;


//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
#define WRAP_ENUM(ENUM, VAL) .value(#VAL, ENUM::VAL, DOC(ENUM, VAL))
#define WRAP_METHOD(NAME, CLASS, METH) .def(NAME, &CLASS::METH, DOC(CLASS, METH))
#define WRAP_METHOD_REF(NAME, CLASS, METH) .def(NAME, &CLASS::METH, pybind11::return_value_policy::reference, DOC(CLASS, METH))
#define WRAP_PROPERTY_RO(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::get##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_GETTER(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::get##METH_STEM, DOC(CLASS, get##METH_STEM))
#define WRAP_PROPERTY_RO_REF(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::get##METH_STEM, pybind11::return_value_policy::reference, DOC(CLASS, m_##METH_STEM))*/

//----------------------------------------------------------------------------
// _backend_fenn
//----------------------------------------------------------------------------
PYBIND11_MODULE(_backend_fenn, m) 
{
	//------------------------------------------------------------------------
    // Free functions
    //------------------------------------------------------------------------
    m.def("disassemble", 
        [](uint32_t instruction) -> std::optional<std::string> 
        {
            try {
                std::ostringstream ss;
                Disassembler::disassemble(ss, instruction);
                return ss.str();
            }
            catch(const std::runtime_error&) {
                return std::nullopt;
            }
        });
    
}