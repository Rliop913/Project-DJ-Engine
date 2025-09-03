
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Commit_editorCommit.cpp:

Program Listing for File editorCommit.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Commit_editorCommit.cpp>` (``include/editor/featureWrapper/GitWrap/Commit/editorCommit.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editorCommit.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <cstring>
   using namespace gitwrap;
   
   bool
   commitList::UpdateCommits(git_repository *repo)
   {
       clist.back();
       git_revwalk *walker = nullptr;
       if (git_revwalk_new(&walker, repo) != 0) {
           critlog(
               "failed to walk reverse from commitList UpdateCommits. gitLog: ");
           critlog(git_error_last()->message);
           return false;
       }
   
       if (git_revwalk_push_head(walker) != 0) {
           critlog("failed to revwalk push head from commitList UpdateCommits. "
                   "gitLog: ");
           critlog(git_error_last()->message);
           git_revwalk_free(walker);
           return false;
       }
   
       git_revwalk_sorting(walker, GIT_SORT_TIME);
   
       git_oid           tempid;
       std::list<commit> templist;
       while (git_revwalk_next(&tempid, walker) == 0) {
           if (OkToAdd(tempid)) {
               templist.emplace_front();
               templist.front().commitID = tempid;
               if (git_commit_lookup(&templist.front().commitPointer,
                                     repo,
                                     &templist.front().commitID) == 0) {
                   templist.front().msg = std::string(
                       git_commit_message(templist.front().commitPointer));
               }
           } else {
               clist.splice(clist.end(), templist);
               break;
           }
       }
       git_revwalk_free(walker);
       return true;
   }
   
   bool
   commitList::OkToAdd(git_oid id)
   {
       if (clist.empty()) {
           return true;
       } else if (git_oid_cmp(&clist.back().commitID, &id) != 0) {
           return true;
       } else {
           return false;
       }
   }
   
   commit::commit(git_oid oid, git_repository *rep) : commitID(oid)
   {
       if (git_commit_lookup(&commitPointer, rep, &commitID) == 0) {
           msg = git_commit_message(commitPointer);
       } else {
           critlog("failed to lookup commit pointer. from commit::commit(oid, "
                   "rep). gitLog: ");
           critlog(git_error_last()->message);
           commitPointer = nullptr;
       }
   }
   
   commit::commit(const std::string commitMSG, git_repository *rep)
       : msg(commitMSG)
   {
       git_revwalk *walker = nullptr;
       git_revwalk_new(&walker, rep);
       git_revwalk_push_head(walker);
   
       git_oid tempoid;
       while (git_revwalk_next(&tempoid, walker) == 0) {
           git_commit_lookup(&commitPointer, rep, &tempoid);
           if (strcmp(git_commit_message(commitPointer), msg.c_str()) == 0) {
               commitID = tempoid;
               break;
           } else {
               critlog(
                   "something failed. from commit::commit(msg, rep). gitLog: ");
               critlog(git_error_last()->message);
               git_commit_free(commitPointer);
               commitPointer = nullptr;
           }
       }
       git_revwalk_free(walker);
   }
