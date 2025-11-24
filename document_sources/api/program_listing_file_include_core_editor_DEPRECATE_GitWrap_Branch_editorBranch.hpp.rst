
.. _program_listing_file_include_core_editor_DEPRECATE_GitWrap_Branch_editorBranch.hpp:

Program Listing for File editorBranch.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_DEPRECATE_GitWrap_Branch_editorBranch.hpp>` (``include/core/editor/DEPRECATE/GitWrap/Branch/editorBranch.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include "editorCommit.hpp"
   
   #include <git2.h>
   
   #include <iostream> //debugstr
   #include <optional>
   #include <string>
   #include <vector>
   namespace gitwrap {
   
   class branch {
     private:
       git_repository      *repo_pointer;
       git_checkout_options checkoutOpts = GIT_CHECKOUT_OPTIONS_INIT;
   
     public:
       // std::string branchName; <- deprecate branch name.
       std::optional<git_oid> FLAG_TEMP_CHECKOUT;
       template <typename T>
       static std::string
       ToBranchRefName(T branchName);
   
       std::vector<std::string>
       ShowExistBranch();
       std::vector<commit>
       ShowExistCommitsOnBranch(const std::string &branchName);
   
       bool
       SetBranch(const std::string &branchName);
   
       bool
       MakeNewFromHEAD(const std::string &newBranchName);
       bool
       MakeNewFromCommit(commit &c, const std::string &newBranchName);
       bool
       DeleteBranch(const std::string &branchName);
       bool
       CheckoutThisHEAD();
       bool
       CheckoutCommitTemp(commit &c);
   
       std::optional<commit>
       GetHEAD();
   
       branch(git_repository *repo) : repo_pointer(repo)
       {
           checkoutOpts.checkout_strategy = GIT_CHECKOUT_SAFE;
           git_reference *head_ref        = nullptr;
           if (git_repository_head(&head_ref, repo) == 0) {
               // crash-noimpl// todo - impl without cached branch name.
               // branchName = std::string(git_reference_shorthand(head_ref));
               // std::cout << branchName << "in branch init" << std::endl;
           }
           git_reference_free(head_ref);
       };
       ~branch();
   };
   }; // namespace gitwrap
