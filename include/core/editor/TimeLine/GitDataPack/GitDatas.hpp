#pragma once
#include <git2.h>
#include "PDJE_LOG_SETTER.hpp"
namespace PDJE_TIMELINE{
    struct GitData{
    private:
        git_repository * repo = nullptr;
        git_signature * sign = nullptr;
    public:
        git_repository* GetRepo(){
            if(repo){
                return repo;
            }
            else{
                critlog("repo is nullptr");
                CRASH_NOIMPL
            }
        }
        GitData();
        ~GitData()
        {
            if(!sign){
                git_signature_free(sign);
            }
            if(!repo){
                git_repository_free(repo);
            }
        }
    };
};