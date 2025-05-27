
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Add_AddController.cpp:

Program Listing for File AddController.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Add_AddController.cpp>` (``include/editor/featureWrapper/GitWrap/Add/AddController.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "AddController.hpp"
   
   AddController::~AddController() 
   {
       if (index) {
           git_index_free(index);
       }
   }
   
   bool 
   AddController::open(git_repository* repo) 
   {
       if (git_repository_index(&index, repo) != 0) {
           return false;
       }
       return true;
   }
   
   bool 
   AddController::addFile(const std::string& path) 
   {
       if (!index) {
           return false;
       }
       if (git_index_add_bypath(index, path.c_str()) != 0) {
           return false;
       }
       if (git_index_write(index) != 0) {
           return false;
       }
       return true;
   }
