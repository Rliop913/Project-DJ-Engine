
.. _program_listing_file_include_core_editor_TimeLine_EventMarker_EventMarker.hpp:

Program Listing for File EventMarker.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_EventMarker_EventMarker.hpp>` (``include\core\editor\TimeLine\EventMarker\EventMarker.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "GitDatas.hpp"
   #include "GitRAII.hpp"
   #include "LineVersion.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "jsonWrapper.hpp"
   #include <git2/checkout.h>
   #include <git2/commit.h>
   #include <git2/errors.h>
   #include <git2/oid.h>
   #include <git2/refs.h>
   #include <git2/repository.h>
   #include <git2/types.h>
   
   namespace PDJE_TIMELINE {
   
   template <typename CapnpType> class EventMarker {
     private:
       git_checkout_options     opts = GIT_CHECKOUT_OPTIONS_INIT;
       std::shared_ptr<GitData> git;
       OID                      checkedOut;
       git_index               *index;
       bool                     isActivate = true;
   
       bool
       Add()
       {
           if (!Activate()) {
               return false;
           }
           if (git_index_add_bypath(index, git->target_file.c_str()) != 0) {
               critlog("failed to add file. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
           if (git_index_write(index) != 0) {
               critlog("failed to write indexed. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
           return true;
       }
   
     public:
       BranchLine                  line;
       PDJE_JSONHandler<CapnpType> file_handle;
       bool
       Append()
       {
           if (!file_handle.save(git->root / git->target_file)) {
               return false;
           }
           if (!Add()) {
               critlog("failed to append file.");
               return false;
           }
           if (!line.IsDetached()) {
               line.Diverge();
           }
           if (!line.AppendToHead(index)) {
               return false;
           }
           git->LIFO_log.clear();
           return true;
       }
       bool
       MoveFront()
       {
           if (git->LIFO_log.empty()) {
               return false;
           } else {
               bool res = Move(git->LIFO_log.back());
               git->LIFO_log.pop_back();
               return res;
           }
       }
       bool
       MoveBack()
       {
           git->LIFO_log.push_back(checkedOut);
           if (git->log_tree.contains(checkedOut)) {
               return Move(git->log_tree[checkedOut]);
           } else {
               return false;
           }
       }
       bool
       Move(const OID &target)
       {
           git_oid target_oid{};
           if (target == "ROOT") {
               return false;
           }
           if (git_oid_fromstr(&target_oid, target.c_str()) != 0) {
               critlog("failed to get git oid from string. while moving event "
                       "marker. OID & GitErr: ");
               critlog(target);
               critlog(git_error_last()->message);
               return false;
           }
           if (git_repository_set_head_detached(git->GetRepo(), &target_oid) !=
               0) {
               critlog("failed to set head deteched. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
           return Activate();
       }
       bool
       Activate()
       {
           GIT_RAII::ref head;
           auto          err = git_repository_head(&head.p, git->GetRepo());
           if (err == GIT_EUNBORNBRANCH) {
               return true;
           }
           if (err < 0) {
               critlog("failed to get repo head. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
           git_oid head_oid{};
           if (!line.GetHead(head_oid)) {
               return false;
           }
   
           OID headid = git_oid_tostr_s(&head_oid);
           if (headid == checkedOut) {
               return true;
           }
           auto           now = GIT_RAII::commit(headid, git->GetRepo());
           GIT_RAII::tree tree;
           if (git_commit_tree(&tree.p, now.p) != 0) {
               critlog("failed to init commit tree while activate. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
           if (git_checkout_tree(git->GetRepo(),
                                 reinterpret_cast<git_object *>(tree.p),
                                 &opts) != 0) {
               critlog("failed to checkout. GitErr: ");
               critlog(git_error_last()->message);
               return false;
           }
   
           checkedOut = headid;
           return true;
       }
       EventMarker(const std::shared_ptr<GitData> &git_ptr)
           : git(git_ptr), line(git_ptr)
       {
           try {
               if (git_repository_index(&index, git->GetRepo()) != 0) {
                   critlog("failed to open git index. GitErr: ");
                   critlog(git_error_last()->message);
                   return;
               }
               file_handle.load(git->root / git->target_file);
               if (line.CheckUnborn()) {
                   if (!Add()) {
                       return;
                   }
                   line.Born(index);
               }
               opts.checkout_strategy = GIT_CHECKOUT_FORCE;
               auto Branches          = line.ListLines();
               for (const auto &b : Branches) {
                   LogBranch(b.first, git);
               }
               if (!Activate()) {
                   critlog("failed to Activate head commit.");
               }
           } catch (const std::exception &e) {
               critlog("failed to init event marker. What: ");
               critlog(e.what());
           }
       }
       ~EventMarker()
       {
           if (index) {
               git_index_free(index);
           }
       }
   };
   }; // namespace PDJE_TIMELINE
