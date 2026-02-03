
.. _program_listing_file_include_global_Crypto_PDJE_Crypto_Token.cpp:

Program Listing for File PDJE_Crypto_Token.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_PDJE_Crypto_Token.cpp>` (``include/global/Crypto/PDJE_Crypto_Token.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Crypto.hpp"
   #include <botan/auto_rng.h>
   #include <botan/cipher_mode.h>
   #include <botan/hex.h>
   namespace PDJE_CRYPTO {
   
   PSK::PSK()
   {
       startlog();
   }
   
   bool
   PSK::Gen(const std::string &algo)
   {
       try {
   
           Botan::AutoSeeded_RNG rng;
           auto                  enc = Botan::Cipher_Mode::create_or_throw(
               algo, Botan::Cipher_Dir::Encryption);
           psk.resize(enc->maximum_keylength());
           rng.randomize(psk.data(), psk.size());
           return true;
       } catch (const std::exception &e) {
           critlog("failed to generate psk. why: ");
           critlog(e.what());
           return false;
       }
   }
   
   std::string
   PSK::Encode()
   {
       try {
   
           return Botan::hex_encode(psk);
       } catch (const std::exception &e) {
           critlog("failed to encode psk. Why: ");
           critlog(e.what());
           return {};
       }
   }
   
   bool
   PSK::Decode(const std::string &hex)
   {
       try {
   
           psk = Botan::hex_decode_locked(hex);
           return true;
       } catch (const std::exception &e) {
           critlog("failed to decode psk. Why: ");
           critlog(e.what());
           return false;
       }
   }
   
   }; // namespace PDJE_CRYPTO
