#include "PDJE_Crypto.hpp"
#include "botan/hex.h"
#include <nlohmann/json.hpp>
namespace PDJE_CRYPTO{

    AEAD::AEAD(PSK& key, const std::string& algo)
    {
        startlog();
        try{

            enc = Botan::Cipher_Mode::create_or_throw(algo, Botan::Cipher_Dir::Encryption);
            dec = Botan::Cipher_Mode::create_or_throw(algo, Botan::Cipher_Dir::Decryption);
            
            enc->set_key(key.psk);
            dec->set_key(key.psk);
        }catch(const std::exception& e){
            critlog("failed to init AEAD. Why: ");
            critlog(e.what());
        }
    }

    std::string 
    AEAD::Encrypt(std::string& noncestr, const std::string& plaintxt)
    {
        try
        {
            Botan::secure_vector<uint8_t> nonce(enc->default_nonce_length());
            rng.randomize(nonce.data(), nonce.size());
            Botan::secure_vector<uint8_t> plain(plaintxt.data(), plaintxt.data()+plaintxt.length());

            enc->start(nonce);
            enc->finish(plain);
            noncestr = Botan::hex_encode(nonce);
            return Botan::hex_encode(plain);
        }
        catch(const std::exception& e)
        {
            critlog("failed to encrypt with AEAD. Why: ");
            critlog(e.what());
            return {};
        }
        
    }

    std::string 
    AEAD::Decrypt(const std::string& noncehex, const std::string& hexenc)
    {
        try
        {
            auto nonce = Botan::hex_decode(noncehex);
            Botan::secure_vector<uint8_t> nvec(nonce.begin(), nonce.end());
            auto enctemp = Botan::hex_decode(hexenc);
            Botan::secure_vector<uint8_t> enced(enctemp.begin(), enctemp.end());

            dec->start(nvec);
            dec->finish(enced);

            return std::string(enced.data(), enced.data() + enced.size());
        }
        catch(const std::exception& e)
        {
            critlog("failed to decrypt with AEAD. Why: ");
            critlog(e.what());
            return {};
        }
        
    }
    
    std::string 
    AEAD::EncryptAndPack(const std::string& plaintxt)
    {
        nlohmann::json pack;
        std::string nonce;
        auto enchex = Encrypt(nonce, plaintxt);
        pack["NONCE"] = nonce;
        pack["BODY"] = enchex;
        return pack.dump();
    }
    std::string 
    AEAD::UnpackAndDecrypt(const std::string& aead_json)
    {
        auto unpacked = nlohmann::json::parse(aead_json);
        std::string nonce = unpacked["NONCE"];
        std::string body = unpacked["BODY"];
        auto decTXT = Decrypt(nonce, body);
        return decTXT;
    }
};