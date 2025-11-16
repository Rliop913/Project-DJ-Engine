#pragma once
#include "GitDatas.hpp"

namespace PDJE_TIMELINE{
    class commit{
        private:
        git_oid o{};
        public:
        git_commit* p = nullptr;
        commit(const OID& id, git_repository* repo){
            if(git_oid_fromstr(&o, id.c_str()) != 0){
                critlog("failed to get git-oid. OID & GitErr: ");
                critlog(id);
                critlog(git_error_last()->message);
                return;
            }

            if(git_commit_lookup(&p, repo, &o) != 0){
critlog("failed look up git commit. OID & GitErr: ");
                critlog(id);
                critlog(git_error_last()->message);
            }
        };
        ~commit(){
            if(p){
                git_commit_free(p);
            }
        }
    };
    class EventMarker{
        private:
        git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
        std::shared_ptr<GitData> git;
        git_index* index;
        bool isActivate = true;
        void RepoInit();
        bool Add();
        public:
        bool Append();
        bool MoveFront();//todo-impl
        bool MoveBack();//todo-impl
        bool Move(const OID& target);//todo-impl
        bool Activate();
        EventMarker(const std::shared_ptr<GitData>& git_ptr)
        : git(git_ptr){
            try{
                if(git_repository_index(&index, git->GetRepo()) != 0){
                    critlog("failed to open git index. GitErr: ");
                    critlog(git_error_last()->message);
                    return;
                }
                RepoInit();
                opts.checkout_strategy = GIT_CHECKOUT_FORCE;
            }
            catch(const std::exception& e){
                critlog("failed to init event marker. What: ");
                critlog(e.what());
            }
        }
        ~EventMarker();
    };
};