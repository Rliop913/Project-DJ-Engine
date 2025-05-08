#include "DiffController.hpp"

DiffController::DiffController()
{

}

bool
DiffController::CommitToNow(git_repository* repo, git_oid oldID)
{
    git_tree* Otree = nullptr;
    auto OCommit = gitwrap::commit(oldID, repo);
    
    if(OCommit.commitPointer != nullptr){
        if(git_commit_tree(&Otree, OCommit.commitPointer) != 0){
            goto OLD_TREE_INIT_FAILED;
        }
        if(git_diff_tree_to_workdir(&Dobj, repo, Otree, nullptr) != 0){
            goto DIFF_FAILED;
        }
    }
    else{
        goto COMMIT_INIT_FAILED;
    }

    git_tree_free(Otree);
    return true;

    //FOR ERROR
    DIFF_FAILED:
    git_tree_free(Otree);
    OLD_TREE_INIT_FAILED:
    COMMIT_INIT_FAILED:
    return false;
}

bool
DiffController::CommitToCommit(git_repository* repo, git_oid newID, git_oid oldID)
{
    git_tree* Ntree = nullptr;
    git_tree* Otree = nullptr;
    auto NCommit = gitwrap::commit(newID, repo);
    auto OCommit = gitwrap::commit(oldID, repo);
    if(
        NCommit.commitPointer != nullptr &&
        OCommit.commitPointer != nullptr){

        if(git_commit_tree(&Ntree, NCommit.commitPointer) != 0){
            goto NEW_TREE_INIT_FAILED;
        }
        if(git_commit_tree(&Otree, OCommit.commitPointer) != 0){
            goto OLD_TREE_INIT_FAILED;
        }
        if(git_diff_tree_to_tree(&Dobj, repo, Otree, Ntree, nullptr) != 0){
            goto DIFF_FAILED;
        }
    }
    else{
        goto COMMIT_INIT_FAILED;
    }

    git_tree_free(Ntree);
    git_tree_free(Otree);
    return true;

    //FOR ERROR
    DIFF_FAILED:
    git_tree_free(Otree);
    OLD_TREE_INIT_FAILED:
    git_tree_free(Ntree);
    NEW_TREE_INIT_FAILED:
    COMMIT_INIT_FAILED:
    return false;
    
}

int 
DiffCallback(
    const git_diff_delta *delta, 
    const git_diff_hunk *hunk, 
    const git_diff_line *line, 
    void *payload)
{
    auto res = reinterpret_cast<DiffResult*>(payload);
    switch (line->origin)
    {
    case '+':
    {
        lineLog log;
        log.linenumber = line->new_lineno;
        log.text = line->content;
        res->NewLines.push_back(log);
        break;
    }
    case '-':
    {
        lineLog deleteLog;
        deleteLog.linenumber = line->old_lineno;
        deleteLog.text = line->content;
        res->OldLines.push_back(deleteLog);
        break;
    }
    default:
        break;
    };
    return 0;

}


bool
DiffController::execute(DiffResult* res)
{
    return
    git_diff_foreach(
        Dobj,
        nullptr,
        nullptr,
        nullptr,
        DiffCallback,
        reinterpret_cast<void*>(res)
    ) == 0;
}

DiffController::~DiffController()
{
    if(Dobj != nullptr){
        git_diff_free(Dobj);
    }
}