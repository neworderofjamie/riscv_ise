#include "common/logging.h"

// PLOG includes
#include <plog/Init.h>

//----------------------------------------------------------------------------
// GeNN::Logging
//----------------------------------------------------------------------------
namespace Common::Logging
{
void init(plog::Severity frontendLevel, plog::Severity compilerFrontendLevel, 
          plog::IAppender *frontendAppender, plog::IAppender *compilerFrontendAppender)
{
    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FRONTEND>() == nullptr) {
        plog::init<CHANNEL_FRONTEND>(frontendLevel, frontendAppender);
    }
    // Otherwise, just update max severity
    else {
        plog::get<CHANNEL_FRONTEND>()->setMaxSeverity(frontendLevel);
    }

    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_COMPILER_FRONTEND>() == nullptr) {
        plog::init<CHANNEL_COMPILER_FRONTEND>(compilerFrontendLevel, compilerFrontendAppender);
    }
    // Otherwise, just update  max severity
    else {
        plog::get<CHANNEL_COMPILER_FRONTEND>()->setMaxSeverity(compilerFrontendLevel);
    }
}
}