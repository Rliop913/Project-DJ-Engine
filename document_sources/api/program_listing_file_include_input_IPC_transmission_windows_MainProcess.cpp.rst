
.. _program_listing_file_include_input_IPC_transmission_windows_MainProcess.cpp:

Program Listing for File MainProcess.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_transmission_windows_MainProcess.cpp>` (``include/input/IPC/transmission/windows/MainProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MainProcess.hpp"
   #include "ipc_util.hpp"
   namespace PDJE_IPC {
   
   static bool
   PDJE_OpenProcess(const fs::path &pt, Importants &imps, const int port)
   {
       imps.start_up_info    = STARTUPINFOW{};
       imps.process_info     = PROCESS_INFORMATION{};
       imps.start_up_info.cb = sizeof(imps.start_up_info);
       try {
   
           auto cmd = pt.wstring();
           cmd += L" ";
           cmd += std::to_wstring(port);
           BOOL ok = CreateProcessW(nullptr,
                                    cmd.data(),
                                    nullptr,
                                    nullptr,
                                    FALSE,
                                    CREATE_NO_WINDOW,
                                    nullptr,
                                    nullptr,
                                    &imps.start_up_info,
                                    &imps.process_info);
   
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
   
   MainProcess::~MainProcess()
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
   
   MainProcess::MainProcess(const int port)
   {
   
       auto path = GetValidProcessExecutor();
       if (!PDJE_OpenProcess(path, imp, port)) {
           critlog("failed to open child process. Err:");
           critlog(GetLastError());
           return;
       }
       cli.emplace("127.0.0.1", port);
       cli->set_connection_timeout(0, 200'000); // 200ms
       cli->set_read_timeout(0, 200'000);
       cli->set_write_timeout(0, 200'000);
       while (true) {
           if (auto res = cli->Get("/health"); res && res->status == 200) {
               break;
           }
           std::this_thread::sleep_for(std::chrono::milliseconds(200));
       }
   }
   bool
   MainProcess::EndTransmission()
   {
       auto res = cli->Get("/stop");
       if (res) {
           return true;
       } else {
           return false;
       }
   }
   
   }; // namespace PDJE_IPC
