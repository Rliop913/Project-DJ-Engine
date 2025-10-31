#pragma once
#include "ipc_shared_memory.hpp"
#include "ipc_util.hpp"
#include <filesystem>
#include <functional>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>

namespace PDJE_IPC {
using nj = nlohmann::json;

struct Importants {
    STARTUPINFOW start_up_info{};
    PROCESS_INFORMATION process_info{};
};


class MainProcess {
  private:
    std::optional<httplib::Client> cli;

    Importants imp;

  public:
    template <typename T, int MEM_PROT_FLAG>
    bool
    SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                        const std::string                    &mem_path,
                        const std::string                    &dataType);
    
    bool
    EndTransmission();
    MainProcess(const int port);
    ~MainProcess();
};

/*********************************************************************************/

template <typename T, int MEM_PROT_FLAG>
bool
MainProcess::SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                                 const std::string                    &mem_path,
                                 const std::string                    &dataType)
{
    if (!cli) {
        critlog("mainprocess is not initialized.");
        return false;
    }
    nj j;
    j["PATH"]     = mem_path;
    j["DATATYPE"] = dataType;
    j["COUNT"]    = mem.data_count;
    auto res      = cli->Post("/shmem", j.dump(), "application/json");
    if (!res) {
        critlog("shared memory setting from main process has failed result.");
        return false;
    } else {
        if (res->status / 100 == 2) {

            return true;

        } else {
            critlog("failed to set shared memory. status: ");
            critlog(res->status);
            return false;
        }
    }
}

}; // namespace PDJE_IPC_UTILS