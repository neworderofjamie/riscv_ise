// PyBind11 includes
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Plog includes
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// FeNN backend includes
#include "backend/backend_fenn_sim.h"
#include "backend/backend_fenn_hw.h"
#include "backend/model.h"
#include "backend/runtime.h"

// FeNN compiler includes
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"


// Doc strings
//#include "docStrings.h"

using namespace pybind11::literals;

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
#define WRAP_ENUM(ENUM, VAL) .value(#VAL, ENUM::VAL, DOC(ENUM, VAL))
#define WRAP_METHOD(NAME, CLASS, METH) .def(NAME, &CLASS::METH, DOC(CLASS, METH))
#define WRAP_NS_METHOD(NAME, NS, CLASS, METH) .def(NAME, &NS::CLASS::METH, DOC(NS, CLASS, METH))
#define WRAP_NS_ATTR(NAME, NS, CLASS, ATTR) .def_readwrite(NAME, &NS::CLASS::ATTR, DOC(NS, CLASS, ATTR))
#define WRAP_PROPERTY(NAME, CLASS, METH_STEM) .def_property(NAME, &CLASS::get##METH_STEM, &CLASS::set##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_IS(NAME, CLASS, METH_STEM) .def_property(NAME, &CLASS::is##METH_STEM, &CLASS::set##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_WO(NAME, CLASS, METH_STEM) .def_property(NAME, nullptr, &CLASS::set##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_RO(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::get##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_RO_IS(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::is##METH_STEM, DOC(CLASS, m_##METH_STEM))
#define WRAP_PROPERTY_RO_REF(NAME, CLASS, METH_STEM) .def_property_readonly(NAME, &CLASS::get##METH_STEM, pybind11::return_value_policy::reference, DOC(CLASS, m_##METH_STEM))

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{/*
const CodeGenerator::ModelSpecMerged *generateCode(ModelSpecInternal &model, CodeGenerator::BackendBase &backend, 
                                                   const std::string &sharePathStr, const std::string &outputPathStr,
                                                   bool forceRebuild, bool neverRebuild)
{
    const filesystem::path outputPath(outputPathStr);

    // Create merged model and generate code
    auto *modelMerged = new CodeGenerator::ModelSpecMerged(backend, model);
    const auto output = CodeGenerator::generateAll(
        *modelMerged, backend, 
        filesystem::path(sharePathStr), outputPath, 
        forceRebuild, neverRebuild);

#ifdef _WIN32
    // Create MSBuild project to compile and link all generated modules
    std::ofstream makefile((outputPath / "runner.vcxproj").str());
    CodeGenerator::generateMSBuild(makefile, model, backend, "", output);
#else
    // Create makefile to compile and link all generated modules
    std::ofstream makefile((outputPath / "Makefile").str());
    CodeGenerator::generateMakefile(makefile, backend, output);
#endif
    return modelMerged;
}*/

/*void initLogging(plog::Severity level)
{
    auto *consoleAppender = new plog::ConsoleAppender<plog::TxtFormatter>;
    
    // If there isn't already a plog instance, initialise one
    if(plog::get<>() == nullptr) {
        plog::init<>(level, consoleAppender);
    }
    // Otherwise, set it's max severity from GeNN preferences
    else {
        plog::get<>()->setMaxSeverity(consoleAppender);
    }
}*/
}

//----------------------------------------------------------------------------
// _fenn
//----------------------------------------------------------------------------
PYBIND11_MODULE(_fenn, m) 
{
    //------------------------------------------------------------------------
    // fenn.Shape
    //------------------------------------------------------------------------
    pybind11::class_<Shape>(m, "Shape")
         .def(pybind11::init<const std::vector<size_t>&>())
         .def_property_readonly("dims", &Shape::getDims)
         .def_property_readonly("num_neurons", &Shape::getNumNeurons)
         .def_property_readonly("num_source_neurons", &Shape::getNumSourceNeurons)
         .def_property_readonly("num_target_neurons", &Shape::getNumTargetNeurons)
         .def_property_readonly("batch_size", &Shape::getBatchSize)
         .def_property_readonly("flattened_size", &Shape::getFlattenedSize)
         
         .def("__repr__", &Shape::toString);
    
    //------------------------------------------------------------------------
    // fenn.ModelComponent
    //------------------------------------------------------------------------
    pybind11::class_<ModelComponent, std::shared_ptr<ModelComponent>>(m, "ModelComponent")
         .def_property_readonly("name", &ModelComponent::getName);
    
    //------------------------------------------------------------------------
    // fenn.Process
    //------------------------------------------------------------------------
    pybind11::class_<Process, ModelComponent, std::shared_ptr<Process>>(m, "Process");
     
    //------------------------------------------------------------------------
    // fenn.State
    //------------------------------------------------------------------------
    pybind11::class_<State, ModelComponent, std::shared_ptr<State>>(m, "State");
    
    //------------------------------------------------------------------------
    // fenn.EventContainer
    //------------------------------------------------------------------------
    pybind11::class_<EventContainer, State, std::shared_ptr<EventContainer>>(m, "EventContainer")
        .def(pybind11::init(&EventContainer::create),
             pybind11::arg("shape"), pybind11::arg("num_buffer_timesteps") = 1,
             pybind11::arg("name") = "")
        
        .def_property_readonly("shape", &EventContainer::getShape)
        .def_property_readonly("num_buffer_timesteps", &EventContainer::getNumBufferTimesteps);
}
