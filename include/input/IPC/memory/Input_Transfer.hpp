#pragma once
#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "ipc_named_event.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <botan/mac.h>
#include <botan/mem_ops.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <thread>
using nj = nlohmann::json;
struct PDJE_API PDJE_Input_Log {
    PDJE_Dev_Type    type;
    PDJE_Input_Event event;
    PDJE_HID_Event   hid_event;
    char             id[256];
    char             name[256];
    uint16_t         id_len;
    uint16_t         name_len;
    uint64_t         microSecond;
};
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
#ifdef WIN32
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
#endif

  public:
#ifdef WIN32
    void
    SendManageWorker();
    std::string
    GetMetaDatas();
    PDJE_Input_Transfer(const std::string &metajson); // subprocess init
    PDJE_Input_Transfer(
        const Input_Transfer_Metadata &metad); // mainprocess init
#else
    std::vector<PDJE_Input_Log>          datas;
    Atomic_Double_Buffer<PDJE_Input_Log> adbf;
    PDJE_Input_Transfer(const uint32_t max_length); // no ipc transmission
#endif

    void
    Write(const PDJE_Input_Log &input);
    void
    Receive();
    ~PDJE_Input_Transfer();
};

}; // namespace PDJE_IPC