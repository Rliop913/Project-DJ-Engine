#include "gitWrapper.hpp"


PDJE_GitHandler::PDJE_GitHandler()
{
    git_libgit2_init();
}

PDJE_GitHandler::~PDJE_GitHandler()
{
    if(repo.has_value()){
        git_repository_free(repo.value());
    }
    git_libgit2_shutdown();

}


bool
PDJE_GitHandler::Open(const std::string& path)
{
    if(repo.has_value()){
        return true;
    }
    repo.emplace(nullptr);
    if(git_repository_open(&repo.value(), path.c_str()) == 0){
        return true;
    }
    else{
        if(git_repository_init(&repo.value(), path.c_str(), false) == 0){
            return true;
        }
        else{
            return false;
        }
    }
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
    if(!repo.has_value()){
        return false;
    }
    git_repository_free(repo.value());
    return true;
}


bool
PDJE_GitHandler::Save(const std::string& tracingFile)
{
    if(!repo.has_value()){
        return false;
    }
    if(!idx.has_value()){
        if(git_repository_index(&idx.value(), repo.value()) != 0){
            return false;
        }
    }
    if(git_index_add_bypath(idx.value(), tracingFile.c_str())){
        return false;
    }
    git_oid treeOid;
    git_tree* tree = nullptr;
    git_index_write(idx.value());
    git_index_write_tree(&treeOid, idx.value());
    git_tree_lookup(&tree, repo.value(), &treeOid);

}