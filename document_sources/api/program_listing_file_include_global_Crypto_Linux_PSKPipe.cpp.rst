
.. _program_listing_file_include_global_Crypto_Linux_PSKPipe.cpp:

Program Listing for File PSKPipe.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_Linux_PSKPipe.cpp>` (``include/global/Crypto/Linux/PSKPipe.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PSKPipe.hpp"
   
   // PSKPIPE is not used in linux.
   namespace PDJE_CRYPTO {
   
   static std::string
   GetTokenFromSTDPipe()
   {
       return {};
   }
   
   void *
   PSKPipe::Gen()
   {
       return nullptr;
   }
   void
   PSKPipe::Send(const std::string &msg)
   {
       return;
   }
   PSKPipe::~PSKPipe()
   {
       return;
   }
   }; // namespace PDJE_CRYPTO
