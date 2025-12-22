#pragma once
#include "Input_State.hpp"
#include "PDJE_Crypto.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <botan/mac.h>
#include <botan/mem_ops.h>
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
class PDJE_Input_Transfer {
  private:
    SharedMem<uint64_t, PDJE_IPC_RW>                  length;
    SharedMem<PDJE_Input_Log, PDJE_IPC_RW>            body;
    SharedMem<uint8_t, PDJE_IPC_RW>                   hmac;
    MUTEX                                             locker;
    PSK                                               psk;
    std::unique_ptr<Botan::MessageAuthenticationCode> hmacEngine;
    void
    SetHmacEngine()
    {
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
        if (!hmacEngine) {
            critlog("cannot create HMAC(SHA-256).");
            return;
        }
        hmacEngine->set_key(psk.psk);
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
    SetPSK(PSK &origin)
    {
        psk        = origin;
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
    }
    void
    GenPSK()
    {
        psk.Gen();
        hmacEngine = Botan::MessageAuthenticationCode::create("HMAC(SHA-256)");
    }
    void
    Send();
    void
    Receive();
    PDJE_Input_Transfer(const uint32_t max_length)
    {
        datas.reserve(max_length);
    }
    PDJE_Input_Transfer(const uint32_t max_length,
                        const MNAME   &lenname,
                        const MNAME   &bodyname,
                        const MNAME   &hmacname,
                        const MNAME   &lockname,
                        const bool     ishost)
    {
        datas.reserve(max_length);
        if (ishost) {
            length.MakeIPCSharedMemory(lenname, 1);
            body.MakeIPCSharedMemory(bodyname, max_length);
            hmac.MakeIPCSharedMemory(hmacname, 32);
            locker.init(lockname);
        } else {
            length.GetIPCSharedMemory(lenname, 1);
            body.GetIPCSharedMemory(bodyname, max_length);
            hmac.GetIPCSharedMemory(hmacname, 32);
            locker.init(lockname);
        }
    }
};

}; // namespace PDJE_IPC