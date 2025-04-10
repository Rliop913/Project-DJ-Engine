
.. _program_listing_file_include_editor_gitWrapper_gitWrapper.hpp:

Program Listing for File gitWrapper.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_gitWrapper_gitWrapper.hpp>` (``include/editor/gitWrapper/gitWrapper.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <string>
   #include <optional>
   #include <filesystem>
   #include <sstream>
   #include <git2.h>
   namespace fs = std::filesystem;
   
   
   
   class PDJE_GitHandler{
   private:
       std::optional<git_repository*> repo;
       std::optional<git_index*> idx;
   public:
       
       bool Save(const std::string& tracingFile);
       bool Checkout();
       std::string GetLogWithMermaidGraph();
       bool GetDiff();
       
       bool DeleteGIT(const std::string& path);
       bool Open(const std::string& path);
       bool Close();
   
   
   
       PDJE_GitHandler();
       ~PDJE_GitHandler();
   
   };
