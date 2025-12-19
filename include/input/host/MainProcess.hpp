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
#include "pack_ipc.hpp"
#include <sys/socket.h>
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

class MainProc {
  private:
    std::optional<PDJE_CRYPTO::TX_RX> txrx;
    PDJE_CRYPTO::PSK                  psk;
    // std::optional<PDJE_CRYPTO::AEAD> aead;
    Importants imp;
    struct {
        std::optional<std::promise<bool>>                    HEALTH_CHECK;
        std::optional<std::promise<bool>>                    STOP;
        std::optional<std::promise<bool>>                    KILL;
        std::optional<std::promise<std::vector<DeviceData>>> DEVICE_LIST;
        std::optional<std::promise<bool>> DEVICE_CONFIG;  // todo-impl
        std::optional<std::promise<bool>> SEND_IPC_SHMEM; // todo-impl

    } TXRX_RESPONSE;

  public:
    void
    SetTXRX_Features()
    {
        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK,
                          [this](const std::string &msg) {
                              if (msg == "OK") {
                                  TXRX_RESPONSE.HEALTH_CHECK->set_value(true);
                              } else {
                                  TXRX_RESPONSE.HEALTH_CHECK->set_value(false);
                              }
                          });
        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP,
                          [this](const std::string &msg) {
                              if (msg == "OK") {
                                  TXRX_RESPONSE.STOP->set_value(true);
                              } else {
                                  TXRX_RESPONSE.STOP->set_value(false);
                              }
                          });
        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL,
                          [this](const std::string &msg) {
                              if (msg == "OK") {
                                  TXRX_RESPONSE.KILL->set_value(true);
                              } else {
                                  TXRX_RESPONSE.KILL->set_value(false);
                              }
                          });
        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST,
                          [this](const std::string &msg) {
                              try {

                                  nj                      jj = nj::parse(msg);
                                  std::vector<DeviceData> dlist;
                                  for (const auto &i : jj["body"]) {
                                      DeviceData dd;
                                      dd.device_specific_id =
                                          i.at("id").get<std::string>();
                                      dd.Name = i.at("name").get<std::string>();
                                      std::string tp =
                                          i.at("type").get<std::string>();
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
                                      dlist.push_back(dd);
                                  }
                                  TXRX_RESPONSE.DEVICE_LIST->set_value(dlist);
                              } catch (const std::exception &e) {
                                  critlog("failed to list devices. Why: ");
                                  critlog(e.what());
                                  critlog("JSON dump: ");
                                  critlog(msg);
                                  TXRX_RESPONSE.DEVICE_LIST->set_value({});
                              }
                          });
        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG,
                          [this](const std::string &msg) {
                              if (msg == "OK") {
                                  TXRX_RESPONSE.DEVICE_CONFIG->set_value(true);
                              } else {
                                  critlog("Device config failed. Why:");
                                  critlog(msg);
                                  TXRX_RESPONSE.DEVICE_CONFIG->set_value(false);
                              }
                          });

        txrx->AddFunction(PDJE_CRYPTO::TXRXHEADER::SEND_IPC_SHMEM,
                          [this](const std::string &msg) {
                              if (msg == "OK") {
                                  TXRX_RESPONSE.SEND_IPC_SHMEM->set_value(true);
                              } else {
                                  critlog("Device config failed. Why:");
                                  critlog(msg);
                                  TXRX_RESPONSE.SEND_IPC_SHMEM->set_value(
                                      false);
                              }
                          });
    }

    bool
    CheckHealth()
    {
        TXRX_RESPONSE.HEALTH_CHECK.emplace();
        auto resp = TXRX_RESPONSE.HEALTH_CHECK->get_future();
        bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK, "");

        if (res) {
            res = resp.get();
        }

        TXRX_RESPONSE.HEALTH_CHECK.reset();
        if (res)
            return true;
        else {
            critlog("health check failed.");
            return false;
        }
    }

    template <typename T, int MEM_PROT_FLAG>
    bool
    SendIPCSharedMemory(const SharedMem<T, MEM_PROT_FLAG> &mem,
                        const std::string                 &mem_path,
                        const std::string                 &dataType);
    template <typename T>
    bool
    SendBufferArena(const PDJE_Buffer_Arena<T> &mem);

    std::vector<DeviceData>
    GetDevices()
    {
        TXRX_RESPONSE.DEVICE_LIST.emplace();
        auto resp = TXRX_RESPONSE.DEVICE_LIST->get_future();
        bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST, "");

        if (!res) {
            TXRX_RESPONSE.DEVICE_LIST.reset();
            critlog("failed to request device list.");
            return {};
        }
        return resp.get();
    }

    bool
    QueryConfig(const std::string &dumped_json)
    {
        TXRX_RESPONSE.DEVICE_CONFIG.emplace();
        auto resp = TXRX_RESPONSE.DEVICE_CONFIG->get_future();
        bool res = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG,
                              dumped_json);
        if (res) {
            res = resp.get();
        }

        TXRX_RESPONSE.DEVICE_CONFIG.reset();
        if (res)
            return true;
        else {
            critlog("query configure failed.");
            return false;
        }
    }

    bool
    EndTransmission();

    bool
    Kill()
    {
        TXRX_RESPONSE.KILL.emplace();
        auto resp = TXRX_RESPONSE.KILL->get_future();
        bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, "");

        if (res) {
            res = resp.get();
        }

        TXRX_RESPONSE.KILL.reset();
        txrx.reset();
        if (res)
            return true;
        else {
            critlog("failed to send kill signal.");
            return false;
        }
    }

    MainProc();
    ~MainProc();
};

}; // namespace PDJE_IPC

#ifdef WIN32
#include "ipc_Send_Windows.tpp"
#elif defined(__linux__)
#include "ipc_Send_Linux.tpp"
#endif