#pragma once
#include "PDJE_LOG_SETTER.hpp"
#include "Secured_IPC_TX_RX.hpp"
#include "SubProcess.hpp"
#include <PDJE_Crypto.hpp>
#include <PDJE_Input_DataLine.hpp>
#include <future>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>

namespace PDJE_INPUT_LAYER {
using nj = nlohmann::json;

class Config_TX_RX {
  private:
    std::optional<PDJE_CRYPTO::TX_RX> txrx;
    PDJE_CRYPTO::PSK                  psk;
    std::shared_ptr<SubProcCTRL>      sub;
    struct {
        std::optional<std::promise<bool>>                    HEALTH_CHECK;
        std::optional<std::promise<bool>>                    STOP;
        std::optional<std::promise<bool>>                    KILL;
        std::optional<std::promise<std::vector<DeviceData>>> DEVICE_LIST;
        std::optional<std::promise<bool>>                    DEVICE_CONFIG;
        std::optional<std::promise<bool>>                    SEND_IPC_SHMEM;

    } TXRX_RESPONSE;

  public:
    void
    SetTXRX_Features();

    bool
    CheckHealth();

    std::vector<DeviceData>
    GetDevices();

    bool
    SetDevices(const std::string &device_json); // todo

    bool
    Cancel(); // todo
    bool
    Quit(); // todo

    Config_TX_RX();
    ~Config_TX_RX(); // windows - send ipc, linux - end process.
};

}; // namespace PDJE_INPUT_LAYER
