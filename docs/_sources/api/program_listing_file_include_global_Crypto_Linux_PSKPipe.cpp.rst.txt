
.. _program_listing_file_include_global_Crypto_Linux_PSKPipe.cpp:

Program Listing for File PSKPipe.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_Linux_PSKPipe.cpp>` (``include/global/Crypto/Linux/PSKPipe.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PSKPipe.hpp"
   
   namespace PDJE_CRYPTO {
   
   static std::string
   GetTokenFromSTDPipe()
   {
       return {}; // todo -- impl
   }
   
   void *
   PSKPipe::Gen() // todo -- Linux PSK Exchange: FD fixed socket + IPC shmem
                  // exchange
   {
       try {
   
           return nullptr; // todo -- impl
       } catch (const std::exception &e) {
           critlog("failed to generate token pipe. WHY: ");
           critlog(e.what());
           return nullptr;
       }
   }
   void
   PSKPipe::Send(const std::string &msg)
   {
       try {
           return; // todo -- impl
       } catch (const std::exception &e) {
           critlog("failed to Send Token. Why: ");
           critlog(e.what());
       }
   }
   PSKPipe::~PSKPipe()
   {
       // todo -- impl
   }
   }; // namespace PDJE_CRYPTO
