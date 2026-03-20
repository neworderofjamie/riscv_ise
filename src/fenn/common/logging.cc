#include "fenn/common/logging.h"

// PLOG includes
#include <plog/Init.h>

//----------------------------------------------------------------------------
// FeNN::Common::Logging
//----------------------------------------------------------------------------
namespace FeNN::Common::Logging
{
void init(plog::Severity commonLevel, plog::Severity assemblerLevel, plog::Severity backendLevel, 
          plog::Severity compilerLevel, plog::Severity iseLevel, plog::IAppender *commonAppender, 
          plog::IAppender *assemblerAppender, plog::IAppender *backendAppender, 
          plog::IAppender *compilerAppender, plog::IAppender *iseAppender)
{
    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FENN_COMMON>() == nullptr) {
        plog::init<CHANNEL_FENN_COMMON>(commonLevel, commonAppender);
    }
    // Otherwise, just update max severity
    else {
        plog::get<CHANNEL_FENN_COMMON>()->setMaxSeverity(commonLevel);
    }

    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FENN_ASSEMBLER>() == nullptr) {
        plog::init<CHANNEL_FENN_ASSEMBLER>(assemblerLevel, assemblerAppender);
    }
    // Otherwise, just update  max severity
    else {
        plog::get<CHANNEL_FENN_ASSEMBLER>()->setMaxSeverity(assemblerLevel);
    }

    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FENN_BACKEND>() == nullptr) {
        plog::init<CHANNEL_FENN_BACKEND>(backendLevel, backendAppender);
    }
    // Otherwise, just update  max severity
    else {
        plog::get<CHANNEL_FENN_BACKEND>()->setMaxSeverity(backendLevel);
    }

    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FENN_COMPILER>() == nullptr) {
        plog::init<CHANNEL_FENN_COMPILER>(compilerLevel, compilerAppender);
    }
    // Otherwise, just update  max severity
    else {
        plog::get<CHANNEL_FENN_COMPILER>()->setMaxSeverity(compilerLevel);
    }

    // If there isn't already a plog instance, initialise one
    if(plog::get<CHANNEL_FENN_ISE>() == nullptr) {
        plog::init<CHANNEL_FENN_ISE>(iseLevel, iseAppender);
    }
    // Otherwise, just update  max severity
    else {
        plog::get<CHANNEL_FENN_ISE>()->setMaxSeverity(iseLevel);
    }

}
}