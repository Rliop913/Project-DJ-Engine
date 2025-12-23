#pragma once

#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "Secured_IPC_TX_RX.hpp"
#include "ipc_shared_memory.hpp"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
namespace PDJE_IPC {

using PDJE_DEV_PATH = std::string;
using PDJE_NAME     = std::string;
// using NAME_OFFSET   = std::pair<PDJE_NAME, int64_t>;
class SubProc {
  private:
#ifdef WIN32
    DWORD ThreadID;
#elif defined(__linux__)

#endif
    std::optional<PDJE_CRYPTO::TX_RX>      txrx;
    std::unordered_map<PDJE_ID, PDJE_NAME> id_name;

    std::optional<PDJE_IPC::PDJE_Input_Transfer> input_buffer;
    std::optional<PDJE_IPC::SharedMem<int, PDJE_IPC::PDJE_IPC_RW>>
        spinlock_run; // 0 = stop, 1 = go, -1 = terminate

    bool
    RecvIPCSharedMem(const std::string &mem_path,
                     const std::string &dataType,
                     const uint64_t     data_count);

    std::vector<DeviceData>                      configed_devices;
    std::unordered_map<PDJE_DEV_PATH, PDJE_NAME> unlisted_targets;

    std::string
    ListDev();

  public:
    bool KillCheck = false;
    SubProc(PDJE_CRYPTO::PSK      &psk,
            const PDJE_IPC::MNAME &memFirst,
            const PDJE_IPC::MNAME &firstLock,
            const PDJE_IPC::MNAME &memSecond,
            const PDJE_IPC::MNAME &secondLock)
    {
        startlog();
        txrx.emplace(psk, memFirst, firstLock, memSecond, secondLock, false);

        txrx->AddFunction(
            PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, [this](const std::string &msg) {
                KillCheck = true;
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, "OK");
                txrx->StopListen();
            });

        txrx->AddFunction(
            PDJE_CRYPTO::TXRXHEADER::TXRX_STOP, [this](const std::string &msg) {
                txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP, "OK");
                txrx->StopListen();
            });

        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK,
                          [this](const std::string &msg) {
                              txrx->Send(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK,
                                         "OK");
                          });

        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST,
                          [this](const std::string &msg) {
                              txrx->Send(PDJE_CRYPTO::DEVICE_LIST, ListDev());
                          });

        txrx->AddFunction(
            PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG,
            [this](const std::string &msg) {
                try {
                    configed_devices.clear();
                    auto nj = nlohmann::json::parse(msg);
                    for (const auto &i : nj["body"]) {
                        DeviceData dd;
                        dd.device_specific_id = i.at("id").get<std::string>();
                        dd.Name               = i.at("name").get<std::string>();

                        std::string tp = i.at("type").get<std::string>();
                        if (tp == "KEYBOARD") {
                            dd.Type = PDJE_Dev_Type::KEYBOARD;
                        } else if (tp == "MOUSE") {
                            dd.Type = PDJE_Dev_Type::MOUSE;
                        } else if (tp == "MIDI") {
                            dd.Type = PDJE_Dev_Type::MIDI;
                        } else if (tp == "HID") {
                            dd.Type = PDJE_Dev_Type::HID;
                        } else {
                            continue;
                        }
                        configed_devices.push_back(dd);
                    }
                    txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, "OK");
                } catch (const std::exception &e) {

                    std::string errlog =
                        "INVALID_JSON. why:" + std::string(e.what());
                    txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, errlog);

                    critlog("failed to config device data. WHY: ");
                    critlog(e.what());
                    critlog("received json: ");
                    critlog(msg);
                }
            });

        txrx->AddFunction(
            PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM,
            [this](const std::string &msg) {
                try {
                    auto nj = nlohmann::json::parse(msg);

                    if (!RecvIPCSharedMem(nj.at("PATH").get<std::string>(),
                                          nj.at("DATATYPE").get<std::string>(),
                                          nj.at("COUNT").get<uint64_t>())) {
                        throw std::runtime_error(
                            "failed to receive ipc shared memory.");
                    }
                    txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, "OK");
                } catch (const std::exception &e) {
                    std::string errlog =
                        "INVALID_JSON. why:" + std::string(e.what());
                    txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM, errlog);
                    critlog("failed to config device data. WHY: ");
                    critlog(e.what());
                    critlog("received json: ");
                    critlog(msg);
                }
            });
        txrx->AddFunction(
            PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
            [this](const std::string &msg) {
                try {
                    input_buffer.emplace(msg);
                } catch (const std::exception &e) {
                    std::string errlog =
                        "INVALID_JSON. why:" + std::string(e.what());
                    txrx->Send(
                        PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
                        errlog);
                    critlog("failed to config device data. WHY: ");
                    critlog(e.what());
                    critlog("received json: ");
                    critlog(msg);
                }
            });
    }
    PDJE_HIGHRES_CLOCK::CLOCK timer;
    void
    BlockedListen()
    {
        txrx->BlockedListen();
    }
    void *
    Init();
    void
    LoopTrig();
    void
    Run();

    ~SubProc() = default;
};
}; // namespace PDJE_IPC