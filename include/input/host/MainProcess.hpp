#pragma once
#include "NameGen.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "Secured_IPC_TX_RX.hpp"
#include "ipc_shared_memory.hpp"
#include <PDJE_Crypto.hpp>
#include <filesystem>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>

#ifdef WIN32

#elif defined(__linux__)

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
namespace MAINPROC {

class TXRXTransport {
  private:
    std::optional<PDJE_CRYPTO::TX_RX> txrx;
    PDJE_CRYPTO::PSK                  psk;
    Importants                        imp;
    struct {
        std::optional<std::promise<bool>>                    HEALTH_CHECK;
        std::optional<std::promise<bool>>                    STOP;
        std::optional<std::promise<bool>>                    KILL;
        std::optional<std::promise<std::vector<DeviceData>>> DEVICE_LIST;
        std::optional<std::promise<bool>>                    DEVICE_CONFIG;
        std::optional<std::promise<bool>>                    SEND_IPC_SHMEM;
        std::optional<std::promise<bool>> SEND_INPUT_TRANSFER_SHMEM;
    } TXRX_RESPONSE;

  public:
    struct {
        EVENT input_loop_run_event;
        EVENT terminate_event;
    } events;
    void
    SetTXRX_Features();

    bool
    CheckHealth();

    bool
    SendIPCSharedMemory(const uint64_t     mem_length,
                        const std::string &mem_path,
                        const std::string &dataType);
    bool
    SendInputTransfer(PDJE_Input_Transfer &trsf);

    std::vector<DeviceData>
    GetDevices();

    bool
    QueryConfig(const std::string &dumped_json);

    bool
    EndTransmission();

    void
    InitEvents();
    bool
    Kill();

    TXRXTransport();
    ~TXRXTransport();
};

}; // namespace MAINPROC

}; // namespace PDJE_IPC