
.. _program_listing_file_include_global_Crypto_Windows_PSKPipe.cpp:

Program Listing for File PSKPipe.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Crypto_Windows_PSKPipe.cpp>` (``include\global\Crypto\Windows\PSKPipe.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PSKPipe.hpp"
   #include <Windows.h>
   namespace PDJE_CRYPTO {
   
   std::string
   PSKPipe::GetTokenFromSTDPipe()
   {
       HANDLE      stdhandle = GetStdHandle(STD_INPUT_HANDLE);
       char        buf[4096];
       std::string token;
   
       for (;;) {
           DWORD got = 0;
           BOOL  ok  = ReadFile(stdhandle, buf, sizeof(buf), &got, nullptr);
           if (!ok || got == 0) {
               break;
           }
           token.append(buf, buf + got);
       }
       return token;
   }
   
   void *
   PSKPipe::Gen()
   {
       try {
   
           SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
           void               *readHandle = nullptr;
           if (!CreatePipe(&readHandle, &writeHandle, &sa, 0)) {
               critlog("failed to create pipe. tokenpipe generate failed.");
               return nullptr;
           }
   
           SetHandleInformation(writeHandle, HANDLE_FLAG_INHERIT, 0);
   
           return readHandle;
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
           DWORD written = 0;
           if (!WriteFile(writeHandle,
                          msg.data(),
                          (DWORD)msg.size(),
                          &written,
                          nullptr)) {
               critlog("failed to write token to pipe.");
           }
           CloseHandle(writeHandle);
           writeHandle = nullptr;
       } catch (const std::exception &e) {
           critlog("failed to Send Token. Why: ");
           critlog(e.what());
       }
   }
   PSKPipe::~PSKPipe()
   {
       if (writeHandle != nullptr) {
           CloseHandle(writeHandle);
       }
   }
   }; // namespace PDJE_CRYPTO
