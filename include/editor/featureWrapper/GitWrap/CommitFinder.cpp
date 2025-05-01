#include "CommitFinder.hpp"



GitCommit::GitCommit(git_oid commitID, git_repository* rep)
: id(commitID) 
{
    if(git_commit_lookup(&commit, rep, &id) == 0){
        USABLE_FLAG = true;
        commitMessage = git_commit_message(commit);
    }
}


GitCommit::GitCommit(const std::string commitMSG, git_repository* rep)
    : commitMessage(commitMSG) 
{
    git_revwalk* walker = nullptr;
    git_revwalk_new(&walker, rep);
    git_revwalk_push_head(walker);

    git_oid tempoid;
    while(git_revwalk_next(&tempoid, walker) == 0){
        git_commit* tempcommit = nullptr;
        git_commit_lookup(&tempcommit, rep, &tempoid);
        if(strcmp( git_commit_message(tempcommit), commitMessage.c_str()) == 0){
            git_commit_free(tempcommit);
            id = tempoid;
            USABLE_FLAG = true;
            break;
        }
        else{
            git_commit_free(tempcommit);
        }
    }
    git_revwalk_free(walker);
    
}