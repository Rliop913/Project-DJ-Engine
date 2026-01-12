
.. _program_listing_file_include_global_Crypto_PSKPipe.hpp:

Program Listing for File PSKPipe.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_PSKPipe.hpp>` (``include\global\Crypto\PSKPipe.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_LOG_SETTER.hpp"
   #include <string>
   namespace PDJE_CRYPTO {
   
   class PSKPipe {
     private:
       void *writeHandle = nullptr;
   
     public:
       static std::string
       GetTokenFromSTDPipe();
       void *
       Gen();
       void
       Send(const std::string &msg);
   
       PSKPipe()
       {
           startlog();
       }
       ~PSKPipe();
   };
   }; // namespace PDJE_CRYPTO
