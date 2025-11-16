#include "EventMarker.hpp"
#include <fstream>
namespace PDJE_TIMELINE{


    bool
    EventMarker::Activate()
    {
        if(git_repository_head_detached(git->GetRepo()) == 0){
            return true;
        }
        git_reference* head   = nullptr;
        if(git_repository_head(&head, git->GetRepo()) == GIT_EUNBORNBRANCH){
            if(head){
                git_reference_free(head);
            }
            return true;
        }
        if(head){
            git_reference_free(head);
        }
        auto now = commit(git->now, git->GetRepo());

        if(git_reset(
            git->GetRepo(),
            reinterpret_cast<git_object*>(now.p),
            GIT_RESET_HARD,
            &opts
        ) != 0){
            critlog("failed to checkout. GitErr: ");
            critlog(git_error_last()->message);
            return false;
        }
    }
    bool
    EventMarker::Add()
    {
        if(!Activate()){
            return false;
        }
        auto abs_path =git->root / git->target_file; 
        if(!fs::exists(abs_path)){
            std::ofstream makefile(abs_path);
            if(!makefile){
                critlog("failed to create target file. Path: ");
                critlog(abs_path.generic_string());
                return false;
            }
        }
        if(git_index_add_bypath(index, git->target_file.c_str()) != 0){
            critlog("failed to add file. Path & GitErr: ");
            critlog(abs_path.generic_string());
            critlog(git_error_last()->message);
            return false;
        }
        if(git_index_write(index) != 0){
            critlog("failed to write indexed. Path & GitErr: ");
            critlog(abs_path.generic_string());
            critlog(git_error_last()->message);
            return false;
        }
    }
    void
    EventMarker::RepoInit()
    {
        git_reference* head   = nullptr;
        git_tree*      tree   = nullptr;
        git_oid tree_id{};
        git_oid commit_id{};
        if (git_repository_head(&head, git->GetRepo()) != GIT_EUNBORNBRANCH) {
            if(head){
                git_reference_free(head);
            }
            return;
        }
        if(!Add()){
            if(head){
                git_reference_free(head);
            }
            throw std::runtime_error("failed to append file.");
        }
        if(git_index_write_tree(&tree_id, index)!=0){
            if(head){
                git_reference_free(head);
            }
            return;
        }
        if(git_tree_lookup(&tree, git->GetRepo(), &tree_id)){
            if(head){
                git_reference_free(head);
            }
            
            return;
        }

        if(git_commit_create_v(
            &commit_id,
            git->GetRepo(),
            "HEAD",
            git->GetSign(),
            git->GetSign(),
            nullptr,
            "Init",
            tree,
            0
        ) != 0){
            if(head){
                git_reference_free(head);
            }
            if(tree){
                git_tree_free(tree);
            }
            return;
        }
        git->log_tree[git_oid_tostr_s(&commit_id)] = "ROOT";
        if(head){
            git_reference_free(head);
        }
        if(tree){
            git_tree_free(tree);
        }

    }
    bool
    EventMarker::Append()
    {
        if(!Add()){
            critlog("failed to append file.");
            return false;
        }

        NO IMPL CRASH
        //todo
        // 1. commit to head
        // 2. make new branch & commit

        // add commit log to log graph here.
        return true;
    }

    EventMarker::~EventMarker()
    {
        if(index){
            git_index_free(index);
        }
    }
};