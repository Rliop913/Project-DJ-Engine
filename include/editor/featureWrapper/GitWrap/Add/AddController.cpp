#include "AddController.hpp"

AddController::~AddController() 
{
    if (index) {
        git_index_free(index);
    }
}

bool 
AddController::open(git_repository* repo) 
{
    if (git_repository_index(&index, repo) != 0) {
        return false;
    }
    return true;
}

bool 
AddController::addFile(const std::u8string& path) 
{
    auto strPath = std::string(path.begin(), path.end());
    if (!index) {
        return false;
    }
    if (git_index_add_bypath(index, strPath.c_str()) != 0) {
        return false;
    }
    if (git_index_write(index) != 0) {
        return false;
    }
    return true;
}
