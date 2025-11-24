
.. _program_listing_file_include_core_editor_TimeLine_GitDataPack_GitDatas.hpp:

Program Listing for File GitDatas.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_GitDataPack_GitDatas.hpp>` (``include/core/editor/TimeLine/GitDataPack/GitDatas.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_LOG_SETTER.hpp"
   #include "jsonWrapper.hpp"
   #include <filesystem>
   #include <git2.h>
   #include <git2/errors.h>
   #include <git2/global.h>
   #include <unordered_map>
   namespace PDJE_TIMELINE {
   namespace fs = std::filesystem;
   
   using OID      = std::string;
   using BACK_OID = std::string;
   static std::string
   GenTimeStamp()
   {
       using namespace std::chrono;
   
       auto now = system_clock::now();
   
       std::time_t tt = system_clock::to_time_t(now);
       std::tm     tm{};
   #if defined(_WIN32)
       localtime_s(&tm, &tt);
   #else
       localtime_r(&tt, &tm);
   #endif
   
       auto ms = duration_cast<milliseconds>(now - system_clock::from_time_t(tt))
                     .count();
   
       std::ostringstream oss;
       oss << std::put_time(&tm, "%Y-%m-%d--%H-%M-%S");
       oss << '.' << std::setw(3) << std::setfill('0') << ms;
   
       return oss.str();
   }
   struct GitData {
     private:
       git_repository *repo = nullptr;
       git_signature  *sign = nullptr;
   
     public:
       std::unordered_map<OID, BACK_OID> log_tree;
       std::vector<OID>                  LIFO_log;
       fs::path                          root;
       std::string                       target_file;
       git_signature *
       GetSign()
       {
           if (sign) {
               return sign;
           } else {
               critlog("git signature is nullptr");
               return nullptr;
           }
       }
       git_repository *
       GetRepo()
       {
           if (repo) {
               return repo;
           } else {
               critlog("repo is nullptr");
               return nullptr;
           }
       }
       GitData(const fs::path    &git_repo_root,
               const std::string &file_name,
               const std::string &auth_name,
               const std::string &auth_email)
       {
           git_libgit2_init();
           if (git_signature_now(&sign, auth_name.c_str(), auth_email.c_str()) !=
               0) {
               critlog("failed to init signature.");
               critlog(git_error_last()->message);
           }
           target_file      = file_name;
           root             = git_repo_root;
           auto string_path = git_repo_root.generic_string();
           if (git_repository_open(&repo, string_path.c_str()) != 0) {
               if (git_repository_init(&repo, string_path.c_str(), false) != 0) {
                   critlog("failed to init repository. git log: ");
                   critlog(git_error_last()->message);
                   return;
               }
           }
       }
       ~GitData()
       {
           if (!sign) {
               git_signature_free(sign);
           }
           if (!repo) {
               git_repository_free(repo);
           }
           git_libgit2_shutdown();
       }
   };
   }; // namespace PDJE_TIMELINE
