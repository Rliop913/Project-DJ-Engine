#pragma once
#include <vector>
#include <botan/aead.h>
#include <botan/mac.h>
#include <botan/hex.h>
#include <functional>
#include <map>
#include "PDJE_Crypto.hpp"
#include "ipc_shared_memory.hpp"
namespace PDJE_BUFFER{
using namespace PDJE_CRYPTO;

constexpr int MSG_MAX_SIZE = 2048;
constexpr int MAYBE_USABLE_MAX_SIZE = (MSG_MAX_SIZE / 2) - 54;//hex encode -> x2, nonce + json overhead -> apprx 54
using HEADER = uint8_t;
constexpr int BODY_SIZE = MAYBE_USABLE_MAX_SIZE - sizeof(HEADER);

using FEATURE = std::function<void(const char[])>;

class TX_RX{
private:
    AEAD aead;
    PDJE_IPC::SharedMem<char[MSG_MAX_SIZE], PDJE_IPC::PDJE_IPC_RW> WBuf;
    PDJE_IPC::SharedMem<char[MSG_MAX_SIZE], PDJE_IPC::PDJE_IPC_RW> RBuf;
    std::map<FEATURE> feature_map;
    void Listen();
public:
    TX_RX(PSK& key):(aead(key)){}
    void AddFunction(const HEADER header, FEATURE feature);
    bool Send(const HEADER header, const char[] body);
    
    ~TX_RX();
};

struct secured_buffer{
private:
    std::unique_ptr<Botan::MessageAuthenticationCode> hmac_engine;
    Botan::secure_vector<uint8_t> key;
    std::vector<uint8_t> data;
    secured_buffer():hmac_engine(Botan::MessageAuthenticationCode::create("HMAC(SHA-256)")){}
public:
    void hmac_calc(){
        hmac_engine->set_key(key.data(), key.size());
        hmac_engine->update(data.data(), data.size());
        auto res = hmac_engine->final_stdvec();
        //code samples.
    }
};
};

template <typename T> class PDJE_Secured_Transmission_Buffer {

};