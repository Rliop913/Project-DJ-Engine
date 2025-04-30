#pragma once

#include <git2.h>
#include <string>
#include <cstring>
struct GitCommit
{
    bool USABLE_FLAG =  false;
    git_oid id;
    std::string commitMessage;
    git_commit * commit = nullptr;
    GitCommit(git_oid commitID, git_repository* rep);
    GitCommit(const std::string commitMSG, git_repository* rep);
    ~GitCommit(){
        if(commit != nullptr){
            git_commit_free(commit);
        }
    }
};
