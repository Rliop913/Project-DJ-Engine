/**
 * @file gitLog.hpp
 * @brief Structures for storing commit logs and branch heads.
 */
#pragma once
#include "editorBranch.hpp"
#include "editorCommit.hpp"
#include "fileNameSanitizer.hpp"
#include <bit>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace gitwrap {
/// Single log entry associated with a commit
struct log {
    commit        now;
    git_oid       parentID = { { 0 } };
    DONT_SANITIZE authName;
    DONT_SANITIZE authEmail;
};

/// Branch name and HEAD commit pair
struct BranchHead {
    DONT_SANITIZE BranchName;
    git_oid       head = { { 0 } };
};

/**
 * @brief Collects logs for a Git repository.
 */
class logHandle {
  private:
    struct OID_HASHER {
        std::size_t
        operator()(const git_oid &oid) const noexcept;
    };
    struct OID_EQUAL {
        bool
        operator()(const git_oid &lhs, const git_oid &rhs) const noexcept
        {
            return git_oid_equal(&lhs, &rhs);
        }
    };
    git_repository *repoPointer = nullptr;
    void
    AddLog(const git_oid        &id,
           git_oid              &ChildID,
           const git_signature *&sign,
           const std::string    &msg);

  public:
    bool
    WalkBranch(const DONT_SANITIZE &branchName);
    std::unordered_map<git_oid, log, OID_HASHER, OID_EQUAL> logs;
    git_oid                                                 ROOT_ID = { { 0 } };
    std::vector<BranchHead>                                 heads;
    logHandle(git_repository *repo) : repoPointer(repo)
    {
    }
};
}; // namespace gitwrap
