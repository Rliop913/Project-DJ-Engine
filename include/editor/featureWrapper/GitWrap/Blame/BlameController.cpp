#include "BlameController.hpp"



bool
BlameController::BlameOpen(
    git_repository *repo, 
    const std::u8string& path, 
    git_blame_options *options)
{
    std::string strPath = std::string(path.begin(), path.end());
    if(blame != nullptr){
        return false;
    }
    if(git_blame_file(&blame, repo, strPath.c_str(), options) != 0){
        return false;
    }
    blameAmount = git_blame_get_hunk_count(blame);

    return true;

}

MAYBE_BLAME_RESULT
BlameController::operator[](unsigned int idx)
{
    if(idx >= blameAmount){
        return std::nullopt;
    }
    auto temphunk = git_blame_get_hunk_byindex(blame, idx);
    if(temphunk){
        BlameResult tempres;
        tempres.commitID        = temphunk->final_commit_id         ;
        tempres.editStartLine   = temphunk->final_start_line_number ;
        tempres.editedLines     = temphunk->lines_in_hunk           ;
        tempres.originID        = temphunk->orig_commit_id          ;
        tempres.originStartLine = temphunk->orig_start_line_number  ;
        return tempres;
    }
    else{
        return std::nullopt;
    }
    
}

BlameController::~BlameController()
{
    if(blame != nullptr){
        git_blame_free(blame);
    }
}