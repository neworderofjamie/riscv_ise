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
#include "frontend/kernel.h"
#include "frontend/process.h"
#include "frontend/process_group.h"
#include "frontend/runtime.h"
#include "frontend/shape.h"
#include "frontend/variable.h"

// Doc strings
#include "frontend_doc_strings.h"

using namespace pybind11::literals;

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
#define WRAP_ENUM(ENUM, VAL) .value(#VAL, ENUM::VAL, DOC(ENUM, VAL))
#define WRAP_METHOD(NAME, NS, CLASS, METH) .def(NAME, &NS::CLASS::METH, DOC(NS, CLASS, METH))
#define WRAP_PROPERTY_GETTER(NAME, NS, CLASS, METH_STEM) .def_property_readonly(NAME, &NS::CLASS::get##METH_STEM, DOC(NS, CLASS, get##METH_STEM))
#define WRAP_PROPERTY_RO(NAME, NS, CLASS, METH_STEM) .def_property_readonly(NAME, &NS::CLASS::get##METH_STEM, DOC(NS, CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_RO_SHOULD(NAME, NS, CLASS, METH_STEM) .def_property_readonly(NAME, &NS::CLASS::should##METH_STEM, DOC(NS, CLASS, m_##METH_STEM))

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
//----------------------------------------------------------------------------
// PyAppender
//----------------------------------------------------------------------------
// 'Trampoline' base class to wrap classes derived off Snippet::Base
class PyAppender : public plog::IAppender
{   
public: 
	virtual void write(const plog::Record& record) override { PYBIND11_OVERRIDE_PURE(void, plog::IAppender, write, record); }
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

    //------------------------------------------------------------------------
    // Free functions
    //------------------------------------------------------------------------
    m.def("init_logging", &Common::Logging::init);
    
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

        .def_property_readonly("value", &CompilerFrontend::Type::NumericValue::get);

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

    pybind11::implicitly_convertible<const std::string&, CompilerFrontend::Type::UnresolvedType>();
    pybind11::implicitly_convertible<const CompilerFrontend::Type::ResolvedType&, CompilerFrontend::Type::UnresolvedType>();

    //------------------------------------------------------------------------
    // frontend.Shape
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Shape>(m, "Shape")
         .def(pybind11::init<const std::vector<size_t>&>())
         .def(pybind11::init<size_t>())

         WRAP_PROPERTY_RO("dims", Frontend, Shape, Dims)
         WRAP_PROPERTY_GETTER("flattened_size", Frontend, Shape, FlattenedSize)
         
         .def("__repr__", &Frontend::Shape::toString);

    pybind11::implicitly_convertible<const std::vector<size_t>&, Frontend::Shape>();
    pybind11::implicitly_convertible<size_t, Frontend::Shape>();
	
	//------------------------------------------------------------------------
    // frontend.ModelComponent
    //------------------------------------------------------------------------
	pybind11::class_<Frontend::ModelComponent, std::shared_ptr<Frontend::ModelComponent>>(m, "ModelComponent")
		WRAP_PROPERTY_RO("name", Frontend, ModelComponent, Name);
	
	//------------------------------------------------------------------------
    // frontend.State
    //------------------------------------------------------------------------
	pybind11::class_<Frontend::State, Frontend::ModelComponent, std::shared_ptr<Frontend::State>>(m, "State")
		WRAP_PROPERTY_GETTER("shape", Frontend, State, Shape);
		
    //------------------------------------------------------------------------
    // frontend.Variable
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Variable, Frontend::State, std::shared_ptr<Frontend::Variable>>(m, "Variable")
        WRAP_PROPERTY_RO("type", Frontend, Variable, Type);
    
    //------------------------------------------------------------------------
    // frontend.Process
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Process, Frontend::ModelComponent, std::shared_ptr<Frontend::Process>>(m, "Process");
    
    //------------------------------------------------------------------------
    // fenn.NeuronUpdateProcess
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::NeuronUpdateProcess, Frontend::Process, std::shared_ptr<Frontend::NeuronUpdateProcess>>(m, "NeuronUpdateProcess")
        WRAP_PROPERTY_RO("variables", Frontend, NeuronUpdateProcess, Variables)
        WRAP_PROPERTY_RO("output_event_sinks", Frontend,NeuronUpdateProcess, OutputEventSinks)
        WRAP_PROPERTY_RO("shape", Frontend,NeuronUpdateProcess, Shape);

    //------------------------------------------------------------------------
    // fenn.EventPropagationProcess
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::EventPropagationProcess, Frontend::Process, std::shared_ptr<Frontend::EventPropagationProcess>>(m, "EventPropagationProcess")
        WRAP_PROPERTY_RO("input_event_source", Frontend, EventPropagationProcess, InputEventSource)
        WRAP_PROPERTY_RO("target", Frontend, EventPropagationProcess, Target);

    //------------------------------------------------------------------------
    // fenn.RNGInitProcess
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::RNGInitProcess, Frontend::Process, std::shared_ptr<Frontend::RNGInitProcess>>(m, "RNGInitProcess")
        WRAP_PROPERTY_RO("seed", Frontend, RNGInitProcess, Seed);

    //------------------------------------------------------------------------
    // frontend.MemsetProcess
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::MemsetProcess, Frontend::Process, std::shared_ptr<Frontend::MemsetProcess>>(m, "MemsetProcess")
        WRAP_PROPERTY_RO("target", Frontend, MemsetProcess, Target);

    //------------------------------------------------------------------------
    // frontend.ProcessGroup
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::ProcessGroup, Frontend::ModelComponent, std::shared_ptr<Frontend::ProcessGroup>>(m, "ProcessGroup")
        .def(pybind11::init(&Frontend::ProcessGroup::create),
             pybind11::arg("processes"), pybind11::arg("record_performance") = false,
             pybind11::arg("name") = "")

        WRAP_PROPERTY_RO("processes", Frontend, ProcessGroup, Processes)
        WRAP_PROPERTY_RO_SHOULD("record_performance", Frontend, ProcessGroup, RecordPerformance);

    //------------------------------------------------------------------------
    // frontend.Kernel
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Kernel, Frontend::ModelComponent, std::shared_ptr<Frontend::Kernel>>(m, "Kernel");

    //------------------------------------------------------------------------
    // frontend.SimpleKernel
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::SimpleKernel, Frontend::Kernel, std::shared_ptr<Frontend::SimpleKernel>>(m, "SimpleKernel")
        WRAP_PROPERTY_RO("process_groups", Frontend, SimpleKernel, ProcessGroups);

    //------------------------------------------------------------------------
    // frontend.SimulationLoopKernel
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::SimulationLoopKernel, Frontend::Kernel, std::shared_ptr<Frontend::SimulationLoopKernel>>(m, "SimulationLoopKernel")
        WRAP_PROPERTY_RO("num_timesteps", Frontend, SimulationLoopKernel, NumTimesteps)
        WRAP_PROPERTY_RO("timestep_process_groups", Frontend, SimulationLoopKernel, TimestepProcessGroups)
        WRAP_PROPERTY_RO("begin_process_groups", Frontend, SimulationLoopKernel, BeginProcessGroups)
        WRAP_PROPERTY_RO("end_process_groups", Frontend, SimulationLoopKernel, EndProcessGroups);
    
        //------------------------------------------------------------------------
    // frontend.ArrayBase
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::ArrayBase>(m, "ArrayBase")
        .def_property_readonly("host_view",
            [](Frontend::ArrayBase &a)
            {
               return pybind11::memoryview::from_memory(a.getHostPointer(),
                                                        a.getSizeBytes());
            })
        WRAP_PROPERTY_RO("type", Frontend, ArrayBase, Type)
		WRAP_PROPERTY_RO("shape", Frontend, ArrayBase, Shape)

        WRAP_METHOD("push_to_device", Frontend, ArrayBase, pushToDevice)
        WRAP_METHOD("pull_from_device", Frontend, ArrayBase, pullFromDevice);

    //------------------------------------------------------------------------
    // frontend.Runtime
    //------------------------------------------------------------------------
    pybind11::class_<Frontend::Runtime>(m, "Runtime")
        WRAP_METHOD("allocate", Frontend, Runtime, allocate)
        WRAP_METHOD("run", Frontend, Runtime, run)
        WRAP_METHOD("push_state_to_device", Frontend, Runtime, pushStateToDevice)
        WRAP_METHOD("pull_state_from_device", Frontend, Runtime, pullStateFromDevice)
		WRAP_METHOD("get_arrays", Frontend, Runtime, getArrays)
        WRAP_PROPERTY_RO("num_devices", Frontend, Runtime, NumDevices);    
}
