#pragma once

#include "editorCommit.hpp"

#include <git2.h>

#include <string>
#include <vector>
#include <optional>

namespace gitwrap{
    
    class branch{
    private:
        git_reference* bRef;
        git_repository* repo_pointer;
    public:
        commit HEAD;

        std::vector<std::string> ShowExistBranch();
        std::vector<commit> ShowExistCommitsOnBranch();
        
        bool SetBranch(const std::string& branchName);
        
        bool HEADMoveFoward();
        bool HEADMoveBackward();
        bool HEADFindAndMove(const std::string& commitMSG);

        bool MakeNewFromHEAD(const std::string& newBranchName);

        bool CheckoutThisHEAD();

        branch(git_repository* repo) : repo_pointer(repo) {};
        ~branch();
    };
};