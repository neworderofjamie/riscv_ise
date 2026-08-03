// PyBind11 includes
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Plog includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// Common includes
#include "common/logging.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// Frontend includes
#include "frontend/events.h"
#include "frontend/process.h"
#include "frontend/process_group.h"
#include "frontend/runtime.h"
#include "frontend/shape.h"
#include "frontend/variable.h"

// Doc strings
//#include "docStrings.h"

using namespace pybind11::literals;

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
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
//----------------------------------------------------------------------------
// PyAppender
//----------------------------------------------------------------------------
// 'Trampoline' base class to wrap classes derived off Snippet::Base
class PyAppender : public plog::IAppender, public pybind11::trampoline_self_life_support
{   
public: 
	virtual void write(const Record& record) override { PYBIND11_OVERRIDE_PURE(void, plog::IAppender, write, record); }
};
}

//----------------------------------------------------------------------------
// _frontend
//----------------------------------------------------------------------------
PYBIND11_MODULE(_frontend, m) 
{
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    pybind11::enum_<plog::Severity>(m, "PlogSeverity")
        .value("NONE", plog::Severity::none)
        .value("FATAL", plog::Severity::fatal)
        .value("ERROR", plog::Severity::error)
        .value("WARNING", plog::Severity::warning)
        .value("INFO", plog::Severity::info)
        .value("DEBUG", plog::Severity::debug)
        .value("VERBOSE", plog::Severity::verbose);
	
    pybind11::enum_<RoundingMode>(m, "RoundingMode")
        WRAP_ENUM(RoundingMode, TO_ZERO)
        WRAP_ENUM(RoundingMode, NEAREST)
        WRAP_ENUM(RoundingMode, STOCHASTIC);

    //------------------------------------------------------------------------
    // Free functions
    //------------------------------------------------------------------------
    m.def("init_logging", &initLogging, pybind11::arg("level") = plog::Severity::info);
    
	//------------------------------------------------------------------------
	// frontend.Record
	//------------------------------------------------------------------------
    pybind11::class_<plog::Record>(m, "Record")
        .def_property_readonly("time", &plog::Record::getTime)
		.def_property_readonly("severity", &plog::Record::getSeverity)
		.def_property_readonly("tid", &plog::Record::getTid)
		.def_property_readonly("line", &plog::Record::getLine)
		.def_property_readonly("message", &plog::Record::getMessage)
		.def_property_readonly("func", &plog::Record::getFunc)
		.def_property_readonly("file", &plog::Record::getFile);

	//------------------------------------------------------------------------
    // frontend.IAppender
    //------------------------------------------------------------------------
    pybind11::class_<plog::IAppender, PyAppender>(m, "IAppender")
        .def("write", &plog::IAppender::write);

    //------------------------------------------------------------------------
    // frontend.NumericValue
    //------------------------------------------------------------------------
    pybind11::class_<CompilerFrontend::Type::NumericValue>(m, "NumericValue")
        .def(pybind11::init<double>())
        .def(pybind11::init<int64_t>())

        .def_property_readonly("value", &GeNN::Type::NumericValue::get);

    pybind11::implicitly_convertible<double, CompilerFrontend::Type::NumericValue>();
    pybind11::implicitly_convertible<int64_t, CompilerFrontend::Type::NumericValue>();

    //------------------------------------------------------------------------
    // frontend.ResolvedType
    //------------------------------------------------------------------------
    pybind11::class_<CompilerFrontend::Type::ResolvedType>(m, "ResolvedType");
        
    //------------------------------------------------------------------------
    // frontend.UnresolvedType
    //------------------------------------------------------------------------
    pybind11::class_<CompilerFrontend::Type::UnresolvedType>(m, "UnresolvedType")
        .def(pybind11::init<const std::string&>())
        .def(pybind11::init<const CompilerFrontend::Type::ResolvedType&>());

    pybind11::implicitly_convertible<const std::string&, GeNN::Type::UnresolvedType>();
    pybind11::implicitly_convertible<const CompilerFrontend::Type::ResolvedType&, CompilerFrontend::Type::UnresolvedType>();

    //------------------------------------------------------------------------
    // frontend.Shape
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Shape>(m, "Shape")
         .def(pybind11::init<const std::vector<size_t>&>())
         .def(pybind11::init<size_t>())

         WRAP_PROPERTY_RO("dims", Shape, Dims)
         WRAP_PROPERTY_GETTER("flattened_size", Shape, FlattenedSize)
         
         .def("__repr__", &Shape::toString);

    pybind11::implicitly_convertible<const std::vector<size_t>&, Shape>();
    pybind11::implicitly_convertible<size_t, Shape>();
	
	//------------------------------------------------------------------------
    // frontend.ModelComponent
    //------------------------------------------------------------------------
	pybind11::class_<ModelComponent, std::shared_ptr<ModelComponent>>(m, "ModelComponent")
		WRAP_PROPERTY_RO("name", ModelComponent, Name);
	
	//------------------------------------------------------------------------
    // frontend.State
    //------------------------------------------------------------------------
	pybind11::class_<State, ModelComponent, std::shared_ptr<State>>(m, "State")
		WRAP_PROPERTY_RO("shape", State, Shape);
		
    //------------------------------------------------------------------------
    // frontend.Variable
    //------------------------------------------------------------------------
    pybind11::class_<Variable, State, std::shared_ptr<Variable>>(m, "Variable")
        WRAP_PROPERTY_RO("type", Variable, Type);
    
    //------------------------------------------------------------------------
    // frontend.Process
    //------------------------------------------------------------------------
    pybind11::class_<Process, ModelComponent, std::shared_ptr<Process>>(m, "Process");
    
    //------------------------------------------------------------------------
    // fenn.NeuronUpdateProcess
    //------------------------------------------------------------------------
    pybind11::class_<NeuronUpdateProcess, Process, std::shared_ptr<NeuronUpdateProcess>>(m, "NeuronUpdateProcess")
        WRAP_PROPERTY_RO("variables", NeuronUpdateProcess, Variables)
        WRAP_PROPERTY_RO("output_event_sinks", NeuronUpdateProcess, OutputEventSinkls)
        WRAP_PROPERTY_RO("shape", NeuronUpdateProcess, Shape);

    //------------------------------------------------------------------------
    // fenn.EventPropagationProcess
    //------------------------------------------------------------------------
    pybind11::class_<EventPropagationProcess, Process, std::shared_ptr<EventPropagationProcess>>(m, "EventPropagationProcess")
        WRAP_PROPERTY_RO("input_event_source", EventPropagationProcess, InputEventSource)
        WRAP_PROPERTY_RO("target", EventPropagationProcess, Target);

    //------------------------------------------------------------------------
    // fenn.RNGInitProcess
    //------------------------------------------------------------------------
    pybind11::class_<RNGInitProcess, Process, std::shared_ptr<RNGInitProcess>>(m, "RNGInitProcess")
        WRAP_PROPERTY_RO("seed", RNGInitProcess, Seed);

    //------------------------------------------------------------------------
    // frontend.MemsetProcess
    //------------------------------------------------------------------------
    pybind11::class_<MemsetProcess, Process, std::shared_ptr<MemsetProcess>>(m, "MemsetProcess")
        WRAP_PROPERTY_RO("target", MemsetProcess, Target);

    //------------------------------------------------------------------------
    // frontend.ProcessGroup
    //------------------------------------------------------------------------
    pybind11::class_<ProcessGroup, Stateful, std::shared_ptr<ProcessGroup>>(m, "ProcessGroup")
        .def(pybind11::init(&ProcessGroup::create),
             pybind11::arg("processes"), pybind11::arg("record_performance") = false,
             pybind11::arg("name") = "")

        WRAP_PROPERTY_RO("processes", ProcessGroup, Processes)
        WRAP_PROPERTY_RO("performance_counter", ProcessGroup, shouldRecordPerformance);

    //------------------------------------------------------------------------
    // frontend.ArrayBase
    //------------------------------------------------------------------------
    pybind11::class_<ArrayBase>(m, "ArrayBase")
        .def_property_readonly("host_view",
            [](ArrayBase &a)
            {
               return pybind11::memoryview::from_memory(a.getHostPointer(),
                                                        a.getSizeBytes());
            })
        WRAP_PROPERTY_RO("type", ArrayBase, Type)
		WRAP_PROPERTY_RO("shape", ArrayBase, Shape)

        WRAP_METHOD("push_to_device", ArrayBase, pushToDevice)
        WRAP_METHOD("pull_from_device", ArrayBase, pullFromDevice);

    //------------------------------------------------------------------------
    // frontend.Runtime
    //------------------------------------------------------------------------
    pybind11::class_<Runtime>(m, "Runtime")
        WRAP_METHOD("allocate", Runtime, allocate)
        WRAP_METHOD("run", Runtime, run)
        WRAP_METHOD("push_state_to_device", Runtime, pushStateToDevice)
        WRAP_METHOD("pull_state_from_device", Runtime, pullStateFromDevice)
		WRAP_METHOD("get_arrays", Runtime, getArrays)
        WRAP_PROPERTY_RO("num_devices", Runtime, NumDevices);    
}
