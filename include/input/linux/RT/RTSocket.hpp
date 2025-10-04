#pragma once
#include <cstddef>
#include <fcntl.h>
#include <functional>
#include <libevdev/libevdev.h>
#include <numa.h>
#include <numaif.h>
#include <sched.h>
#include <string>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "Common_Features.hpp"
#include "nlohmann/json_fwd.hpp"

#include <nlohmann/json.hpp>
#include <unordered_map>

using nj            = nlohmann::json;
using data_body     = std::vector<std::string>;
using regi_function = std::function<int(const data_body &)>;

struct RT_ID {
    int host_socket = -1;
};

class RTSocket {
  private:
    std::unordered_map<std::string, regi_function> functionRegistry;
    std::unordered_map<int, std::function<void()>> errorHandler;
    int
          CoreValid(int core_number);
    RT_ID importants;

    int
    ParseMsg(const std::string &raw_json_msg);

    bool
    FixCPU(int core_number = 2);
    void
    MLock();

    int
    SocketOpen(const std::string &socket_path);

    int
    SocketClose();

    void
    RegisterFunctions();

  public:
    void
    SendMsg(const std::string &msg);
    int
    Communication();

    RTSocket(const std::string &socket_path);
    ~RTSocket();
    std::string ErrMsg;
};