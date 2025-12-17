#pragma once
#include "PDJE_Buffer.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "ipc_shared_memory.hpp"
#include <PDJE_Crypto.hpp>
#include <filesystem>
#include <functional>
#include <httplib.h>
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
    std::optional<httplib::Client>   cli;
    PDJE_CRYPTO::PSK                 psk;
    std::optional<PDJE_CRYPTO::AEAD> aead;
    Importants                       imp;

  public:
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
        auto                    res = cli->Get("/lsdev");
        std::vector<DeviceData> ddvector;
        if (res->status == 200) {
            if (!aead) {
                critlog("AEAD is not initialized. Get Devices Failed.");
                return {};
            }
            auto devs = aead->UnpackAndDecrypt(res->body);
            nj   jj   = nj::parse(devs);
            for (const auto &i : jj["body"]) {
                DeviceData dd;
                dd.device_specific_id = i.at("id").get<std::string>();
                dd.Name               = i.at("name").get<std::string>();
                std::string tp        = i.at("type").get<std::string>();
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
                ddvector.push_back(dd);
            }
            return ddvector;
        } else {
            critlog("failed to get device. status code: ");
            critlog(res->status);
            return {};
        }
    }

    bool
    QueryConfig(const std::string &dumped_json)
    {

        if (!aead) {
            critlog("AEAD is not initialized. Query Config Failed.");
            return false;
        }
        auto res = cli->Post(
            "/config", aead->EncryptAndPack(dumped_json), "application/json");
        if (res->status == 200) {
            return true;
        } else {
            critlog(res->body);
            return false;
        }
    }

    bool
    EndTransmission();

    bool
    Kill()
    {
        auto res = cli->Get("/kill");
        if (res->status == 200) {
            return true;
        } else {
            critlog(res->body);
            return false;
        }
    }

    MainProc(const int port);
    ~MainProc();
};

}; // namespace PDJE_IPC

#ifdef WIN32
#include "ipc_Send_Windows.tpp"
#elif defined(__linux__)
#include "ipc_Send_Linux.tpp"
#endif