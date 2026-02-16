
.. _program_listing_file_include_core_editor_TimeLine_TimeLine.hpp:

Program Listing for File TimeLine.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_TimeLine.hpp>` (``include/core/editor/TimeLine/TimeLine.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "EventMarker.hpp"
   #include "GitDatas.hpp"
   #include "GitRAII.hpp"
   #include "LineVersion.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "jsonWrapper.hpp"
   #include <exception>
   #include <filesystem>
   #include <git2/commit.h>
   #include <string>
   namespace PDJE_TIMELINE {
   namespace fs = std::filesystem;
   template <typename CapnpType> class TimeLine {
     private:
       std::shared_ptr<GitData> git;
   
       EventMarker<CapnpType> mark;
   
       bool
       Save()
       {
           try {
               if (!mark.Activate()) {
                   return false;
               }
               return mark.Append();
           } catch (const std::exception &e) {
               critlog("failed to save. error occurred. What: ");
               critlog(e.what());
               return false;
           }
       }
   
     public:
       PDJE_JSONHandler<CapnpType> *
       GetJson()
       {
           return &mark.file_handle;
       }
       template <typename ArgType>
       bool
       WriteData(const ArgType &arg)
       {
           if (!mark.file_handle.add(arg)) {
               critlog("failed to write json data to file.");
               return false;
           }
           return Save();
       }
       template <typename ArgType>
       int
       DeleteData(const ArgType &arg)
       {
           int deleted = mark.file_handle.deleteLine(arg);
           if (!Save()) {
   
               return -1;
           }
           return deleted;
       }
       template <typename ArgType>
       int
       DeleteData(const ArgType &arg, const bool skipType, const bool skipDetail)
       {
           int deleted = mark.file_handle.deleteLine(arg, skipType, skipDetail);
           if (!Save()) {
               return -1;
           }
           return deleted;
       }
       bool
       Undo()
       {
           try {
               return mark.MoveBack();
           } catch (const std::exception &e) {
               critlog("failed to undo. error occurred. What: ");
               critlog(e.what());
               return false;
           }
       }
       bool
       Redo()
       {
           try {
               return mark.MoveFront();
           } catch (const std::exception &e) {
               critlog("failed to redo. error occurred. What: ");
               critlog(e.what());
               return false;
           }
       }
       bool
       Go(const std::string &OID)
       {
           try {
               if (git->log_tree.contains(OID)) {
                   return mark.Move(OID);
               } else {
                   return false;
               }
           } catch (const std::exception &e) {
               critlog("failed to Go. error occurred. What: ");
               critlog(e.what());
               return false;
           }
       }
       void
       Diff(const OID &origin, const OID &compare) // todo-impl
       {
       }
       void
       UpdateLogs()
       {
           try {
   
               auto Branches = mark.line.ListLines();
               for (const auto &b : Branches) {
                   LogBranch(b.first, git);
               }
           } catch (const std::exception &e) {
               critlog("failed to update logs. What: ");
               critlog(e.what());
           }
       }
       std::string
       GetLogs()
       {
           try {
               nj logRoot;
               logRoot["LINE"] = nj::array();
               logRoot["LOGS"] = nj::array();
               for (std::pair<OID, TIME_STAMP> &versions : mark.line.ListLines()) {
                   nj pairs;
                   pairs["OID"]        = versions.first;
                   pairs["TIME_STAMP"] = versions.second;
                   logRoot["LINE"].push_back(pairs);
               }
               for (const auto &logs : git->log_tree) {
                   nj pairs;
                   pairs["OID"]  = logs.first;
                   pairs["BACK"] = logs.second;
                   GIT_RAII::commit lookup(logs.first, git->GetRepo());
                   auto             auth_info = git_commit_author(lookup.p);
                   pairs["AUTHOR"]            = auth_info->name;
                   pairs["EMAIL"]             = auth_info->email;
                   pairs["TIME_STAMP"]        = git_commit_message(lookup.p);
                   logRoot["LOGS"].push_back(pairs);
               }
               return logRoot.dump();
           } catch (const std::exception &e) {
               critlog("failed to logging. What: ");
               critlog(e.what());
               return {};
           }
       }
   
       TimeLine(const fs::path    &git_repo_root,
                const std::string &file_name,
                const std::string &auth_name,
                const std::string &auth_email)
           : git(std::make_shared<GitData>(
                 git_repo_root, file_name, auth_name, auth_email)),
             mark(git)
       {
           startlog();
       }
       ~TimeLine() = default;
   };
   }; // namespace PDJE_TIMELINE
