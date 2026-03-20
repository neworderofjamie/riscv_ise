#pragma once

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

// Common includes
#include "common/common_export.h"

// Forward declarations
namespace plog
{
class IAppender;
}

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
// Shorthand macros for logging to 'frontend' channel
#define LOGV_FRONTEND LOGV_(Common::Logging::CHANNEL_FRONTEND)
#define LOGD_FRONTEND LOGD_(Common::Logging::CHANNEL_FRONTEND)
#define LOGI_FRONTEND LOGI_(Common::Logging::CHANNEL_FRONTEND)
#define LOGW_FRONTEND LOGW_(Common::Logging::CHANNEL_FRONTEND)
#define LOGE_FRONTEND LOGE_(Common::Logging::CHANNEL_FRONTEND)
#define LOGF_FRONTEND LOGF_(Common::Logging::CHANNEL_FRONTEND)

// Shorthand macros for logging to 'compiler frontend' channel
#define LOGV_COMPILER_FRONTEND LOGV_(Common::Logging::CHANNEL_COMPILER_FRONTEND)
#define LOGD_COMPILER_FRONTEND LOGD_(Common::Logging::CHANNEL_COMPILER_FRONTEND)
#define LOGI_COMPILER_FRONTEND LOGI_(Common::Logging::CHANNEL_COMPILER_FRONTEND)
#define LOGW_COMPILER_FRONTEND LOGW_(Common::Logging::CHANNEL_COMPILER_FRONTEND)
#define LOGE_COMPILER_FRONTEND LOGE_(Common::Logging::CHANNEL_COMPILER_FRONTEND)
#define LOGF_COMPILER_FRONTEND LOGF_(Common::Logging::CHANNEL_COMPILER_FRONTEND)

//----------------------------------------------------------------------------
// Common::Logging
//----------------------------------------------------------------------------
namespace Common::Logging
{
enum Channel
{
    CHANNEL_FRONTEND,
    CHANNEL_COMPILER_FRONTEND,
    CHANNEL_MAX
};

COMMON_EXPORT void init(plog::Severity frontendLevel, plog::Severity compilerFrontendLevel, 
                        plog::IAppender *frontendAppender, plog::IAppender *compilerFrontendAppender);
}