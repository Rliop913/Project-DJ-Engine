#pragma once
#include "Input_State.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <botan/mac.h>
#include <botan/mem_ops.h>
#include <nlohmann/json.hpp>

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
    uint32_t max_length;
    MNAME    lenname;
    MNAME    bodyname;
    MNAME    hmacname;
    MNAME    lockname;
    PSK      psk;
};

class PDJE_Input_Transfer {
  private:
    SharedMem<uint64_t, PDJE_IPC_RW>       length;
    SharedMem<PDJE_Input_Log, PDJE_IPC_RW> body;
    SharedMem<uint8_t, PDJE_IPC_RW>        hmac;
    MUTEX                                  locker;
    // PSK                                               psk;
    std::unique_ptr<Botan::MessageAuthenticationCode> hmacEngine;
    Input_Transfer_Metadata                           metadata;

    void
    SetHmacEngine()
    {
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
        if (!hmacEngine) {
            critlog("cannot create HMAC(SHA-256).");
            return;
        }
        hmacEngine->set_key(metadata.psk.psk);
    }
    void
    SetHmac()
    {
        if (!hmacEngine) {
            critlog("Failed to Set Hmac. hmac is invalid.");
            return;
        }
        try {
            hmacEngine->update(reinterpret_cast<const uint8_t *>(datas.data()),
                               (*length.ptr) * sizeof(PDJE_Input_Log));
            std::memcpy(hmac.ptr,
                        hmacEngine->final().data(),
                        hmacEngine->output_length());
        } catch (const std::exception &e) {
            critlog("caused exception on Setting hmac. What: ");
            critlog(e.what());
        }
    }
    bool
    VerifyHmac()
    {
        if (!hmacEngine) {
            critlog("Failed to Verify Hmac. hmac is invalid.");
            return false;
        }
        try {
            hmacEngine->update(reinterpret_cast<const uint8_t *>(datas.data()),
                               (*length.ptr) * sizeof(PDJE_Input_Log));
            return Botan::constant_time_compare(hmacEngine->final().data(),
                                                hmac.ptr,
                                                hmacEngine->output_length());
        } catch (const std::exception &e) {
            critlog("caused exception on VerityHmac. What: ");
            critlog(e.what());
            return false;
        }
    }

  public:
    std::vector<PDJE_Input_Log> datas;
    void
    Send();
    void
    Receive();
    PDJE_Input_Transfer(const uint32_t max_length)
    {
        datas.reserve(max_length);
    }
    std::string
    GetMetaDatas()
    {
        nj out;
        out["MAX_LENGTH"] = metadata.max_length;
        out["LENNAME"]    = metadata.lenname.string();
        out["BODYNAME"]   = metadata.bodyname.string();
        out["HMACNAME"]   = metadata.hmacname.string();
        out["LOCKNAME"]   = metadata.lockname.string();
        out["PSK"]        = metadata.psk.Encode();
        return out.dump();
    }
    PDJE_Input_Transfer(const std::string &metajson)
    {
        nj meta             = nj::parse(metajson);
        metadata.max_length = meta["MAX_LENGTH"].get<uint32_t>();
        metadata.lenname    = meta["LENNAME"].get<MNAME>();
        metadata.bodyname   = meta["BODYNAME"].get<MNAME>();
        metadata.hmacname   = meta["HMACNAME"].get<MNAME>();
        metadata.lockname   = meta["LOCKNAME"].get<MNAME>();
        if (!metadata.psk.Decode(meta["PSK"].get<std::string>())) {
            throw std::runtime_error("failed to decode psk.");
        }
        datas.reserve(metadata.max_length);
        length.GetIPCSharedMemory(metadata.lenname, 1);
        body.GetIPCSharedMemory(metadata.bodyname, metadata.max_length);
        hmac.GetIPCSharedMemory(metadata.hmacname, 32);
        locker.init(metadata.lockname);
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
    }
    PDJE_Input_Transfer(const Input_Transfer_Metadata &metad, const bool ishost)
    {
        metadata = metad;
        datas.reserve(metadata.max_length);
        if (ishost) {
            length.MakeIPCSharedMemory(metadata.lenname, 1);
            body.MakeIPCSharedMemory(metadata.bodyname, metadata.max_length);
            hmac.MakeIPCSharedMemory(metadata.hmacname, 32);
            locker.init(metadata.lockname);
            metadata.psk.Gen();
        } else {
            length.GetIPCSharedMemory(metadata.lenname, 1);
            body.GetIPCSharedMemory(metadata.bodyname, metadata.max_length);
            hmac.GetIPCSharedMemory(metadata.hmacname, 32);
            locker.init(metadata.lockname);
        }
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
    }
};

}; // namespace PDJE_IPC