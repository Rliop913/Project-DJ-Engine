#include "editorBranch.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "git2/oid.h"
using namespace gitwrap;

std::vector<std::string>
branch::ShowExistBranch()
{
    git_branch_iterator *branchIT = nullptr;
    if (git_branch_iterator_new(&branchIT, repo_pointer, GIT_BRANCH_LOCAL) !=
        0) {
        critlog("failed to iterate branch. from gitwrap::branch "
                "ShowExistBranch. gitLog: ");
        critlog(git_error_last()->message);
        return std::vector<std::string>();
    }

    git_reference           *branchITRRef;
    git_branch_t             branchType;
    const char              *branchName;
    std::vector<std::string> branchlist;
    while (git_branch_next(&branchITRRef, &branchType, branchIT) == 0) {
        if (git_branch_name(&branchName, branchITRRef) == 0) {
            branchlist.emplace_back(branchName);
        }
        git_reference_free(branchITRRef);
    }
    git_branch_iterator_free(branchIT);
    return branchlist;
}

template <>
std::string
branch::ToBranchRefName(const std::string &branchName)
{
    return "refs/heads/" + branchName;
}

template <>
std::string
branch::ToBranchRefName(const char *branchName)
{
    return "refs/heads/" + std::string(branchName);
}

std::vector<commit>
branch::ShowExistCommitsOnBranch(const std::string &branchName)
{
    git_revwalk *walker = nullptr;
    if (git_revwalk_new(&walker, repo_pointer) != 0) {
        critlog("failed to reverse walk. from gitwrap::branch "
                "ShowExistCommitsOnBranch. gitLog: ");
        critlog(git_error_last()->message);
        return std::vector<commit>();
    }

    if (git_revwalk_push_ref(
            walker, ToBranchRefName<const std::string &>(branchName).c_str()) !=
        0) {
        git_revwalk_free(walker);
        critlog("failed to push reverse walk ref. from gitwrap::branch "
                "ShowExistCommitsOnBranch. gitLog: ");
        critlog(git_error_last()->message);
        return std::vector<commit>();
    }

    git_revwalk_sorting(walker, GIT_SORT_TIME);

    std::vector<commit> commits;
    git_oid             tempid;
    while (git_revwalk_next(&tempid, walker) == 0) {
        commit tempcommit;
        git_oid_cpy(&tempcommit.commitID, &tempid);

        if (git_commit_lookup(&tempcommit.commitPointer,
                              repo_pointer,
                              &tempcommit.commitID) == 0) {
            tempcommit.msg =
                std::string(git_commit_message(tempcommit.commitPointer));
            commits.emplace_back(std::move(tempcommit));
        }
    }
    git_revwalk_free(walker);
    return commits;
}

bool
branch::SetBranch(const std::string &NewbranchName)
{
    if (git_repository_set_head(
            repo_pointer,
            ToBranchRefName<const std::string &>(NewbranchName).c_str()) == 0) {
        branchName = NewbranchName;
        std::cout << branchName << " in set branch" << std::endl;
        return true;
    } else {
        critlog(
            "failed to set branch. from gitwrap::branch SetBranch. gitLog: ");
        critlog(git_error_last()->message);
        return false;
    }
}

bool
branch::MakeNewFromHEAD(const std::string &newBranchName)
{
    auto head = GetHEAD();
    if (!head.has_value()) {
        critlog("failed to get head. from gitwrap::branch MakeNewFromHEAD.");
        return false;
    }
    git_reference *newbranch = nullptr;
    if (git_branch_create(&newbranch,
                          repo_pointer,
                          newBranchName.c_str(),
                          head->commitPointer,
                          1) == 0) {
        git_reference_free(newbranch);
        SetBranch(newBranchName);
        return true;
    }
    if (newbranch != nullptr) {
        git_reference_free(newbranch);
    }
    critlog("failed to make new from head. from gitwrap::branch "
            "MakeNewFromHEAD. gitLog: ");
    critlog(git_error_last()->message);
    return false;
}

bool
branch::MakeNewFromCommit(commit &c, const std::string &newBranchName)
{
    git_reference *newBranchRef = nullptr;
    if (git_branch_create(&newBranchRef,
                          repo_pointer,
                          newBranchName.c_str(),
                          c.commitPointer,
                          1) == 0) {
        git_reference_free(newBranchRef);
        return SetBranch(newBranchName);
    } else {
        critlog("failed to create from commit . from gitwrap::branch "
                "MakeNewFromCommit. gitLog: ");
        critlog(git_error_last()->message);
        return false;
    }
}

bool
branch::DeleteBranch(const std::string &branchName)
{

    git_reference *branchForDelete = nullptr;
    if (git_branch_lookup(&branchForDelete,
                          repo_pointer,
                          branchName.c_str(),
                          GIT_BRANCH_LOCAL) != 0) {
        warnlog("failed to ref branch from gitwrap::branch DeleteBranch . "
                "gitLog: ");
        warnlog(git_error_last()->message);
        return false;
    }
    if (git_branch_delete(branchForDelete) != 0) {
        git_reference_free(branchForDelete);
        critlog("failed to delete branch. from gitwrap::branch DeleteBranch. "
                "gitLog: ");
        critlog(git_error_last()->message);
        return false;
    }
    git_reference_free(branchForDelete);
    return true;
}

bool
branch::CheckoutThisHEAD()
{
    FLAG_TEMP_CHECKOUT.reset();
    bool chkHead = git_checkout_head(repo_pointer, &checkoutOpts) == 0;
    if (!chkHead) {
        critlog("failed to checkout to head. from gitwrap::branch "
                "CheckouotThisHEAD. gitLog: ");
        critlog(git_error_last()->message);
    }
    return chkHead;
}

bool
branch::CheckoutCommitTemp(commit &c)
{
    git_object *target = nullptr;
    if (git_object_lookup(
            &target, repo_pointer, &c.commitID, GIT_OBJECT_COMMIT) != 0) {
        critlog("failed to ref target. from gitwrap::branch "
                "CheckoutCommitTemp. gitLog: ");
        critlog(git_error_last()->message);
        return false;
    }

    if (git_checkout_tree(repo_pointer, target, &checkoutOpts) != 0) {
        git_object_free(target);
        critlog("failed to checkout to tree. from gitwrap::branch "
                "CheckoutCommitTemp. gitLog: ");
        critlog(git_error_last()->message);
        return false;
    }
    if (FLAG_TEMP_CHECKOUT.has_value()) {
        FLAG_TEMP_CHECKOUT.reset();
    }
    FLAG_TEMP_CHECKOUT.emplace();
    git_oid_cpy(&(FLAG_TEMP_CHECKOUT.value()), &(c.commitID));
    git_object_free(target);
    return true;
}

std::optional<commit>
branch::GetHEAD()
{
    git_reference *headref = nullptr;
    if (git_repository_head(&headref, repo_pointer) == 0) {
        commit c;
        if(git_oid_cpy(&c.commitID, git_reference_target(headref))!= 0){
            git_reference_free(headref);
            critlog("failed to copy oid.");
            return {};
        }
        if (git_commit_lookup(&c.commitPointer, repo_pointer, &c.commitID) == 0) {
            c.msg = git_commit_message(c.commitPointer);
            git_reference_free(headref);
            return std::move(c);
        } else {
            git_reference_free(headref);
        }
    }
    critlog("failed to getHead. from gitwrap::branch GetHEAD. gitLog: ");
    critlog(git_error_last()->message);
    return std::nullopt;
}

branch::~branch()
{
}
