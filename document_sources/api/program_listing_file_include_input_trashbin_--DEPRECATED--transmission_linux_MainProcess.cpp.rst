
.. _program_listing_file_include_input_trashbin_--DEPRECATED--transmission_linux_MainProcess.cpp:

Program Listing for File MainProcess.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_--DEPRECATED--transmission_linux_MainProcess.cpp>` (``include/input/trashbin/--DEPRECATED--transmission/linux/MainProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MainProcess.hpp"
   #include "PDJE_INPUT_PROCESS_HASH.hpp"
   #include "httplib.h"
   #include "ipc_util.hpp"
   #include <chrono>
   #include <format>
   #include <iostream>
   #include <spawn.h>
   #include <string>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <thread>
   #include <unistd.h>
   namespace PDJE_IPC {
   static std::string
   GenCommand(const std::string &store_value,
              const std::string &command,
              const std::string &arg1,
              const std::string &arg2)
   {
       return store_value + "=$(" + command + " " + arg1 + " " + arg2 + ");";
   }
   static std::string
   GenIF(const std::string &A,
         const std::string &plain_text,
         const std::string &THEN)
   {
       return std::format("[[ \"${}\" == \"{}\" ]] &&", A, plain_text) + " { " +
              THEN + " };";
   }
   static std::string
   GenExecuteShell(const fs::path &exec_path, const int port)
   {
       std::string tmpBlob =
           "umask 077; tmp=$(mktemp /dev/shm/blob.XXXXXX) || exit 1;";
       tmpBlob +=
           "trap '[ -n \"${tmp:-}\" ] && { shred -u -- \"$tmp\" 2>/dev/null "
           "|| rm -f -- \"$tmp\"; }' EXIT;";
       std::string fillTMP = std::format("cat -- {} > \"$tmp\";",
                                         "\"" + exec_path.generic_string() + "\"");
       auto        command = GenCommand("exec_hash",
                                 "sha256sum --",
                                 ("\"$tmp\""),
                                 "| tr -s ' ' | cut -d ' ' -f1");
       std::string then    = "chmod 700 \"$tmp\";exec {fd}<\"$tmp\";rm "
                             "-f -- \"$tmp\";trap - EXIT; exec \"/proc/$$/fd/$fd\" " +
                          std::to_string(port) + " || exit 2; ";
       command += GenIF("exec_hash", EMBEDDED_INPUT_PROCESS_SHA256, then);
       return tmpBlob + fillTMP + command;
   }
   
   static bool
   OpenProcess(const fs::path &pt, pid_t *child_pid, const int port)
   {
       auto        bash  = "/bin/bash";
       std::string Shell = GenExecuteShell(pt, port);
       std::cout << Shell << std::endl;
       char *pkexec_args[] = { (char *)"pkexec",
                               (char *)bash,
                               (char *)"-c",
                               (char *)Shell.c_str(),
                               nullptr };
       char *sudo_args[]   = { (char *)"sudo",
                               (char *)bash,
                               (char *)"-c",
                               (char *)Shell.c_str(),
                               nullptr };
   
       if ((getenv("DISPLAY") || getenv("WAYLAND_DISPLAY")) &&
           access("/usr/bin/pkexec", X_OK) == 0) {
           int spawn_stat = posix_spawn(child_pid,
                                        "/usr/bin/pkexec",
                                        nullptr,
                                        nullptr,
                                        pkexec_args,
                                        environ);
           if (spawn_stat == 0) {
               return true;
           }
       } else {
           int spawn_stat = posix_spawn(
               child_pid, "/usr/bin/sudo", nullptr, nullptr, sudo_args, environ);
           if (spawn_stat == 0) {
               return true;
           }
       }
       return false;
   }
   
   TXRXTransport::TXRXTransport(const int port)
   {
       unlink(imp.socket_path.c_str());
       imp.socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
   
       sockaddr_un address_temp{};
       address_temp.sun_family = AF_UNIX;
   
       if (bind(imp.socket_fd,
                reinterpret_cast<sockaddr *>(&address_temp),
                sizeof(address_temp)) < 0) {
           critlog("failed to bind socket fd. errno:");
           critlog(errno);
   
           return;
       }
   
       if (listen(imp.socket_fd, 1) < 0) {
           critlog("failed to listen socket. errno:");
           critlog(errno);
           return;
       }
       auto path = GetValidProcessExecutor();
       if (!OpenProcess(path, &imp.child_pid, port)) {
           critlog("failed to open child process. errno:");
           critlog(errno);
           return;
       }
   
       // imp.child_fd = accept(imp.socket_fd, nullptr, nullptr);
       // if (imp.child_fd < 0) {
       //     critlog("failed to get child process fd. errno:");
       //     critlog(errno);
       //     return;
       // }
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
   
   TXRXTransport::~TXRXTransport()
   {
       if (imp.child_fd >= 0) {
           close(imp.child_fd);
       }
       if (imp.socket_fd >= 0) {
           close(imp.socket_fd);
       }
       unlink(imp.socket_path.c_str());
   }
   bool
   TXRXTransport::EndTransmission()
   {
       auto res = cli->Get("/stop");
       if (res) {
           return true;
       } else {
           return false;
       }
   }
   }; // namespace PDJE_IPC
