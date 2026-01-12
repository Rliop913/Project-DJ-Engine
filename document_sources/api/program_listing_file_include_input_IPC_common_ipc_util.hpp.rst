
.. _program_listing_file_include_input_IPC_common_ipc_util.hpp:

Program Listing for File ipc_util.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_common_ipc_util.hpp>` (``include\input\IPC\common\ipc_util.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Crypto.hpp"
   #include "PDJE_INPUT_PROCESS_HASH.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "ipc_shared_memory.hpp"
   #include <algorithm>
   #include <cctype>
   #include <cstddef>
   #include <filesystem>
   #include <fstream>
   #include <ios>
   
   #include <string>
   #include <vector>
   
   namespace PDJE_IPC {
   namespace fs = std::filesystem;
   
   static inline bool
   HashCompare(const fs::path &pt)
   {
       try {
           auto lower = [](std::string s) {
               std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
                   return std::tolower(c);
               });
               return s;
           };
           auto target_hash = lower(EMBEDDED_INPUT_PROCESS_SHA256);
           if (!fs::is_regular_file(pt)) {
               critlog(
                   "file is not regular file. hash compare failed. filename: ");
               critlog(pt.string());
               return false;
           }
           auto        hasher    = PDJE_CRYPTO::Hash();
           std::string file_hash = hasher.FileHash(pt);
           if (lower(file_hash) == target_hash) {
               return true;
           } else {
               critlog("hash not matched. filename & hash: ");
               critlog(pt.string());
               critlog(file_hash);
               return false;
           }
       } catch (const std::exception &e) {
           critlog("hashcompare failed. filename & Why: ");
           critlog(pt.string());
           critlog(e.what());
           return false;
       }
   }
   
   static inline fs::path
   GetValidProcessExecutor()
   {
       try {
           fs::path current = fs::current_path();
           auto     lower   = [](std::string s) {
               std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
                   return std::tolower(c);
               });
               return s;
           };
           auto target_hash = lower(EMBEDDED_INPUT_PROCESS_SHA256);
           auto hasher      = PDJE_CRYPTO::Hash();
           for (auto &fp : fs::recursive_directory_iterator(
                    current, fs::directory_options::skip_permission_denied)) {
               if (fs::is_regular_file(fp) &&
                   fp.path().filename().string().find(
                       "PDJE_MODULE_INPUT_PROCESS") != std::string::npos) {
   
                   auto file_hash = hasher.FileHash(fp.path());
                   if (lower(file_hash) == target_hash) {
                       return fp.path();
                   }
               }
           }
   
           return {};
       } catch (const std::exception &e) {
           critlog("failed to get valid process executor. Why: ");
           critlog(e.what());
           return {};
       }
   }
   
   }; // namespace PDJE_IPC
