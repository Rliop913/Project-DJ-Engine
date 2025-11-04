
.. _program_listing_file_include_input_IPC_common_ipc_util.hpp:

Program Listing for File ipc_util.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_common_ipc_util.hpp>` (``include/input/IPC/common/ipc_util.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_INPUT_PROCESS_HASH.hpp"
   #include "ipc_shared_memory.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <algorithm>
   #include <cctype>
   #include <cstddef>
   #include <filesystem>
   #include <fstream>
   #include <ios>
   #include <picosha2.h>
   
   #include <string>
   #include <vector>
   
   namespace PDJE_IPC {
   namespace fs = std::filesystem;
   
   static inline fs::path
   GetValidProcessExecutor()
   {
       fs::path current = fs::current_path();
       auto     lower   = [](std::string s) {
           std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
               return std::tolower(c);
           });
           return s;
       };
       auto target_hash = lower(EMBEDDED_INPUT_PROCESS_SHA256);
       for (auto &fp : fs::recursive_directory_iterator(
                current, fs::directory_options::skip_permission_denied)) {
           if (fs::is_regular_file(fp) &&
               fp.path().filename().string().find("PDJE_MODULE_INPUT_PROCESS") !=
                   std::string::npos) {
   
               std::ifstream exfile(fp.path(), std::ios::binary);
               if (!exfile) {
                   continue;
               }
               std::vector<unsigned char> hash(picosha2::k_digest_size);
               picosha2::hash256(exfile, hash.begin(), hash.end());
               auto file_hash = picosha2::bytes_to_hex_string(hash);
               if (lower(file_hash) == target_hash) {
                   return fp.path();
               }
           }
       }
   
       return {};
   }
   
   }; // namespace PDJE_IPC_UTILS
