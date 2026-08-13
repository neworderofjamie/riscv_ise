// Standard C++ includes
#include <optional>
#include <sstream>

// PyBind11 includes
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Frontend includes
#include "frontend/process_group.h"

// FeNN common includes
#include "fenn/common/logging.h"

// FeNN disassembler includes
#include "fenn/disassembler/disassembler.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/events.h"
#include "fenn/backend/kernel.h"
#include "fenn/backend/process.h"
#include "fenn/backend/runtime.h"
#include "fenn/backend/runtime_hw.h"
#include "fenn/backend/runtime_sim.h"
#include "fenn/backend/variable.h"

// Doc strings
#include "fenn_backend_doc_strings.h"

using namespace pybind11::literals;
using namespace FeNN;


//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
#define WRAP_ENUM(NS, ENUM, VAL) .value(#VAL, NS::ENUM::VAL, DOC(FeNN, NS, ENUM, VAL))
#define WRAP_METHOD(NAME, NS, CLASS, METH) .def(NAME, &NS::CLASS::METH, DOC(FeNN, NS, CLASS, METH))
#define WRAP_PROPERTY_GETTER(NAME, NS, CLASS, METH_STEM) .def_property_readonly(FeNN, NAME, &NS::CLASS::get##METH_STEM, DOC(NS, CLASS, get##METH_STEM))
#define WRAP_PROPERTY_RO(NAME, NS, CLASS, METH_STEM) .def_property_readonly(FeNN, NAME, &NS::CLASS::get##METH_STEM, DOC(NS, CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_RO_SHOULD(NAME, NS, CLASS, METH_STEM) .def_property_readonly(FeNN, NAME, &NS::CLASS::should##METH_STEM, DOC(NS, CLASS, m_##METH_STEM))

//----------------------------------------------------------------------------
// _fenn_backend
//----------------------------------------------------------------------------
PYBIND11_MODULE(_fenn_backend, m) 
{
    pybind11::module_::import("pyfenn._frontend");

    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------	
    pybind11::enum_<Compiler::RoundingMode>(m, "RoundingMode")
        WRAP_ENUM(Compiler, RoundingMode, TO_ZERO)
        WRAP_ENUM(Compiler, RoundingMode, NEAREST)
        WRAP_ENUM(Compiler, RoundingMode, STOCHASTIC);

    //------------------------------------------------------------------------
    // Free functions
    //------------------------------------------------------------------------
    m.def("init_logging", &FeNN::Common::Logging::init,
          pybind11::arg("common_level"), pybind11::arg("assembler_level"), 
          pybind11::arg("backend_level"), pybind11::arg("compiler_level"),
          pybind11::arg("ise_level"), pybind11::arg("common_appender"), 
          pybind11::arg("assembler_appender"), 
          pybind11::arg("backend_appender"), 
          pybind11::arg("compiler_appender"), pybind11::arg("ise_appender"));

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
    
    //------------------------------------------------------------------------
    // fenn_backend.EventSourceBuffer
    //------------------------------------------------------------------------
    pybind11::class_<Backend::EventSourceBuffer, Frontend::EventSourceBuffer, std::shared_ptr<Backend::EventSourceBuffer>>(m, "EventSourceBuffer", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::EventSourceBuffer::create),
             pybind11::arg("shape"), pybind11::arg("max_events"),
             pybind11::arg("name") = "");
        
    //------------------------------------------------------------------------
    // fenn_backend.EventSinkBuffer
    //------------------------------------------------------------------------
    pybind11::class_<Backend::EventSinkBuffer, Frontend::EventSinkBuffer, std::shared_ptr<Backend::EventSinkBuffer>>(m, "EventSinkBuffer", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::EventSinkBuffer::create),
             pybind11::arg("shape"), pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.EventChannel
    //------------------------------------------------------------------------
    pybind11::class_<Backend::EventChannel, Frontend::EventChannel, std::shared_ptr<Backend::EventChannel>>(m, "EventChannel")
        .def(pybind11::init(static_cast<std::shared_ptr<Backend::EventChannel>(*)(const std::vector<size_t>&, 
                                                                                  const std::vector<size_t>&, 
                                                                                  bool, const std::string&)>(&Backend::EventChannel::create)),
             pybind11::arg("sink_shape"), pybind11::arg("source_shape"),
             pybind11::arg("record") = false,
             pybind11::arg("name") = "");
        // **TODO** overload syntax
        //.def(pybind11::init(&Frontend::EventChannel::create<Backend::EventChannelSink, Backend::EventChannelSource>),
        //     pybind11::arg("shape"), pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.Variable
    //------------------------------------------------------------------------
    pybind11::class_<Backend::Variable, Frontend::Variable, std::shared_ptr<Backend::Variable>>(m, "Variable")
        .def(pybind11::init(&Backend::Variable::create),
             pybind11::arg("shape"), pybind11::arg("type"), pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.NeuronUpdateProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::NeuronUpdateProcess, Frontend::NeuronUpdateProcess, std::shared_ptr<Backend::NeuronUpdateProcess>>(m, "NeuronUpdateProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::NeuronUpdateProcess::create),
             pybind11::arg("code"), pybind11::arg("variables"), 
             pybind11::arg("output_event_sinks") /*= {}*/,
             pybind11::arg("default_scalar_literal_type") = CompilerFrontend::Type::S8_7,
             pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.DenseEventPropagationProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::DenseEventPropagationProcess, Frontend::EventPropagationProcess, std::shared_ptr<Backend::DenseEventPropagationProcess>>(m, "DenseEventPropagationProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::DenseEventPropagationProcess::create),
             pybind11::arg("input_event_source"), 
             pybind11::arg("weight"), pybind11::arg("target"),
             pybind11::arg("name") = "");
    
    //------------------------------------------------------------------------
    // fenn_backend.SparseEventPropagationProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::SparseEventPropagationProcess, Frontend::EventPropagationProcess, std::shared_ptr<Backend::SparseEventPropagationProcess>>(m, "SparseEventPropagationProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::SparseEventPropagationProcess::create),
             pybind11::arg("input_event_source"), 
             pybind11::arg("weight"), pybind11::arg("target"),
             pybind11::arg("num_sparse_connectivity_bits"),
             pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.RNGInitProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::RNGInitProcess, Frontend::RNGInitProcess, std::shared_ptr<Backend::RNGInitProcess>>(m, "RNGInitProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::RNGInitProcess::create),
             pybind11::arg("seed"), pybind11::arg("name") = "");
    
    //------------------------------------------------------------------------
    // fenn_backend.RNGInitProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::MemsetProcess, Frontend::MemsetProcess, std::shared_ptr<Backend::MemsetProcess>>(m, "MemsetProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::MemsetProcess::create),
             pybind11::arg("target"), pybind11::arg("name") = "");
    
    //------------------------------------------------------------------------
    // fenn_backend.BroadcastProcess
    //------------------------------------------------------------------------
    pybind11::class_<Backend::BroadcastProcess, Frontend::Process, std::shared_ptr<Backend::BroadcastProcess>>(m, "BroadcastProcess", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::BroadcastProcess::create),
             pybind11::arg("source"), pybind11::arg("target"), pybind11::arg("name") = "");
    
    //------------------------------------------------------------------------
    // fenn_backend.SimpleKernel
    //------------------------------------------------------------------------
    pybind11::class_<Backend::SimpleKernel, Frontend::SimpleKernel, std::shared_ptr<Backend::SimpleKernel>>(m, "SimpleKernel", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::SimpleKernel::create),
             pybind11::arg("process_groups"), pybind11::arg("name") = "");
    
    //------------------------------------------------------------------------
    // fenn_backend.SimpleKernel
    //------------------------------------------------------------------------
    pybind11::class_<Backend::SimulationLoopKernel, Frontend::SimulationLoopKernel, std::shared_ptr<Backend::SimulationLoopKernel>>(m, "SimulationLoopKernel", pybind11::multiple_inheritance())
        .def(pybind11::init(&Backend::SimulationLoopKernel::create),
             pybind11::arg("num_timesteps"), pybind11::arg("timestep_process_groups"), 
             pybind11::arg("begin_process_groups"), pybind11::arg("end_process_groups"), 
             pybind11::arg("name") = "");

    //------------------------------------------------------------------------
    // fenn_backend.Runtime
    //------------------------------------------------------------------------
    pybind11::class_<Backend::Runtime, Frontend::Runtime>(m, "Runtime")
        WRAP_METHOD("get_kernel_code", Backend, Runtime, getKernelCode);

    //------------------------------------------------------------------------
    // fenn_backend.RuntimeHW
    //------------------------------------------------------------------------
    pybind11::class_<Backend::RuntimeHW, Backend::Runtime>(m, "RuntimeHW")
        .def(pybind11::init<const std::vector<std::shared_ptr<const Frontend::Kernel>>&, 
             size_t, bool, bool, Compiler::RoundingMode, size_t>(),
             pybind11::arg("kernels"), pybind11::arg("num_devices"),
             pybind11::arg("use_dram_for_weights") = false, 
             pybind11::arg("keep_params_in_registers") = true,
             pybind11::arg("neuron_update_rounding_mode") = Compiler::RoundingMode::NEAREST,
             pybind11::arg("dma_buffer_size") = 512 * 1024);
    

    //------------------------------------------------------------------------
    // fenn_backend.RuntimeSim
    //------------------------------------------------------------------------
    pybind11::class_<Backend::RuntimeSim, Backend::Runtime>(m, "RuntimeSim")
        .def(pybind11::init<const std::vector<std::shared_ptr<const Frontend::Kernel>>&, 
             size_t, bool, bool, Compiler::RoundingMode, size_t>(),
             pybind11::arg("kernels"), pybind11::arg("num_devices"),
             pybind11::arg("use_dram_for_weights") = false, 
             pybind11::arg("keep_params_in_registers") = true,
             pybind11::arg("neuron_update_rounding_mode") = Compiler::RoundingMode::NEAREST,
             pybind11::arg("dma_buffer_size") = 512 * 1024);
}