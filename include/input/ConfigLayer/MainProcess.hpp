#pragma once

#include <string>

#ifdef WIN32

#elif defined(__linux__)

#include <sys/socket.h>
#endif

namespace PDJE_IPC {

struct Importants {
#ifdef WIN32
    STARTUPINFOW        start_up_info{};
    PROCESS_INFORMATION process_info{};
#elif defined(__linux__)
    int         socket_fd = -1;
    int         child_fd  = -1;
    pid_t       child_pid = -1;
    std::string socket_path =
        "/tmp/pdje_input_module_libevdev_socket_path.sock";
#endif
};

class MainProc {
  private:
    Importants imp;

  public:
    template <typename T, int MEM_PROT_FLAG>
    bool
    SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                        const std::string                 &mem_path,
                        const std::string                 &dataType);
    template <typename T>
    bool
    SendBufferArena(const PDJE_Buffer_Arena<T> &mem);

    MainProc();
    ~MainProc();
};

}; // namespace PDJE_IPC

#ifdef WIN32
#include "ipc_Send_Windows.tpp"
#elif defined(__linux__)
#include "ipc_Send_Linux.tpp"
#endif