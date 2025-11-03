#include "ChildProcess.hpp"
#include "ipc_shared_memory.hpp"
namespace PDJE_IPC {

ChildProcess::ChildProcess()
{
    server.Get("/stop", [&](const httplib::Request &, httplib::Response &res) {
        res.set_content("stopped", "text/plain");
        EndTransmission();
    });
    // stop request
}

void
ChildProcess::RunServer(const int port)
{
    server.listen("127.0.0.1", port);
}
void
ChildProcess::EndTransmission()
{
    server.stop();
}
}; // namespace PDJE_IPC