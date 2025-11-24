
.. _program_listing_file_include_core_editor_DEPRECATE_GitWrap_Add_AddController.hpp:

Program Listing for File AddController.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_DEPRECATE_GitWrap_Add_AddController.hpp>` (``include/core/editor/DEPRECATE/GitWrap/Add/AddController.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include <git2.h>
   #include <string>
   
   #include <filesystem>
   namespace fs = std::filesystem;
   
   class AddController {
     public:
       git_index *index = nullptr;
       AddController()  = default;
       bool
       open(git_repository *repo);
   
       bool
       addFile(const fs::path &path);
       ~AddController();
   };
