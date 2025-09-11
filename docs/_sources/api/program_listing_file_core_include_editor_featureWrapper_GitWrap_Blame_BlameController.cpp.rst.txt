
.. _program_listing_file_core_include_editor_featureWrapper_GitWrap_Blame_BlameController.cpp:

Program Listing for File BlameController.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_editor_featureWrapper_GitWrap_Blame_BlameController.cpp>` (``core_include\editor\featureWrapper\GitWrap\Blame\BlameController.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "BlameController.hpp"
   
   #include "PDJE_LOG_SETTER.hpp"
   
   bool
   BlameController::BlameOpen(git_repository    *repo,
                              const fs::path    &path,
                              git_blame_options *options)
   {
       std::string safeStr = path.generic_string();
       if (blame != nullptr) {
           critlog(
               "blame is null. tried from BlameController BlameOpen. Errpath: ");
           critlog(path.generic_string());
           return false;
       }
       if (git_blame_file(&blame, repo, safeStr.c_str(), options) != 0) {
           critlog("failed to blame file. from BlameController BlameOpen. Errpath "
                   "& Errmsg : ");
           critlog(path.generic_string());
           critlog(git_error_last()->message);
           return false;
       }
       blameAmount = git_blame_get_hunk_count(blame);
   
       return true;
   }
   
   MAYBE_BLAME_RESULT
   BlameController::
   operator[](unsigned int idx)
   {
       if (idx >= blameAmount) {
           warnlog("index out of range. from BlameController op[]");
           return std::nullopt;
       }
       auto temphunk = git_blame_get_hunk_byindex(blame, idx);
       if (temphunk) {
           BlameResult tempres;
           tempres.commitID        = temphunk->final_commit_id;
           tempres.editStartLine   = temphunk->final_start_line_number;
           tempres.editedLines     = temphunk->lines_in_hunk;
           tempres.originID        = temphunk->orig_commit_id;
           tempres.originStartLine = temphunk->orig_start_line_number;
           return tempres;
       } else {
           critlog("failed to get hunk. from BlameController op []. gitLog: ");
           critlog(git_error_last()->message);
           return std::nullopt;
       }
   }
   
   BlameController::~BlameController()
   {
       if (blame != nullptr) {
           git_blame_free(blame);
       }
   }
