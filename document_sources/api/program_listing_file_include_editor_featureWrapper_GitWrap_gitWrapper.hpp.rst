
.. _program_listing_file_include_editor_featureWrapper_GitWrap_gitWrapper.hpp:

Program Listing for File gitWrapper.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_gitWrapper.hpp>` (``include\editor\featureWrapper\GitWrap\gitWrapper.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include <string>
   #include <optional>
   
   #include <git2.h>
   #include <nlohmann/json.hpp>
   
   #include "BlameController.hpp"
   #include "DiffController.hpp"
   #include "AddController.hpp"
   
   
   #include "editorBranch.hpp"
   #include "git2/repository.h"
   #include "gitLog.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "fileNameSanitizer.hpp"
   
   using MAYBE_BLAME = std::optional<BlameController>;
   
   using BranchCommits = std::pair<std::string, std::vector<gitwrap::commit>>;
   // using SaveDatas = std::vector<BranchCommits>;
   
   using BRANCH_HANDLE = std::optional<gitwrap::branch>;
   
   class PDJE_API GitWrapper{
   private:
       git_signature* auth_sign = nullptr;
       std::optional<AddController> addIndex;
   public:
       git_repository* repo = nullptr;
       std::optional<gitwrap::logHandle> log_hdl;
       BRANCH_HANDLE handleBranch;
   
       bool add(const fs::path&  path);
       bool open(const fs::path&  path);
   
       DiffResult diff(
           const gitwrap::commit& oldCommit,
           const gitwrap::commit& newCommit);
   
       MAYBE_BLAME Blame(
           const fs::path& filepath,
           const gitwrap::commit& newCommit,
           const gitwrap::commit& oldCommit);
   
       bool commit(git_signature* sign, const std::string& message);
   
       bool log();
       bool log(const std::string& branchName);
   
       static std::string GenTimeStamp();
       // SaveDatas GetCommits();
   
   
       bool close();
   
       GitWrapper();
       ~GitWrapper();
   }; 
   
   
   
   class PDJE_API PDJE_GitHandler{
   private:
       git_signature* sign = nullptr;
   public:
       std::string RecentERR;
       GitWrapper gw;
   
       bool Save(const std::string& tracingFile, const std::string& timeStamp);
       bool Undo();
       bool Redo();
   
       
       bool Go(const std::string& branchName, git_oid* commitID);
       std::string GetLogWithJSONGraph();
       bool UpdateLog(){
           return gw.log();
       }
       bool UpdateLog(const std::string& branchName){
           return gw.log(branchName);
       }
   
       DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
   
       bool DeleteGIT(const fs::path& path);
       bool Open(const fs::path& path);
       bool Close();
       // SaveDatas GetCommits();
   
       PDJE_GitHandler() = delete;
       PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email);
       ~PDJE_GitHandler();
   
   };
   
