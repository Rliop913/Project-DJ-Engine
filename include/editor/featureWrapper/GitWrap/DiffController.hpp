#pragma once

#include <string>
#include <optional>
#include <vector>

#include <git2.h>

#include "CommitFinder.hpp"

struct lineLog{
    unsigned int linenumber;
    std::string text;
};

struct DiffResult{
    std::vector<lineLog> NewLines;
    std::vector<lineLog> OldLines;
};


class DiffController{
private:
    git_diff *Dobj = nullptr;

public:
    bool CommitToCommit(git_repository* repo, git_oid newID, git_oid oldID);
    bool CommitToNow(git_repository* repo, git_oid oldID);

    bool execute(DiffResult* res);

    DiffController();
    ~DiffController();
};