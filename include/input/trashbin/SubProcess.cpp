#include "SubProcess.hpp"

#include "ipc_shared_memory.hpp"
namespace PDJE_IPC {
using namespace SUBPROC;
bool
TXRXListener::RecvIPCSharedMem(const std::string &mem_path,
                               const std::string &dataType,
                               const uint64_t     data_count)
{
    try {

        if (dataType == "EVENT_terminate") {
            terminate_event.ClientInit(mem_path);
            return true;
        }
        if (dataType == "EVENT_input_loop_run") {
            input_loop_run_event.ClientInit(mem_path);
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        critlog("failed to receive memory. WHY: ");
        critlog(e.what());
        return false;
    }
}

std::string
TXRXListener::ListDev()
{
}

void
TXRXListener::LoopTrig()
{
}

}; // namespace PDJE_IPC