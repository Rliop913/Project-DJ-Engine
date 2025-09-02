
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Commit_editorCommit.hpp:

Program Listing for File editorCommit.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Commit_editorCommit.hpp>` (``include/editor/featureWrapper/GitWrap/Commit/editorCommit.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include <git2.h>
   
   #include <list>
   #include <optional>
   #include <string>
   #include <vector>
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   namespace gitwrap {
   struct PDJE_API commit {
       git_commit *commitPointer = nullptr; 
       git_oid     commitID;                
       std::string msg;                     
       commit() = default;
       commit(git_oid commitID, git_repository *rep);
       commit(const std::string commitMSG, git_repository *rep);
       ~commit()
       {
           if (commitPointer != nullptr) {
               git_commit_free(commitPointer);
           }
       }
   };
   struct PDJE_API commitList {
   
       std::list<commit> clist; // Back is Newers
   
       void
       Reset()
       {
           clist.clear();
       }
       bool
       UpdateCommits(git_repository *repo);
       bool
       OkToAdd(git_oid id);
   };
   
   } // namespace gitwrap
