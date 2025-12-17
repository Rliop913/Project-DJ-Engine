#include "ChildProcess.hpp"
#include "ipc_shared_memory.hpp"
namespace PDJE_IPC {

void
SubProc::RunServer(const int port)
{
    server.listen("0.0.0.0", port);
}
void
SubProc::EndTransmission(const httplib::Request &, httplib::Response &res)
{
    res.set_content("stopped", "text/plain");
    server.stop();
}
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