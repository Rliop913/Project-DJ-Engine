
.. _program_listing_file_include_input_trashbin_MainProcess_windows.cpp:

Program Listing for File MainProcess_windows.cpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_MainProcess_windows.cpp>` (``include/input/trashbin/MainProcess_windows.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MainProcess.hpp"
   #include "PSKPipe.hpp"
   #include "ipc_util.hpp"
   namespace PDJE_IPC {
   using namespace MAINPROC;
   static bool
   PDJE_OpenProcess(const fs::path       &pt,
                    Importants           &imps,
                    PDJE_CRYPTO::PSKPipe &pipe)
   {
       imps.start_up_info    = STARTUPINFOW{};
       imps.process_info     = PROCESS_INFORMATION{};
       imps.start_up_info.cb = sizeof(imps.start_up_info);
       try {
           HANDLE FileLocker =
               CreateFileW(pt.wstring().c_str(),
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           nullptr,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                           nullptr);
           if (FileLocker == INVALID_HANDLE_VALUE) {
               critlog("failed to lock subprocess exe file");
               return false;
           }
           if (!PDJE_IPC::HashCompare(pt)) {
               CloseHandle(FileLocker);
               critlog("hash not matched. maybe Under Attack.");
               return false;
           }
           HANDLE readHdl                = pipe.Gen();
           imps.start_up_info.dwFlags    = STARTF_USESTDHANDLES;
           imps.start_up_info.hStdInput  = readHdl;
           imps.start_up_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
           imps.start_up_info.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
           auto cmd                      = pt.wstring();
           BOOL ok                       = CreateProcessW(nullptr,
                                    cmd.data(),
                                    nullptr,
                                    nullptr,
                                    TRUE,
                                    CREATE_NO_WINDOW,
                                    nullptr,
                                    nullptr,
                                    &imps.start_up_info,
                                    &imps.process_info);
   
           CloseHandle(FileLocker);
           CloseHandle(readHdl);
           if (!ok) {
               critlog("failed to create child process. Err:");
               critlog(GetLastError());
               return false;
           }
       } catch (const std::exception &e) {
           critlog("exception on creating child process. Err:");
           critlog(e.what());
           return false;
       }
       return true;
   }
   
   TXRXTransport::~TXRXTransport()
   {
       WaitForSingleObject(imp.process_info.hProcess, INFINITE);
       DWORD exitCode = 0;
       GetExitCodeProcess(imp.process_info.hProcess, &exitCode);
       if (exitCode != 0) {
           critlog("child process exit code is not zero. ErrCode: ");
           critlog(exitCode);
           return;
       }
       CloseHandle(imp.process_info.hThread);
       CloseHandle(imp.process_info.hProcess);
   }
   
   TXRXTransport::TXRXTransport()
   {
   
       auto path = GetValidProcessExecutor();
       auto pipe = PDJE_CRYPTO::PSKPipe();
   
       if (!PDJE_OpenProcess(path, imp, pipe)) {
           critlog("failed to open child process. Err:");
           critlog(GetLastError());
           return;
       }
       if (!psk.Gen()) {
           return;
       }
       PDJE_CRYPTO::RANDOM_GEN rg;
       PDJE_IPC::MNAME         mfirst  = rg.Gen("PDJE_TXRX_F_");
       PDJE_IPC::MNAME         lfirst  = rg.Gen("PDJE_TXRX_LOCK_F_");
       PDJE_IPC::MNAME         msecond = rg.Gen("PDJE_TXRX_S_");
       PDJE_IPC::MNAME         lsecond = rg.Gen("PDJE_TXRX_LOCK_S_");
   
       txrx.emplace(psk, mfirst, lfirst, msecond, lsecond, true);
       SetTXRX_Features();
       std::stringstream ss;
   
       ss << psk.Encode();
       ss << " ";
       ss << mfirst.string();
       ss << " ";
       ss << lfirst.string();
       ss << " ";
       ss << msecond.string();
       ss << " ";
       ss << lsecond.string();
       pipe.Send(ss.str());
       txrx->Listen();
       if (!CheckHealth()) {
           critlog("Check Health Failed on MainProcess init.");
       }
   }
   bool
   TXRXTransport::EndTransmission()
   {
       TXRX_RESPONSE.STOP.emplace();
       auto resp = TXRX_RESPONSE.STOP->get_future();
       bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_STOP, "");
       if (res) {
           res = resp.get();
       }
       TXRX_RESPONSE.STOP.reset();
       txrx.reset();
       return res;
   }
   
   bool
   TXRXTransport::SendInputTransfer(PDJE_Input_Transfer &trsf)
   {
   
       try {
           TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM.emplace();
           auto resp = TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM->get_future();
           bool res =
               txrx->Send(PDJE_CRYPTO::TXRXHEADER::SEND_INPUT_TRANSFER_SHMEM,
                          trsf.GetMetaDatas());
   
           if (res) {
               res = resp.get();
           }
   
           TXRX_RESPONSE.SEND_INPUT_TRANSFER_SHMEM.reset();
           if (res) {
               return true;
           } else {
               critlog("failed to send ipc shared memory.");
               return false;
           }
       } catch (const std::exception &e) {
           critlog("failed to send ipc shared memory. Why:");
           critlog(e.what());
           return false;
       }
   }
   
   }; // namespace PDJE_IPC
