
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Blame_BlameController.cpp:

Program Listing for File BlameController.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Blame_BlameController.cpp>` (``include/editor/featureWrapper/GitWrap/Blame/BlameController.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "BlameController.hpp"
   
   
   
   bool
   BlameController::BlameOpen(
       git_repository *repo, 
       const std::string& path, 
       git_blame_options *options)
   {
       if(blame != nullptr){
           return false;
       }
       if(git_blame_file(&blame, repo, path.c_str(), options) != 0){
           return false;
       }
       blameAmount = git_blame_get_hunk_count(blame);
   
       return true;
   
   }
   
   MAYBE_BLAME_RESULT
   BlameController::operator[](unsigned int idx)
   {
       if(idx >= blameAmount){
           return std::nullopt;
       }
       auto temphunk = git_blame_get_hunk_byindex(blame, idx);
       if(temphunk){
           BlameResult tempres;
           tempres.commitID        = temphunk->final_commit_id         ;
           tempres.editStartLine   = temphunk->final_start_line_number ;
           tempres.editedLines     = temphunk->lines_in_hunk           ;
           tempres.originID        = temphunk->orig_commit_id          ;
           tempres.originStartLine = temphunk->orig_start_line_number  ;
           return tempres;
       }
       else{
           return std::nullopt;
       }
       
   }
   
   BlameController::~BlameController()
   {
       if(blame != nullptr){
           git_blame_free(blame);
       }
   }
