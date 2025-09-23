#pragma once
#include <cstddef>
#include <fcntl.h>
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
struct RT_ID {
    int host_socket = -1;
};

class RTSocket {
  private:
    int
          CoreValid(int core_number);
    RT_ID importants;

  public:
    bool
    FixCPU(int core_number = 2);
    void
    MLock();

    void
    SendMsg(const std::string &msg);
    int
    SocketOpen(const std::string &socket_path);
    int
    SocketClose();
    int
    SocketRecv();
    RTSocket();
    ~RTSocket();
    std::string ErrMsg;
};