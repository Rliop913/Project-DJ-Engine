
.. _program_listing_file_include_core_editor_TimeLine_GitDataPack_GitRAII.hpp:

Program Listing for File GitRAII.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_GitDataPack_GitRAII.hpp>` (``include/core/editor/TimeLine/GitDataPack/GitRAII.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_LOG_SETTER.hpp"
   #include <git2.h>
   #include <git2/blob.h>
   #include <git2/branch.h>
   #include <git2/diff.h>
   #include <git2/refs.h>
   #include <git2/tree.h>
   #include <git2/types.h>
   namespace GIT_RAII {
   using OID = std::string;
   class commit {
     private:
       git_oid o{};
   
     public:
       git_commit *p = nullptr;
       commit(const OID &id, git_repository *repo)
       {
           if (git_oid_fromstr(&o, id.c_str()) != 0) {
               critlog("failed to get git-oid. OID & GitErr: ");
               critlog(id);
               critlog(git_error_last()->message);
               return;
           }
   
           if (git_commit_lookup(&p, repo, &o) != 0) {
               critlog("failed look up git commit. OID & GitErr: ");
               critlog(id);
               critlog(git_error_last()->message);
           }
       };
       ~commit()
       {
           if (p) {
               git_commit_free(p);
           }
       }
   };
   
   class ref {
     public:
       git_reference *p = nullptr;
       ref()            = default;
       ~ref()
       {
           if (p) {
               git_reference_free(p);
           }
       }
   };
   
   class tree {
     public:
       git_tree *p = nullptr;
       tree()      = default;
       ~tree()
       {
           if (p) {
               git_tree_free(p);
           }
       }
   };
   
   class tree_entry {
     public:
       git_tree_entry *p = nullptr;
       tree_entry()      = default;
       ~tree_entry()
       {
           if (p) {
               git_tree_entry_free(p);
           }
       }
   };
   
   class blob {
     public:
       git_blob *p = nullptr;
       blob()      = default;
       ~blob()
       {
           if (p) {
               git_blob_free(p);
           }
       }
   };
   
   class branch_itr {
     public:
       git_branch_iterator *p = nullptr;
       branch_itr()           = default;
       ~branch_itr()
       {
           if (p) {
               git_branch_iterator_free(p);
           }
       }
   };
   
   class diff {
     public:
       git_diff *p = nullptr;
       diff()      = default;
       ~diff()
       {
           if (p) {
               git_diff_free(p);
           }
       }
   };
   
   class patch {
     public:
       git_patch *p = nullptr;
       patch()      = default;
       ~patch()
       {
           if (p) {
               git_patch_free(p);
           }
       }
   };
   
   }; // namespace GIT_RAII
