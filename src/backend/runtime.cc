#include "backend/runtime.h"

// Backend includes
#include "backend/backend_fenn.h"

namespace
{

}

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const ProcessFields &processFields, const BackendFeNN &backend)
{
    m_State = backend.createState();
}
//----------------------------------------------------------------------------
void Runtime::setInstructions(const std::vector<uint32_t> &instructions)
{
    m_State->setInstructions(instructions);
}
//----------------------------------------------------------------------------
void Runtime::allocate()
{

}
//----------------------------------------------------------------------------
void Runtime::run()
{
    m_State->run();
}
//----------------------------------------------------------------------------
ArrayBase *Runtime::getArray(std::shared_ptr<State> variable) const
{
    return m_Arrays.at(variable).get();
}
