
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Diff_DiffController.cpp:

Program Listing for File DiffController.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Diff_DiffController.cpp>` (``include/editor/featureWrapper/GitWrap/Diff/DiffController.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "DiffController.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   DiffController::DiffController()
   {
   }
   
   bool
   DiffController::CommitToNow(git_repository *repo, git_oid oldID)
   {
       git_tree *Otree   = nullptr;
       auto      OCommit = gitwrap::commit(oldID, repo);
   
       if (OCommit.commitPointer != nullptr) {
           if (git_commit_tree(&Otree, OCommit.commitPointer) != 0) {
               critlog("failed to init tree. from DiffController CommitToNow. "
                       "gitLog: ");
               critlog(git_error_last()->message);
               goto OLD_TREE_INIT_FAILED;
           }
           if (git_diff_tree_to_workdir(&Dobj, repo, Otree, nullptr) != 0) {
               critlog("diff failed. from DiffController CommitToNow. gitLog: ");
               critlog(git_error_last()->message);
               goto DIFF_FAILED;
           }
       } else {
           critlog(
               "failed to init commit. from DiffController CommitToNow. gitLog: ");
           critlog(git_error_last()->message);
           goto COMMIT_INIT_FAILED;
       }
   
       git_tree_free(Otree);
       return true;
   
   // FOR ERROR
   DIFF_FAILED:
       git_tree_free(Otree);
   OLD_TREE_INIT_FAILED:
   COMMIT_INIT_FAILED:
       return false;
   }
   
   bool
   DiffController::CommitToCommit(git_repository *repo,
                                  git_oid         newID,
                                  git_oid         oldID)
   {
       git_tree *Ntree   = nullptr;
       git_tree *Otree   = nullptr;
       auto      NCommit = gitwrap::commit(newID, repo);
       auto      OCommit = gitwrap::commit(oldID, repo);
       if (NCommit.commitPointer != nullptr && OCommit.commitPointer != nullptr) {
   
           if (git_commit_tree(&Ntree, NCommit.commitPointer) != 0) {
               critlog("failed to init new tree. from DiffController "
                       "CommitToCommit. gitLog: ");
               critlog(git_error_last()->message);
               goto NEW_TREE_INIT_FAILED;
           }
           if (git_commit_tree(&Otree, OCommit.commitPointer) != 0) {
               critlog("failed to init old tree. from DiffController "
                       "CommitToCommit. gitLog: ");
               critlog(git_error_last()->message);
               goto OLD_TREE_INIT_FAILED;
           }
           if (git_diff_tree_to_tree(&Dobj, repo, Otree, Ntree, nullptr) != 0) {
               critlog(
                   "failed to diff. from DiffController CommitToCommit. gitLog: ");
               critlog(git_error_last()->message);
               goto DIFF_FAILED;
           }
       } else {
           critlog("failed to init commit. from DiffController CommitToCommit. "
                   "gitLog: ");
           critlog(git_error_last()->message);
           goto COMMIT_INIT_FAILED;
       }
   
       git_tree_free(Ntree);
       git_tree_free(Otree);
       return true;
   
   // FOR ERROR
   DIFF_FAILED:
       git_tree_free(Otree);
   OLD_TREE_INIT_FAILED:
       git_tree_free(Ntree);
   NEW_TREE_INIT_FAILED:
   COMMIT_INIT_FAILED:
       return false;
   }
   
   int
   DiffCallback(const git_diff_delta *delta,
                const git_diff_hunk  *hunk,
                const git_diff_line  *line,
                void                 *payload)
   {
       auto res = reinterpret_cast<DiffResult *>(payload);
       switch (line->origin) {
       case '+': {
           lineLog log;
           log.linenumber = line->new_lineno;
           log.text       = line->content;
           res->NewLines.push_back(log);
           break;
       }
       case '-': {
           lineLog deleteLog;
           deleteLog.linenumber = line->old_lineno;
           deleteLog.text       = line->content;
           res->OldLines.push_back(deleteLog);
           break;
       }
       default:
           infolog("discarded case. from DiffController.cpp DiffCallback.");
           break;
       };
       return 0;
   }
   
   bool
   DiffController::execute(DiffResult *res)
   {
       bool diffRes = git_diff_foreach(Dobj,
                                       nullptr,
                                       nullptr,
                                       nullptr,
                                       DiffCallback,
                                       reinterpret_cast<void *>(res)) == 0;
   
       if (!diffRes) {
           critlog("failed to diff. from DiffController execute. gitLog: ");
           critlog(git_error_last()->message);
       }
       return diffRes;
   }
   
   DiffController::~DiffController()
   {
       if (Dobj != nullptr) {
           git_diff_free(Dobj);
       }
   }
