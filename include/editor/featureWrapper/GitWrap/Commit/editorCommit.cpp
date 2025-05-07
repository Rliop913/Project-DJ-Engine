#include "editorCommit.hpp"

using namespace gitwrap;


bool
commitList::UpdateCommits(git_repository* repo)
{
    clist.back();
    git_revwalk* walker = nullptr;
    if(git_revwalk_new(&walker, repo) != 0){
        return false;
    }
    
    if(git_revwalk_push_head(walker) != 0){
        git_revwalk_free(walker);
        return false;
    }

    git_revwalk_sorting(walker, GIT_SORT_TIME);

    git_oid tempid;
    std::list<commit> templist;
    while(git_revwalk_next(&tempid, walker) == 0){
        if(OkToAdd(tempid)){
            templist.emplace_front();
            templist.front().commitID = tempid;
            if(git_commit_lookup(&templist.front().commitPointer, repo, &templist.front().commitID) == 0){
                templist.front().msg = std::string(git_commit_message(templist.front().commitPointer));
            }
        }
        else{
            clist.splice(clist.end(), templist);
            break;
        }
    }
    git_revwalk_free(walker);
    return true;
}

bool
commitList::OkToAdd(git_oid id)
{
    if(clist.empty()){
        return true;
    }
    else if(git_oid_cmp(&clist.back().commitID, &id) != 0){
        return true;
    }
    else{
        return false;
    }
}