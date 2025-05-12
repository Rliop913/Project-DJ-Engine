#pragma once

#include "editorCommit.hpp"

#include <git2.h>

#include <string>
#include <vector>
#include <optional>

namespace gitwrap{

    class branch{
    private:
        git_repository* repo_pointer;
        git_checkout_options checkoutOpts = GIT_CHECKOUT_OPTIONS_INIT;

    public:
        std::optional<git_oid> FLAG_TEMP_CHECKOUT;
        template<typename T>
        static
        std::string
        ToBranchRefName(T branchName);

        std::vector<std::string> ShowExistBranch();
        std::vector<commit> ShowExistCommitsOnBranch(const std::string& branchName);

        bool SetBranch(const std::string& branchName);


        bool MakeNewFromHEAD(const std::string& newBranchName);
        bool MakeNewFromCommit(commit& c, const std::string& newBranchName);
        bool DeleteBranch(const std::string& branchName);
        bool CheckoutThisHEAD();
        bool CheckoutCommitTemp(commit& c);

        std::optional<commit> GetHEAD();

        bool MergeToBranch(const std::string& branchToMerge);

        branch(git_repository* repo) : repo_pointer(repo) {
            checkoutOpts.checkout_strategy= GIT_CHECKOUT_SAFE;
        };
        ~branch();
    };
};
