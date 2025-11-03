#include "MainProcess.hpp"
#include "httplib.h"
#include "ipc_util.hpp"
#include <spawn.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace PDJE_IPC {

static std::string
GenExecuteShell(const fs::path &exec_path)
{
    auto shell =
        "sha256sum libPDJE_MODULE_INPUT.a | tr -s ' ' | cut -d ' ' -f1";
}

static bool
OpenProcess(const fs::path &pt, pid_t *child_pid)
{
    char *pkexec_args[] = { (char *)"pkexec",
                            (char *)GenExecuteShell(pt).c_str(),
                            nullptr };
    char *sudo_args[]   = { (char *)"sudo",
                            (char *)GenExecuteShell(pt).c_str(),
                            nullptr };

    if ((getenv("DISPLAY") || getenv("WAYLAND_DISPLAY")) &&
        access("/usr/bin/pkexec", X_OK) == 0) {
        int spawn_stat = posix_spawn(child_pid,
                                     "/usr/bin/pkexec",
                                     nullptr,
                                     nullptr,
                                     pkexec_args,
                                     environ);
        if (spawn_stat == 0) {
            return true;
        }
    } else {
        int spawn_stat = posix_spawn(
            child_pid, "/usr/bin/sudo", nullptr, nullptr, sudo_args, environ);
        if (spawn_stat == 0) {
            return true;
        }
    }
    return false;
}

MainProcess::MainProcess(const int port)
{
    unlink(imp.socket_path.c_str());
    imp.socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un address_temp{};
    address_temp.sun_family = AF_UNIX;

    if (bind(imp.socket_fd,
             reinterpret_cast<sockaddr *>(&address_temp),
             sizeof(address_temp)) < 0) {
        critlog("failed to bind socket fd. errno:");
        critlog(errno);

        return;
    }

    if (listen(imp.socket_fd, 1) < 0) {
        critlog("failed to listen socket. errno:");
        critlog(errno);
        return;
    }
    auto path = GetValidProcessExecutor();
    if (!OpenProcess(path, &imp.child_pid)) {
        critlog("failed to open child process. errno:");
        critlog(errno);
        return;
    }

    imp.child_fd = accept(imp.socket_fd, nullptr, nullptr);
    if (imp.child_fd < 0) {
        critlog("failed to get child process fd. errno:");
        critlog(errno);
        return;
    }
    cli.emplace("127.0.0.1", port);
    cli->set_connection_timeout(5, 0);
    cli->set_read_timeout(5, 0);
    cli->set_write_timeout(5, 0);
}

MainProcess::~MainProcess()
{
    if (imp.child_fd >= 0) {
        close(imp.child_fd);
    }
    if (imp.socket_fd >= 0) {
        close(imp.socket_fd);
    }
    unlink(imp.socket_path.c_str());
}
bool
MainProcess::EndTransmission()
{
    auto res = cli->Get("/stop");
    if (res) {
        return true;
    } else {
        return false;
    }
}
}; // namespace PDJE_IPC