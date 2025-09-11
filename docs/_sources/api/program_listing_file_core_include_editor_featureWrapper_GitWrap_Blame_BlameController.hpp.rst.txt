
.. _program_listing_file_core_include_editor_featureWrapper_GitWrap_Blame_BlameController.hpp:

Program Listing for File BlameController.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_editor_featureWrapper_GitWrap_Blame_BlameController.hpp>` (``core_include\editor\featureWrapper\GitWrap\Blame\BlameController.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include <filesystem>
   #include <git2.h>
   #include <optional>
   #include <string>
   namespace fs = std::filesystem;
   
   struct BlameResult {
   
       git_oid commitID;
       size_t  editStartLine = 0;
       size_t  editedLines   = 0;
   
       git_oid originID;
       size_t  originStartLine = 0;
   };
   
   using MAYBE_BLAME_RESULT = std::optional<BlameResult>;
   
   class BlameController {
     private:
       git_blame   *blame       = nullptr;
       unsigned int blameAmount = 0;
   
     public:
       BlameController() = default;
   
       bool
       BlameOpen(git_repository    *repo,
                 const fs::path    &path,
                 git_blame_options *options = nullptr);
   
       MAYBE_BLAME_RESULT
       operator[](unsigned int idx);
       ~BlameController();
   };
