#include "linux_input.hpp"
#include "Common_Features.hpp"
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
    return 0;
}

int
OS_Input::CloseClient()
{
    // somthing to close client
    return 0;
}

void
OS_Input::SocketClose()
{
    CloseClient();
    close(importants.client_fd);
    close(importants.socket_fd);
    unlink(importants.socket_path.c_str());
}
#include <iostream>
std::vector<DeviceData>
OS_Input::getDevices()
{
    nlohmann::json toSend;
    toSend["HEAD"] = "GET_DEV";

    std::vector<std::string> strlist;
    strlist.push_back("temp");
    toSend["BODY"] = strlist;
    std::string msggot;
    Common_Features::LPSend(importants.client_fd, toSend.dump());
    Common_Features::LPRecv(importants.client_fd, msggot);
    std::cout << msggot << std::endl;

    return std::vector<DeviceData>();
}

void
OS_Input::EndSocketTransmission()
{
    nlohmann::json toSend;
    toSend["HEAD"] = "END_SOCKET";

    std::vector<std::string> strlist;
    strlist.push_back("temp");
    toSend["BODY"] = strlist;
    std::string msggot;
    Common_Features::LPSend(importants.client_fd, toSend.dump());
    Common_Features::LPRecv(importants.client_fd, msggot);
    std::cout << msggot << std::endl;
}