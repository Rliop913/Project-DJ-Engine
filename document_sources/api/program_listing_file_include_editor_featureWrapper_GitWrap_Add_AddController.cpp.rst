
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Add_AddController.cpp:

Program Listing for File AddController.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Add_AddController.cpp>` (``include\editor\featureWrapper\GitWrap\Add\AddController.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "AddController.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   AddController::~AddController()
   {
       if (index) {
           git_index_free(index);
       }
   }
   
   bool
   AddController::open(git_repository *repo)
   {
       if (git_repository_index(&index, repo) != 0) {
           critlog("failed to open or create git repository. from AddController "
                   "open. Errmsg; ");
           critlog(git_error_last()->message);
           return false;
       }
       return true;
   }
   
   bool
   AddController::addFile(const fs::path &path)
   {
   
       std::string safeStr = path.generic_string();
       if (!index) {
           critlog("git_index is null. tried from AddController addFile.");
           critlog(path.generic_string());
           return false;
       }
   
       if (git_index_add_bypath(index, safeStr.c_str()) != 0) {
           critlog("failed to add index by path. from AddController addFile. "
                   "ErrPath & errmsg: ");
           critlog(path.generic_string());
           critlog(git_error_last()->message);
           return false;
       }
       if (git_index_write(index) != 0) {
           critlog("failed to write index. from AddController addFile. ErrPath & "
                   "errmsg: ");
           critlog(path.generic_string());
           critlog(git_error_last()->message);
           return false;
       }
       return true;
   }
