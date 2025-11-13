#include <filesystem>

#include "git2/repository.h"
#include "gitWrapper.hpp"

#include "PDJE_LOG_SETTER.hpp"

namespace fs = std::filesystem;

PDJE_GitHandler::PDJE_GitHandler(const DONT_SANITIZE &auth_name,
                                 const DONT_SANITIZE &auth_email)
{
    git_signature_now(&sign, auth_name.c_str(), auth_email.c_str());
}

PDJE_GitHandler::~PDJE_GitHandler()
{
    try {
        git_signature_free(sign);
    } catch (std::exception &e) {
        critlog("failed to free signature. from "
                "PDJE_GitHandler::~PDJE_GitHandler. ErrException: ");
        critlog(e.what());
    }
}

bool
PDJE_GitHandler::Open(const fs::path &gitpath, const fs::path &filepath)
{
    file = filepath.filename();
    bool openRes = gw.open(gitpath, file, sign);
    if (!openRes) {
        critlog("failed to open git. from PDJE_GitHandler Open. path: ");
        critlog(gitpath.generic_string());
    }
    return openRes;
}

bool
PDJE_GitHandler::DeleteGIT(const fs::path &path)
{

    if (!fs::exists(path) || !fs::is_directory(path) || !Close()) {
        critlog("something failed from PDJE_GitHandler DeleteGIT. path: ");
        critlog(path.generic_string());
        return false;
    }
    fs::remove_all(path);
    return true;
}

bool
PDJE_GitHandler::Close()
{
    bool closeRes = gw.close();
    if (!closeRes) {
        critlog("failed to close git. from PDJE_GitHandler Close.");
    }
    return closeRes;
}

bool
PDJE_GitHandler::Save(const DONT_SANITIZE &timeStamp)
{
    if (gw.handleBranch->FLAG_TEMP_CHECKOUT.has_value()) {
        gitwrap::commit tempcommit(gw.handleBranch->FLAG_TEMP_CHECKOUT.value(),
                                   gw.repo);
        if (!gw.handleBranch->MakeNewFromCommit(tempcommit,
                                                gw.GenTimeStamp())) {
            critlog(
                "failed to save. from PDJE_GitHandler Save. file&timestamp: ");
            critlog(file.generic_string());
            critlog(timeStamp);
            return false;
        }
        gw.handleBranch->FLAG_TEMP_CHECKOUT.reset();
    }
    if (!gw.add(file)) {
        critlog("failed to add. from PDJE_GitHandler Save. file&timestamp: ");
        critlog(file.generic_string());
        critlog(timeStamp);
        return false;
    }
    if (!gw.commit(sign, timeStamp)) {
        critlog(
            "failed to commit. from PDJE_GitHandler Save. file&timestamp: ");
        critlog(file.generic_string());
        critlog(timeStamp);
        return false;
    }
    return true;
}

bool
PDJE_GitHandler::Undo()
{
    std::cout <<gw.handleBranch->branchName << std::endl;
    if (UpdateLog(gw.handleBranch->branchName)) {
        gitwrap::commit before_commit;
        if (gw.handleBranch->FLAG_TEMP_CHECKOUT.has_value()) {

            git_oid_cpy(
                &before_commit.commitID,
                &(gw.log_hdl->logs[gw.handleBranch->FLAG_TEMP_CHECKOUT.value()]
                      .parentID));
        } else {
            auto head = std::move(gw.handleBranch->GetHEAD());
            if (head.has_value()) {
                git_oid_cpy(&before_commit.commitID, &head->commitID);
            } else {
                critlog("failed to getHead. from PDJE_GitHandler Undo.");
                return false;
            }
        }

        if (gw.handleBranch->CheckoutCommitTemp(before_commit)) {
            return true;
        } else {
            critlog("failed to checkout to commit. from PDJE_GitHandler Undo.");
            return false;
        }
    } else {
        critlog("failed to update log. from PDJE_GitHandler Undo.");
        return false;
    }
}

bool
PDJE_GitHandler::Redo()
{
    if (gw.handleBranch->FLAG_TEMP_CHECKOUT.has_value()) {
        try {
            for (auto &i : gw.log_hdl->logs) {
                if (git_oid_equal(
                        &i.second.parentID,
                        &gw.handleBranch->FLAG_TEMP_CHECKOUT.value())) {

                    if (gw.handleBranch->CheckoutCommitTemp(i.second.now)) {
                        return true;
                    } else {
                        critlog(
                            "failed to checkout. from PDJE_GitHandler Redo.");
                        return false;
                    }
                }
            }

        } catch (const std::exception &e) {
            critlog(
                "something failed. from PDJE_GitHandler Redo. ErrException: ");
            critlog(e.what());
            return false;
        }
    } else {
        infolog("nothing to redo. returned false. from PDJE_GitHandler Redo. "
                "no err");
        return false;
    }
    return false;
}

DiffResult
PDJE_GitHandler::GetDiff(const gitwrap::commit &oldTimeStamp,
                         const gitwrap::commit &newTimeStamp)
{
    return gw.diff(oldTimeStamp, newTimeStamp);
}

struct BranchJSON {
    DONT_SANITIZE branchname;
    DONT_SANITIZE oid;
};

DONT_SANITIZE
PDJE_GitHandler::GetLogWithJSONGraph()
{
    using nj = nlohmann::json;
    nj GraphRoot;
    try {
        if (!gw.log_hdl.has_value()) {
            throw std::runtime_error("log handle is empty.");
        }
        for (auto &i : gw.log_hdl->heads) {
            nj b;
            b["NAME"] = i.BranchName;
            b["OID"]  = DONT_SANITIZE(git_oid_tostr_s(&i.head));
            GraphRoot["BRANCH"].push_back(b);
        }
        for (auto &i : gw.log_hdl->logs) {
            nj c;
            c["OID"]      = DONT_SANITIZE(git_oid_tostr_s(&i.first));
            c["EMAIL"]    = i.second.authEmail;
            c["NAME"]     = i.second.authName;
            c["PARENTID"] = DONT_SANITIZE(git_oid_tostr_s(&i.second.parentID));
            GraphRoot["COMMIT"].push_back(c);
        }
    } catch (std::exception &e) {
        critlog("failed to get log with json. from PDJE_GitHandler "
                "GitLogWithJSONGraph. ErrException: ");
        critlog(e.what());
        return std::string(e.what());
    }
    return GraphRoot.dump();
}

bool
PDJE_GitHandler::Go(const DONT_SANITIZE &branchName, git_oid *commitID)
{
    if (!gw.handleBranch->SetBranch(branchName)) {
        critlog("setBranch failed. from PDJE_GitHandler Go.");
        return false;
    }
    auto headbranch = std::move(gw.handleBranch->GetHEAD());
    if (headbranch.has_value()) {

        if (git_oid_equal(&headbranch->commitID, commitID)) {
            if (gw.handleBranch->CheckoutThisHEAD()) {
                return true;
            } else {
                critlog("checkout head failed. from PDJE_GitHandler Go.");
                return false;
            }
        } else {
            auto tempcommit = gitwrap::commit(*commitID, gw.repo);
            if (gw.handleBranch->CheckoutCommitTemp(tempcommit)) {
                return true;
            } else {
                critlog("checkout commit failed. from PDJE_GitHandler Go.");
                return false;
            }
        }
    }
    critlog("headBranch has no value. from PDJE_GitHandler Go.");
    return false;
}