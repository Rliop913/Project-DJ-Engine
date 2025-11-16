#pragma once
#include <git2.h>
#include <filesystem>
#include <unordered_map>
#include "PDJE_LOG_SETTER.hpp"
namespace PDJE_TIMELINE{
    namespace fs = std::filesystem;

    using OID = std::string;
    using BACK_OID = std::string;
    struct GitData{
    private:
        git_repository * repo = nullptr;
        git_signature * sign = nullptr;
    public:
        std::unordered_map<OID, BACK_OID> log_tree;
        OID now;
        std::vector<OID> LIFO_log;
        fs::path root;
        std::string target_file;
        git_signature * GetSign(){
            if(sign){
                return sign;
            }
            else{
                critlog("git signature is nullptr");
                return nullptr;
            }
        }
        git_repository* GetRepo(){
            if(repo){
                return repo;
            }
            else{
                critlog("repo is nullptr");
                return nullptr;
            }
        }
        GitData(
            const fs::path& git_repo_root, 
            const std::string& file_name,
            const std::string& auth_name,
            const std::string& auth_email
        )
        {
            if(git_signature_now(&sign, auth_name.c_str(), auth_email.c_str())!= 0){
                critlog("failed to init signature.");
            }
            target_file = file_name;
            root = git_repo_root;
            auto string_path = git_repo_root.generic_string();
            if(git_repository_open(&repo, string_path.c_str()) != 0){
                if(git_repository_init(&repo, string_path.c_str(), false) != 0){
                    critlog("failed to init repository. git log: ");
                    critlog(git_error_last()->message);
                    return;
                }
            }
        }
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