/**
 * @file editorCommit.hpp
 * @brief Lightweight wrappers around Git commit objects.
 */
#pragma once
#include <git2.h>

#include <string>
#include <vector>
#include <optional>
#include <list>

#include "PDJE_EXPORT_SETTER.hpp"

namespace gitwrap
{
    /**
     * @brief Represents a Git commit.
     */
    struct PDJE_API commit{
        git_commit* commitPointer = nullptr; ///< pointer to libgit2 commit object
        git_oid commitID;                   ///< commit identifier
        std::string msg;                    ///< commit message
        commit() = default;
        commit(git_oid commitID, git_repository* rep);
        commit(const std::string commitMSG, git_repository* rep);
        ~commit(){
            if(commitPointer != nullptr){
                git_commit_free(commitPointer);
            }
        }
    };
    /**
     * @brief Container of commits ordered from oldest to newest.
     */
    struct PDJE_API commitList{
        
        std::list<commit> clist; //Back is Newers
        

        /// Clear the stored commit list
        void Reset(){
            clist.clear();
        }
        bool UpdateCommits(git_repository* repo);
        bool OkToAdd(git_oid id);
    };

}

