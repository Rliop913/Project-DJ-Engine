#include "gitWrapper.hpp"


std::vector<const git_blame_hunk*>
GitWrapper::blame(const std::string& filepath)
{
    if(gbm != nullptr){
        git_blame_free(gbm);
        gbm = nullptr;
    }
    if(git_blame_file(&gbm, repo, filepath.c_str(), nullptr) != 0){
        return std::vector<const git_blame_hunk*>();
    }

    auto hunksize = git_blame_get_hunk_count(gbm);
    std::vector<const git_blame_hunk*> hunks;
    for(unsigned long long i = 0; i < hunksize; ++i){
        hunks.emplace_back(git_blame_get_hunk_byindex(gbm, i));
    }
    return hunks;
}



bool
GitWrapper::open(const std::string& path)
{
    if(git_repository_open(&repo, path.c_str()) == 0){
        return true;
    }
    else{
        if(git_repository_init(&repo, path.c_str(), false) == 0){
            return true;
        }
        else{
            return false;
        }
    }
}

GitWrapper::~GitWrapper()
{
    if(repo != nullptr){
        git_repository_free(repo);
    }
    if(idx != nullptr){
        git_index_free(idx);
    }
    if(gbm != nullptr){
        git_blame_free(gbm);
    }
}












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