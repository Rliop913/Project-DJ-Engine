#include <filesystem>

#include "gitWrapper.hpp"


namespace fs = std::filesystem;



PDJE_GitHandler::PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email)
{
    git_signature_now(&sign, auth_name.c_str(), auth_email.c_str());

}

PDJE_GitHandler::~PDJE_GitHandler()
{
    git_signature_free(sign);

}


bool
PDJE_GitHandler::Open(const std::string& path)
{
    return gw.open(path);
}

bool
PDJE_GitHandler::DeleteGIT(const std::string& path)
{

    if( !fs::exists(path) ||
        !fs::is_directory(path) ||
        !Close())
    {
        return false;
    }
    fs::remove_all(path);
    return true;
}


bool
PDJE_GitHandler::Close()
{
    return gw.close();
}


bool
PDJE_GitHandler::Save(const std::string& tracingFile)
{
    
    // if(!repo.has_value()){
    //     return false;
    // }
    // if(!idx.has_value()){
    //     if(git_repository_index(&idx.value(), repo.value()) != 0){
    //         return false;
    //     }
    // }
    // if(git_index_add_bypath(idx.value(), tracingFile.c_str())){
    //     return false;
    // }
    // git_oid treeOid;
    // git_tree* tree = nullptr;
    // git_index_write(idx.value());
    // git_index_write_tree(&treeOid, idx.value());
    // git_tree_lookup(&tree, repo.value(), &treeOid);

}