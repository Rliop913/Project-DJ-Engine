#include "LineVersion.hpp"
#include "GitDatas.hpp"
#include "GitRAII.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <git2/branch.h>
#include <git2/errors.h>
#include <git2/oid.h>
#include <git2/refs.h>
#include <git2/types.h>
#include <stdexcept>
namespace PDJE_TIMELINE {
bool
BranchLine::CheckUnborn()
{
    GIT_RAII::ref head;
    return git_repository_head(&head.p, git->GetRepo()) == GIT_EUNBORNBRANCH;
}

void
BranchLine::Born(git_index *idx)
{
    git_oid        tree_id{};
    git_oid        commit_id{};
    GIT_RAII::tree tree;
    if (git_index_write_tree(&tree_id, idx) != 0) {
        return;
    }
    if (git_tree_lookup(&tree.p, git->GetRepo(), &tree_id)) {
        return;
    }

    if (git_commit_create_v(&commit_id,
                            git->GetRepo(),
                            "HEAD",
                            git->GetSign(),
                            git->GetSign(),
                            nullptr,
                            "Init",
                            tree.p,
                            0) != 0) {

        return;
    }

    NewFirstLog(git, git_oid_tostr_s(&commit_id));
}

bool
BranchLine::IsDetached()
{
    auto res = git_repository_head_detached(git->GetRepo());
    if (res < 0) {
        critlog("git_repository_head_detached(repo) failed. GitErr: ");
        critlog(git_error_last()->message);
        throw std::runtime_error(git_error_last()->message);
    }
    return res == 1;
}
bool
BranchLine::GetHead(git_oid &oid)
{
    if (git_reference_name_to_id(&oid, git->GetRepo(), "HEAD")) {
        critlog("git_reference_name_to_id(HEAD) failed. GitErr:");
        critlog(git_error_last()->message);
        return false;
    } else {
        return true;
    }
}
bool
BranchLine::Diverge()
{
    git_oid det_head;
    if (!GetHead(det_head)) {
        return false;
    }
    auto headc = GIT_RAII::commit(git_oid_tostr_s(&det_head), git->GetRepo());
    GIT_RAII::ref branch;

    if (git_branch_create(
            &branch.p, git->GetRepo(), GenTimeStamp().c_str(), headc.p, 0) !=
        0) {
        critlog("git_branch_create failed. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }
    if (git_repository_set_head(git->GetRepo(), git_reference_name(branch.p)) !=
        0) {
        critlog("git_repository_set_head failed. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }
    return true;
}

bool
BranchLine::AppendToHead(git_index *idx)
{
    git_oid        tree_id{};
    git_oid        commit_id{};
    git_oid        parent_id{};
    GIT_RAII::tree tree;

    if ((git_index_write_tree(&tree_id, idx)) < 0) {
        critlog("git_index_write_tree failed. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }

    if ((git_tree_lookup(&tree.p, git->GetRepo(), &tree_id)) < 0) {
        critlog("git_tree_lookup failed. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }
    if ((git_reference_name_to_id(&parent_id, git->GetRepo(), "HEAD")) < 0) {
        critlog("git_reference_name_to_id failed. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }
    GIT_RAII::commit parent(git_oid_tostr_s(&parent_id), git->GetRepo());

    if (git_commit_create_v(&commit_id,
                            git->GetRepo(),
                            "HEAD",
                            git->GetSign(),
                            git->GetSign(),
                            nullptr,
                            GenTimeStamp().c_str(),
                            tree.p,
                            1,
                            parent.p) != 0) {
        critlog("failed to create commit. GitErr: ");
        critlog(git_error_last()->message);
        return false;
    }
    AddLog(git, git_oid_tostr_s(&commit_id), git_oid_tostr_s(&parent_id));
    return true;
}

}; // namespace PDJE_TIMELINE