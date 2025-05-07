#pragma once
#include <git2.h>

#include <string>
#include <vector>
#include <optional>
#include <list>
namespace gitwrap
{
    struct commit{
        git_commit* commitPointer = nullptr;
        git_oid commitID;
        std::string msg;
        ~commit(){
            if(commitPointer != nullptr){
                git_commit_free(commitPointer);
            }
        }
    };
    struct commitList{
        
        std::list<commit> clist; //Back is Newers
        

        void Reset(){
            clist.clear();
        }
        bool UpdateCommits(git_repository* repo);
        bool OkToAdd(git_oid id);
    };

}
