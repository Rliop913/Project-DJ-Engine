/**
 * @file DiffController.hpp
 * @brief Helpers for generating diffs between Git commits.
 */
#pragma once

#include <string>
#include <optional>
#include <vector>

#include <git2.h>
#include "editorCommit.hpp"
#include "fileNameSanitizer.hpp"
// #include "CommitFinder.hpp"

/// Line data container used in diff results
struct lineLog{
    unsigned int linenumber;
    UNSANITIZED text;
};

/// Collection of lines changed between commits
struct DiffResult{
    std::vector<lineLog> NewLines;
    std::vector<lineLog> OldLines;
};


/**
 * @brief Executes diff operations between commits.
 */
class DiffController{
private:
    git_diff *Dobj = nullptr;

public:
    /// Compare two commits
    bool CommitToCommit(git_repository* repo, git_oid newID, git_oid oldID);
    /// Compare a commit with the working directory
    bool CommitToNow(git_repository* repo, git_oid oldID);

    /// Run the diff and fill the result structure
    bool execute(DiffResult* res);

    DiffController();
    ~DiffController();
};
