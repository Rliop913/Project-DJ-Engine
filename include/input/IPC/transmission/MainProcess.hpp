#pragma once
#include "PDJE_LOG_SETTER.hpp"
#include "ipc_shared_memory.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Input_DataLine.hpp"
#include <filesystem>
#include <functional>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>


#ifdef WIN32

#elif defined(__linux__)
#include <sys/socket.h>
#include "pack_ipc.hpp"
#endif

namespace PDJE_IPC {
using nj = nlohmann::json;

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

class MainProcess {
  private:
    std::optional<httplib::Client> cli;

    Importants imp;

  public:
    template <typename T, int MEM_PROT_FLAG>
    bool
    SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                        const std::string                 &mem_path,
                        const std::string                 &dataType);
    template <typename T>
                        bool
    SendBufferArena(const PDJE_Buffer_Arena<T> &mem); //todo -impl
    
    std::vector<DeviceData> 
    GetDevices();

    bool
    QueryConfig(const std::string& dumped_json){
      auto res = cli->Post("/config", dumped_json, "application/json");
      if(res->status == 200){
        return true;
      }
      else{
        critlog(res->body);
        return false;
      }
    }

    bool
    EndTransmission();

    bool
    Kill();

    MainProcess(const int port);
    ~MainProcess();
};

}; // namespace PDJE_IPC

#ifdef WIN32
#include "ipc_Send_Windows.tpp"
#elif defined(__linux__)
#include "ipc_Send_Linux.tpp"
#endif