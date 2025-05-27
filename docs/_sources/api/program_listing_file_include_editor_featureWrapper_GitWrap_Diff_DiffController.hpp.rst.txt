
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Diff_DiffController.hpp:

Program Listing for File DiffController.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Diff_DiffController.hpp>` (``include/editor/featureWrapper/GitWrap/Diff/DiffController.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <string>
   #include <optional>
   #include <vector>
   
   #include <git2.h>
   #include "editorCommit.hpp"
   // #include "CommitFinder.hpp"
   
   struct lineLog{
       unsigned int linenumber;
       std::string text;
   };
   
   struct DiffResult{
       std::vector<lineLog> NewLines;
       std::vector<lineLog> OldLines;
   };
   
   
   class DiffController{
   private:
       git_diff *Dobj = nullptr;
   
   public:
       bool CommitToCommit(git_repository* repo, git_oid newID, git_oid oldID);
       bool CommitToNow(git_repository* repo, git_oid oldID);
   
       bool execute(DiffResult* res);
   
       DiffController();
       ~DiffController();
   };
