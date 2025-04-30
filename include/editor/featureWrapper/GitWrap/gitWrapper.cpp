#include "gitWrapper.hpp"


MAYBE_BLAME
GitWrapper::Blame(const std::string& filepath, const GitCommit& newCommit, const GitCommit& oldCommit)
{
    auto newBlame = BlameController();
    git_blame_options opts;
    git_blame_options_init(&opts, GIT_BLAME_OPTIONS_VERSION);
    opts.newest_commit = newCommit.id;
    opts.oldest_commit = oldCommit.id;
    if(newBlame.BlameOpen(repo, filepath, &opts)){
        return std::move(newBlame);
    }
    else{
        return std::nullopt;
    }
}

DiffResult
GitWrapper::diff(const GitCommit& oldCommit, const GitCommit& newCommit)
{
    auto DiffHandle = DiffController();
    DiffResult results;
    if(!oldCommit.USABLE_FLAG) return results;

    if(newCommit.USABLE_FLAG){
        DiffHandle.CommitToCommit(repo, newCommit.id, oldCommit.id);
    }
    else{
        DiffHandle.CommitToNow(repo, oldCommit.id);
    }
    DiffHandle.execute(&results);
    return results;
}

bool
GitWrapper::add(const std::string& path)
{
    
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


bool
GitWrapper::close()
{
    if(repo == nullptr){
        return false;
    }
    git_repository_free(repo);
    return true;
}

GitWrapper::GitWrapper()
{
    git_libgit2_init();
}

GitWrapper::~GitWrapper()
{
    if(repo != nullptr){
        git_repository_free(repo);
    }
    if(idx != nullptr){
        git_index_free(idx);
    }
    git_libgit2_shutdown();
}











PDJE_GitHandler::PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email)
{
    
}

PDJE_GitHandler::~PDJE_GitHandler()
{
    

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