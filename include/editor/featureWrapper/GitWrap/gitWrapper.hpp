#pragma once
#include <string>
#include <optional>

#include <git2.h>
#include <nlohmann/json.hpp>

#include "BlameController.hpp"
#include "DiffController.hpp"
#include "AddController.hpp"


#include "editorBranch.hpp"
#include "git2/repository.h"
#include "gitLog.hpp"


using MAYBE_BLAME = std::optional<BlameController>;

using BranchCommits = std::pair<std::string, std::vector<gitwrap::commit>>;
// using SaveDatas = std::vector<BranchCommits>;

using BRANCH_HANDLE = std::optional<gitwrap::branch>;

class GitWrapper{
private:
    git_repository* repo = nullptr;
    git_signature* auth_sign = nullptr;
    std::optional<AddController> addIndex;
public:
    std::optional<gitwrap::logHandle> log_hdl;
    BRANCH_HANDLE handleBranch;

    bool add(const std::string&  path);
    bool open(const std::string&  path);

    DiffResult diff(
        const gitwrap::commit& oldCommit,
        const gitwrap::commit& newCommit);

    MAYBE_BLAME Blame(
        const std::string& filepath,
        const gitwrap::commit& newCommit,
        const gitwrap::commit& oldCommit);

    bool commit(git_signature* sign, const std::string& message);

    bool log();
    bool log(const std::string& branchName);

    std::string GenTimeStamp();
    // SaveDatas GetCommits();


    bool close();
    GitWrapper();
    ~GitWrapper();
};



class PDJE_GitHandler{
private:
    git_signature* sign = nullptr;
public:
    GitWrapper gw;

    bool Save(const std::string& tracingFile, const std::string& timeStamp);
    bool Undo(const std::string& tracingFile);
    bool Redo(const std::string& tracingFile);

    // bool Checkout();
    bool ChangeHEAD(const std::string& branchName, const std::string& commitID);
    std::string GetLogWithJSONGraph();
    bool UpdateLog(){
        return gw.log();
    }
    bool UpdateLog(const std::string& branchName){
        return gw.log(branchName);
    }

    DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);

    bool DeleteGIT(const std::string& path);
    bool Open(const std::string& path);
    bool Close();
    // SaveDatas GetCommits();


    PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email);
    ~PDJE_GitHandler();

};
