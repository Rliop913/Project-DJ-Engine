
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Log_gitLog.hpp:

Program Listing for File gitLog.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Log_gitLog.hpp>` (``include\editor\featureWrapper\GitWrap\Log\gitLog.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include <unordered_map>
   #include <string>
   #include <vector>
   #include <cstdint>
   #include <bit>
   #include "fileNameSanitizer.hpp"
   #include "editorBranch.hpp"
   #include "editorCommit.hpp"
   namespace gitwrap{
       struct log{
           commit now;
           git_oid parentID = {{0}};
           DONT_SANITIZE authName;
           DONT_SANITIZE authEmail;
       };
   
       struct BranchHead{
           DONT_SANITIZE BranchName;
           git_oid head = {{0}};
       };
       
       class logHandle{
       private:
           struct OID_HASHER{
               std::size_t operator()(const git_oid& oid) const noexcept;
           };
           struct OID_EQUAL{
               bool operator()(const git_oid& lhs, const git_oid& rhs) const noexcept{
                   return git_oid_equal(&lhs, &rhs);
               }
           };
           git_repository* repoPointer = nullptr;
           void AddLog(const git_oid& id, git_oid& ChildID, const git_signature* & sign, const std::string& msg);
           
       public:
           bool WalkBranch(const DONT_SANITIZE& branchName);
           std::unordered_map<git_oid, log, OID_HASHER, OID_EQUAL> logs;
           git_oid ROOT_ID = {{0}};
           std::vector<BranchHead> heads;
           logHandle(git_repository* repo) : repoPointer(repo) {}
       };
   };
