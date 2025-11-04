#pragma once

#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "ipc_shared_memory.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
namespace PDJE_IPC {

using PDJE_DEV_PATH = std::string;
using PDJE_NAME     = std::string;
class ChildProcess {
  private:
#ifdef WIN32
    DWORD ThreadID;
#elif defined(__linux__)

#endif
    std::unordered_map<std::string, std::function<void()>> callables;
    httplib::Server                                        server;

    std::unordered_map<PDJE_ID, PDJE_NAME>           id_name;
    std::optional<PDJE_Buffer_Arena<PDJE_Input_Log>> input_buffer;
    std::optional<PDJE_IPC::SharedMem<int, PDJE_IPC::PDJE_IPC_RW>>
        spinlock_run; // 0 = stop, 1 = go, -1 = terminate

    void
    EndTransmission(const httplib::Request &, httplib::Response &res);

    bool
    RecvIPCSharedMem(const std::string &mem_path,
                     const std::string &dataType,
                     const uint64_t     data_count); // todo - impl

    std::vector<DeviceData>                      configed_devices;
    std::unordered_map<PDJE_DEV_PATH, PDJE_NAME> unlisted_targets;

    std::string
    ListDev();

  public:
    bool KillCheck = false;
    ChildProcess()
    {
        startlog();
        server.Get("/kill",
                   [&](const httplib::Request &req, httplib::Response &res) {
                       EndTransmission(req, res);
                       KillCheck = true;
                   });
        server.Get("/stop",
                   [&](const httplib::Request &req, httplib::Response &res) {
                       EndTransmission(req, res);
                   });
        server.Get("/health",
                   [&](const httplib::Request &, httplib::Response &res) {
                       res.set_content("serverOK", "text/plain");
                   });
        server.Get("/lsdev",
                   [&](const httplib::Request &req, httplib::Response &res) {
                       res.set_content(ListDev(), "application/json");
                   });
        server.Post("/config",
                    [&](const httplib::Request &req, httplib::Response &res) {
                        try {
                            configed_devices.clear();
                            auto nj = nlohmann::json::parse(req.body);
                            for (const auto &i : nj["body"]) {
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
                                configed_devices.push_back(dd);
                            }
                            res.status = 200;

                        } catch (const std::exception &e) {
                            res.status = 400;
                            std::string errlog =
                                "INVALID_JSON. why:" + std::string(e.what());
                            res.set_content(errlog, "text/plain");

                            critlog("failed to config device data. WHY: ");
                            critlog(e.what());
                            critlog("received json: ");
                            critlog(req.body);
                        }
                    });
        server.Post(
            "/shmem", [&](const httplib::Request &req, httplib::Response &res) {
                try {

                    auto nj = nlohmann::json::parse(req.body);

                    if (!RecvIPCSharedMem(nj.at("PATH").get<std::string>(),
                                          nj.at("DATATYPE").get<std::string>(),
                                          nj.at("COUNT").get<uint64_t>())) {
                        throw std::runtime_error(
                            "failed to receive ipc shared memory.");
                    }

                    res.status = 200;
                } catch (const std::exception &e) {
                    res.status = 400;
                    std::string errlog =
                        "INVALID_JSON. why:" + std::string(e.what());
                    res.set_content(errlog, "text/plain");

                    critlog("failed to config device data. WHY: ");
                    critlog(e.what());
                    critlog("received json: ");
                    critlog(req.body);
                }
            });
    }
    PDJE_HIGHRES_CLOCK::CLOCK timer;
    void
    RunServer(const int port);
    void *
    Init();
    void
    LoopTrig();
    void
    Run();

    ~ChildProcess() = default;
};
}; // namespace PDJE_IPC