#pragma once
#include "PDJE_Crypto.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <botan/aead.h>
#include <botan/hex.h>
#include <botan/mac.h>
#include <functional>
#include <map>
#include <thread>
#include <vector>

namespace PDJE_BUFFER {
using namespace PDJE_CRYPTO;

constexpr int MSG_MAX_SIZE = 4096;
constexpr int MAYBE_USABLE_MAX_SIZE =
    (MSG_MAX_SIZE / 2) -
    54; // hex encode -> x2, nonce + json overhead -> apprx 54
using HEADER           = uint8_t;
using ENCRYPT_RES_SIZE = uint16_t;
constexpr int BODY_SIZE =
    MAYBE_USABLE_MAX_SIZE - sizeof(HEADER) - sizeof(ENCRYPT_RES_SIZE);
constexpr int ENCRYPT_MAX_SIZE =
    MSG_MAX_SIZE - sizeof(HEADER) - sizeof(ENCRYPT_RES_SIZE);

using FEATURE = std::function<void(const std::string &)>;

class TX_RX {
  private:
    AEAD                                             aead;
    PDJE_IPC::SharedMem<char, PDJE_IPC::PDJE_IPC_RW> TXBuf;
    PDJE_IPC::SharedMem<char, PDJE_IPC::PDJE_IPC_RW> RXBuf;
    PDJE_IPC::MUTEX                                  TXM;
    PDJE_IPC::MUTEX                                  RXM;
    std::map<HEADER, FEATURE>                        feature_map;
    std::optional<std::thread>                       listen_worker;
    std::atomic<bool>                                worker_switch{ false };

    void
    Listen();

  public:
    TX_RX(PSK                   &key,
          const fs::path        &memFirst,
          const PDJE_IPC::MNAME &firstLock,
          const fs::path        &memSecond,
          const PDJE_IPC::MNAME &secondLock,
          const bool             isHost)
        : aead(key)
    {
        try {
            startlog();
            if (isHost) {
                TXBuf.MakeIPCSharedMemory(memFirst, MSG_MAX_SIZE);
                RXBuf.MakeIPCSharedMemory(memSecond, MSG_MAX_SIZE);
                std::memset(TXBuf.ptr, 0, MSG_MAX_SIZE);
                std::memset(RXBuf.ptr, 0, MSG_MAX_SIZE);

                TXM.init(firstLock);
                RXM.init(secondLock);
            } else {
                TXBuf.GetIPCSharedMemory(memSecond, MSG_MAX_SIZE);
                RXBuf.GetIPCSharedMemory(memFirst, MSG_MAX_SIZE);
                TXM.init(secondLock);
                RXM.init(firstLock);
            }
        } catch (const std::exception &e) {
            critlog("failed to init TX_RX shared memory. What: ");
            critlog(e.what());
        }
    }

    void
    AddFunction(const HEADER header, FEATURE feature);
    bool
    Send(const HEADER header, const std::string &body);

    ~TX_RX();
};

struct secured_buffer {
  private:
    std::unique_ptr<Botan::MessageAuthenticationCode> hmac_engine;
    Botan::secure_vector<uint8_t>                     key;
    std::vector<uint8_t>                              data;
    secured_buffer()
        : hmac_engine(Botan::MessageAuthenticationCode::create("HMAC(SHA-256)"))
    {
    }

  public:
    void
    hmac_calc()
    {
        hmac_engine->set_key(key.data(), key.size());
        hmac_engine->update(data.data(), data.size());
        auto res = hmac_engine->final_stdvec();
        // code samples.
    }
};
}; // namespace PDJE_BUFFER

template <typename T> class PDJE_Secured_Transmission_Buffer {};