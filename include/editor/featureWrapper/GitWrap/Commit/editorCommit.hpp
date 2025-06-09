#pragma once
#include <git2.h>

#include <string>
#include <vector>
#include <optional>
#include <list>

#include "PDJE_EXPORT_SETTER.hpp"

namespace gitwrap
{
    struct PDJE_API commit{
        git_commit* commitPointer = nullptr;
        git_oid commitID;
        std::string msg;
        commit() = default;
        commit(git_oid commitID, git_repository* rep);
        commit(const std::string commitMSG, git_repository* rep);
        ~commit(){
            if(commitPointer != nullptr){
                git_commit_free(commitPointer);
            }
        }
    };
    struct PDJE_API commitList{
        
        std::list<commit> clist; //Back is Newers
        

        void Reset(){
            clist.clear();
        }
        bool UpdateCommits(git_repository* repo);
        bool OkToAdd(git_oid id);
    };

}
