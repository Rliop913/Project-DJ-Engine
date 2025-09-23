#include "linux_input.hpp"
#include "Input_State.hpp"
#include "spawn.h"
#include <cerrno>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int
OS_Input::OpenClientWithSudo(const std::string &exec_path,
                             const std::string &arg)
{
    char *pkexec_args[] = { (char *)"pkexec",
                            (char *)exec_path.c_str(),
                            (char *)arg.c_str(),
                            nullptr };
    char *sudo_args[]   = {
        (char *)"sudo", (char *)exec_path.c_str(), (char *)arg.c_str(), nullptr
    };

    if ((getenv("DISPLAY") || getenv("WAYLAND_DISPLAY")) &&
        access("/usr/bin/pkexec", X_OK) == 0) {
        int spawn_stat = posix_spawn(&importants.rt_pid,
                                     "/usr/bin/pkexec",
                                     nullptr,
                                     nullptr,
                                     pkexec_args,
                                     environ);
        if (spawn_stat == 0) {
            return 0;
        }
    } else {
        int spawn_stat = posix_spawn(&importants.rt_pid,
                                     "/usr/bin/sudo",
                                     nullptr,
                                     nullptr,
                                     sudo_args,
                                     environ);
        if (spawn_stat == 0) {
            return 0;
        }
    }
    return errno;
}

int
OS_Input::SocketOpen(const std::string &exec_path)
{
    unlink(importants.socket_path.c_str());
    importants.socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un address_temp{};
    address_temp.sun_family = AF_UNIX;

    if (bind(importants.socket_fd,
             reinterpret_cast<sockaddr *>(&address_temp),
             sizeof(address_temp)) < 0) {
        return errno;
    }

    if (listen(importants.socket_fd, 1) < 0) {
        return errno;
    }

    if (OpenClientWithSudo(exec_path, importants.socket_path) < 0) {
        return errno;
    }

    importants.client_fd = accept(importants.socket_fd, nullptr, nullptr);
    if (importants.client_fd < 0) {
        return errno;
    }
}