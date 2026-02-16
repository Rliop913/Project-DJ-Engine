#pragma once
#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_Input_Log.hpp"
#include "ipc_named_event.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <botan/mac.h>
#include <botan/mem_ops.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <thread>
using nj = nlohmann::json;
namespace PDJE_IPC {
using namespace PDJE_CRYPTO;

struct Input_Transfer_Metadata {
    uint32_t max_length = 0;
    MNAME    lenname;
    MNAME    bodyname;
    MNAME    hmacname;
    MNAME    data_request_event_name;
    MNAME    data_stored_event_name;
    PSK      psk;
};

class PDJE_API PDJE_Input_Transfer {
  private:
    SharedMem<uint64_t, PDJE_IPC_RW>                  length;
    SharedMem<PDJE_Input_Log, PDJE_IPC_RW>            body;
    SharedMem<uint8_t, PDJE_IPC_RW>                   hmac;
    EVENT                                             req_event;
    EVENT                                             stored_event;
    std::atomic<bool>                                 sendworker_switch = false;
    std::optional<std::thread>                        sendworker;
    std::mutex                                        local_locker;
    std::unique_ptr<Botan::MessageAuthenticationCode> hmacEngine;
    Input_Transfer_Metadata                           metadata;
    void
    SetHmacEngine();
    void
    SetHmac();
    bool
                                VerifyHmac();
    std::vector<PDJE_Input_Log> subBuffer;
    void
    Send();

  public:
    void
    SendManageWorker();
    std::string
    GetMetaDatas();
    PDJE_Input_Transfer(const std::string &metajson); // subprocess init
    PDJE_Input_Transfer(
        const Input_Transfer_Metadata &metad); // mainprocess init
    std::vector<PDJE_Input_Log> datas;

    void
    Write(const PDJE_Input_Log &input);
    void
    Receive();
    ~PDJE_Input_Transfer();
};

}; // namespace PDJE_IPC