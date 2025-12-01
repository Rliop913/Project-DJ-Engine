
.. _program_listing_file_include_global_Crypto_PDJE_Crypto.hpp:

Program Listing for File PDJE_Crypto.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_PDJE_Crypto.hpp>` (``include/global/Crypto/PDJE_Crypto.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_LOG_SETTER.hpp"
   #include <botan/auto_rng.h>
   #include <botan/cipher_mode.h>
   #include <botan/hash.h>
   #include <filesystem>
   #include <string>
   namespace PDJE_CRYPTO {
   namespace fs = std::filesystem;
   class Hash {
     private:
       std::unique_ptr<Botan::HashFunction> hashEngine;
   
     public:
       Hash();
       ~Hash() = default;
       std::string
       TextHash(const std::string &txt);
       std::string
       FileHash(const fs::path &fp);
   };
   
   class PSK {
     public:
       std::vector<uint8_t> psk;
       bool
       Gen(const std::string &algo = "AES-256/GCM");
   
       PSK();
       ~PSK() = default;
       std::string
       Encode();
       bool
       Decode(const std::string &hex);
   };
   
   class AEAD {
     private:
       Botan::AutoSeeded_RNG               rng;
       std::unique_ptr<Botan::Cipher_Mode> enc;
       std::unique_ptr<Botan::Cipher_Mode> dec;
   
     public:
       std::string
       Encrypt(std::string &noncestr, const std::string &plaintxt);
       std::string
       Decrypt(const std::string &nonce, const std::string &hexenc);
   
       std::string
       EncryptAndPack(const std::string &plaintxt);
       std::string
       UnpackAndDecrypt(const std::string &aead_json);
   
       AEAD(PSK &key, const std::string &algo = "AES-256/GCM");
       ~AEAD() = default;
   };
   
   }; // namespace PDJE_CRYPTO
