#pragma once
#include "PDJE_IPC_SHARE_MEM_UTIL.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "nlohmann/json_fwd.hpp"
#include <asm-generic/socket.h>
#include <filesystem>
#include <functional>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>
#ifdef WIN32

#else
#include <sys/socket.h>
#endif
namespace PDJE_IPC_UTILS {
using nj = nlohmann::json;
#ifdef WIN32

#else

template <typename T, int MEM_PROT_FLAG> struct IPC_SHM_LINUX {
  private:
    iovec    iov = { .iov_base = (void *)"F", .iov_len = 1 };
    cmsghdr *cmsg;

  public:
    msghdr msg = { 0 };
    IPC_SHM_LINUX(const IPCSharedMem<T, MEM_PROT_FLAG> &shared_IPC_mem)
    {
        msg.msg_iov    = &iov;
        msg.msg_iovlen = 1;
        char cmsgbuf[CMSG_SPACE(sizeof(int))];
        msg.msg_control    = cmsgbuf;
        msg.msg_controllen = sizeof(cmsgbuf);

        cmsg             = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type  = SCM_RIGHTS;
        cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
        memcpy(CMSG_DATA(cmsg), &shared_IPC_mem.FD, sizeof(int));
    }
};
struct Importants {
    int         socket_fd = -1;
    int         child_fd  = -1;
    pid_t       child_pid = -1;
    std::string socket_path =
        "/tmp/pdje_input_module_libevdev_socket_path.sock";
};
#endif

class ChildProcess {
  private:
    std::unordered_map<std::string, std::function<void()>> callables;
    void
    EndTransmission();

    void
    ParseAndCall(const std::string &msg);

  public:
    ChildProcess(const int port);
    ~ChildProcess();
};

class MainProcess {
  private:
    std::optional<httplib::Client> cli;

    Importants imp;

  public:
    template <typename T, int MEM_PROT_FLAG>
    bool
    SendIPCSharedMemory(const IPCSharedMem<T, MEM_PROT_FLAG> &mem,
                        const std::string                    &mem_path,
                        const std::string                    &dataType);

    MainProcess(const int port);
    ~MainProcess();
};

/*********************************************************************************/

template <typename T, int MEM_PROT_FLAG>
bool
MainProcess::SendIPCSharedMemory(const IPCSharedMem<T, MEM_PROT_FLAG> &mem,
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
#ifdef WIN32
            return true;
#else
            auto shmem_share = IPC_SHM_LINUX(mem);
            auto send_res    = sendmsg(imp.client_fd, shmem_share.msg, 0);
            if (send_res != 0) {
                critlog("failed to send shared memory fd in linux socket "
                        "protocol.");
                return false;
            } else {
                return true;
            }
#endif
        } else {
            critlog("failed to set shared memory. status: ");
            critlog(res->status);
            return false;
        }
    }
}

}; // namespace PDJE_IPC_UTILS