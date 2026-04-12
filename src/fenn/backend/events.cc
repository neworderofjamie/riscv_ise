#include "fenn/backend/events.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/logging.h"

// FeNN backend includes
#include "fenn/backend/runtime.h"

//----------------------------------------------------------------------------
// FeNN::Backend::EventContainer
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
std::unique_ptr<Frontend::ArrayBase> EventContainer::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                                 Frontend::DeviceBase &device) const
{
    LOGI_FENN_BACKEND << "Creating event container '" << getName() << "' array in BRAM";

    // Event containers are implemented as word-aligned bitfields so divide and pad last axis
    auto wordAlignedShape = deviceShape;
    wordAlignedShape.getLast() = ::Common::Utils::ceilDivide(wordAlignedShape.getLast(), 32);

    // Create BRAM array
    return static_cast<DeviceFeNN&>(device).createBRAMArray(CompilerFrontend::Type::Uint32, wordAlignedShape);
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannel
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> EventChannel::createArray(const Frontend::Shape &deviceShape, const Frontend::Model&,
                                                               Frontend::DeviceBase &device) const
{
    // **TODO** recording
    return nullptr;
}
}
