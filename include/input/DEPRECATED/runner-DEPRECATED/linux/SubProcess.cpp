#include "SubProcess.hpp"
#include "ipc_shared_memory.hpp"
namespace PDJE_IPC {

bool
SubProc::RecvIPCSharedMem(const std::string &mem_path,
                          const std::string &dataType,
                          const uint64_t     data_count)
{
    return false; // todo -impl
}
std::string
SubProc::ListDev()
{
    return {}; // todo - impl
}

void
SubProc::LoopTrig()
{
    return; // todo - impl
}

}; // namespace PDJE_IPC