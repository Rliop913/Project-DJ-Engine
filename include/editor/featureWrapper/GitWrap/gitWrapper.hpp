#pragma once
#include <string>
#include <optional>

#include <git2.h>

#include "BlameController.hpp"
#include "DiffController.hpp"
#include "AddController.hpp"


using MAYBE_BLAME = std::optional<BlameController>;

using BranchCommits = std::pair<std::string, std::vector<GitCommit>>;
using SaveDatas = std::vector<BranchCommits>;
class GitWrapper{
private:
    git_repository* repo = nullptr;
    git_signature* auth_sign = nullptr;
    std::optional<AddController> addIndex;

public:
    
    bool        MoveToBranch(const std::string&  branch_name);
    bool        checkout(const std::string&  branch_name,const std::string&  commit_message);
    bool        merge(const std::string&  branch);
    bool        add(const std::string&  path);
    bool        open(const std::string&  path);

    DiffResult  diff(const GitCommit&    oldCommit,  const GitCommit&    newCommit);

    MAYBE_BLAME Blame(const std::string&  filepath,   const GitCommit&    newCommit,  const GitCommit& oldCommit);
    bool        commit(git_signature* sign, const std::string& message);

    std::string log(){
        return std::string();//TODO implement
    };
    SaveDatas GetCommits();


    bool close();
    GitWrapper();
    ~GitWrapper();
};



class PDJE_GitHandler{
private:
    GitWrapper gw;
    git_signature* sign = nullptr;
public:
    
    bool Save(const std::string& tracingFile, const std::string& timeStamp);
    bool Checkout(const std::string& branch_name, const std::string& timeStamp);
    std::string GetLogWithMermaidGraph();
    DiffResult GetDiff(const GitCommit& oldTimeStamp, const GitCommit& newTimeStamp);
    
    bool DeleteGIT(const std::string& path);
    bool Open(const std::string& path);
    bool Close();
    SaveDatas GetCommits();


    PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email);
    ~PDJE_GitHandler();

};