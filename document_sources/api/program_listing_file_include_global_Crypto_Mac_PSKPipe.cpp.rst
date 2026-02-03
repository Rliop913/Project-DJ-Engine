
.. _program_listing_file_include_global_Crypto_Mac_PSKPipe.cpp:

Program Listing for File PSKPipe.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_Mac_PSKPipe.cpp>` (``include/global/Crypto/Mac/PSKPipe.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PSKPipe.hpp"
   
   namespace PDJE_CRYPTO {
   
   static std::string
   GetTokenFromSTDPipe()
   {
   
       return {};
   }
   
   void *
   PSKPipe::Gen()
   {
       try {
           return nullptr;
   
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
           return;
       } catch (const std::exception &e) {
           critlog("failed to Send Token. Why: ");
           critlog(e.what());
       }
   }
   PSKPipe::~PSKPipe()
   {
   }
   }; // namespace PDJE_CRYPTO
