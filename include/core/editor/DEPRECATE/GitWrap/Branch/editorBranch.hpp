/**
 * @file editorBranch.hpp
 * @brief Wrapper utilities for manipulating Git branches.
 */
#pragma once

#include "editorCommit.hpp"

#include <git2.h>

#include <iostream> //debugstr
#include <optional>
#include <string>
#include <vector>
namespace gitwrap {

/**
 * @brief Represents a Git branch and common branch operations.
 */
class branch {
  private:
    git_repository      *repo_pointer;
    git_checkout_options checkoutOpts = GIT_CHECKOUT_OPTIONS_INIT;

  public:
    /// Name of the current branch
    // std::string branchName; <- deprecate branch name.
    /// Temporary checkout commit used when jumping to a commit
    std::optional<git_oid> FLAG_TEMP_CHECKOUT;
    /**
     * @brief Convert a name to a Git branch reference.
     */
    template <typename T>
    static std::string
    ToBranchRefName(T branchName);

    /// List all branches in the repository
    std::vector<std::string>
    ShowExistBranch();
    /// Get commits that exist on the given branch
    std::vector<commit>
    ShowExistCommitsOnBranch(const std::string &branchName);

    /// Switch to the specified branch
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

    /// Retrieve the current HEAD commit
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
