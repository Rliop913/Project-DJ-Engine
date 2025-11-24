
.. _program_listing_file_include_core_editor_TimeLine_LineVersion_LineVersion.hpp:

Program Listing for File LineVersion.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_LineVersion_LineVersion.hpp>` (``include/core/editor/TimeLine/LineVersion/LineVersion.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "GitDatas.hpp"
   #include "GitRAII.hpp"
   #include <git2/commit.h>
   #include <git2/oid.h>
   namespace PDJE_TIMELINE {
   static void
   NewFirstLog(std::shared_ptr<GitData> &git, const OID &id)
   {
       git->log_tree[id] = "ROOT";
   }
   static void
   AddLog(std::shared_ptr<GitData> &git, const OID &id, const OID &parent)
   {
       git->log_tree[id] = parent;
   }
   static bool
   GetParent(const OID &id, OID &parent, std::shared_ptr<GitData> &git)
   {
       GIT_RAII::commit cm(id, git->GetRepo());
       auto             count = git_commit_parentcount(cm.p);
       if (count == 0) {
           return false;
       } else {
           parent = git_oid_tostr_s(git_commit_parent_id(cm.p, 0));
           return true;
       }
   }
   
   static void
   LogBranch(const OID &head, std::shared_ptr<GitData> &git)
   {
       OID parent;
       OID now = head;
       while (GetParent(now, parent, git)) {
           if (!git->log_tree.contains(now)) {
               AddLog(git, now, parent);
           } else {
               break;
           }
           now = parent;
       }
   }
   
   using TIME_STAMP = std::string;
   class BranchLine {
     private:
       std::shared_ptr<GitData> git;
   
     public:
       bool
       GetHead(git_oid &oid);
       bool
       CheckUnborn();
       void
       Born(git_index *idx);
       bool
       IsDetached();
       bool
       Diverge();
       bool
       AppendToHead(git_index *idx);
       std::vector<std::pair<OID, TIME_STAMP>>
       ListLines();
       BranchLine(const std::shared_ptr<GitData> &gitptr) : git(gitptr)
       {
       }
       ~BranchLine() = default;
   };
   }; // namespace PDJE_TIMELINE
