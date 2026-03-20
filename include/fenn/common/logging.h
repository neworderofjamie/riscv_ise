#pragma once

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>

// Common includes
#include "common/logging.h"

// FeNN common includes
#include "fenn/common/fenn_common_export.h"

// Forward declarations
namespace plog
{
class IAppender;
}

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
// Shorthand macros for logging to 'FeNN common' channel
#define LOGV_FENN_COMMON LOGV_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)
#define LOGD_FENN_COMMON LOGD_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)
#define LOGI_FENN_COMMON LOGI_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)
#define LOGW_FENN_COMMON LOGW_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)
#define LOGE_FENN_COMMON LOGE_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)
#define LOGF_FENN_COMMON LOGF_(FeNN::Common::Logging::CHANNEL_FENN_COMMON)

// Shorthand macros for logging to 'FeNN assembler' channel
#define LOGV_FENN_ASSEMBLER LOGV_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)
#define LOGD_FENN_ASSEMBLER LOGD_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)
#define LOGI_FENN_ASSEMBLER LOGI_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)
#define LOGW_FENN_ASSEMBLER LOGW_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)
#define LOGE_FENN_ASSEMBLER LOGE_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)
#define LOGF_FENN_ASSEMBLER LOGF_(FeNN::Common::Logging::CHANNEL_FENN_ASSEMBLER)

// Shorthand macros for logging to 'FeNN backend' channel
#define LOGV_FENN_BACKEND LOGV_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)
#define LOGD_FENN_BACKEND LOGD_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)
#define LOGI_FENN_BACKEND LOGI_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)
#define LOGW_FENN_BACKEND LOGW_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)
#define LOGE_FENN_BACKEND LOGE_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)
#define LOGF_FENN_BACKEND LOGF_(FeNN::Common::Logging::CHANNEL_FENN_BACKEND)

// Shorthand macros for logging to 'FeNN compiler' channel
#define LOGV_FENN_COMPILER LOGV_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)
#define LOGD_FENN_COMPILER LOGD_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)
#define LOGI_FENN_COMPILER LOGI_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)
#define LOGW_FENN_COMPILER LOGW_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)
#define LOGE_FENN_COMPILER LOGE_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)
#define LOGF_FENN_COMPILER LOGF_(FeNN::Common::Logging::CHANNEL_FENN_COMPILER)

// Shorthand macros for logging to 'FeNN ISE' channel
#define LOGV_FENN_ISE LOGV_(FeNN::Common::Logging::CHANNEL_FENN_ISE)
#define LOGD_FENN_ISE LOGD_(FeNN::Common::Logging::CHANNEL_FECHANNEL_FENN_ISENN_COMPILER)
#define LOGI_FENN_ISE LOGI_(FeNN::Common::Logging::CHANNEL_FENN_ISE)
#define LOGW_FENN_ISE LOGW_(FeNN::Common::Logging::CHANNEL_FENN_ISE)
#define LOGE_FENN_ISE LOGE_(FeNN::Common::Logging::CHANNEL_FENN_ISE)
#define LOGF_FENN_ISE LOGF_(FeNN::Common::Logging::CHANNEL_FENN_ISE)
//----------------------------------------------------------------------------
// FeNN::Common::Logging
//----------------------------------------------------------------------------
namespace FeNN::Common::Logging
{
enum Channel
{
    CHANNEL_FENN_COMMON         = ::Common::Logging::CHANNEL_MAX,
    CHANNEL_FENN_ASSEMBLER,
    CHANNEL_FENN_BACKEND,
    CHANNEL_FENN_COMPILER,
    CHANNEL_FENN_ISE,
};

FENN_COMMON_EXPORT void init(plog::Severity commonLevel, plog::Severity assemblerLevel, 
                             plog::Severity backendLevel, plog::Severity compilerLevel,
                             plog::Severity iseLevel, plog::IAppender *commonAppender, 
                             plog::IAppender *assemblerAppender, plog::IAppender *backendAppender, 
                             plog::IAppender *compilerAppender, plog::IAppender *iseAppender);
}