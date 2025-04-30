#pragma once
#include <string>
#include <optional>
#include <filesystem>
#include <sstream>
#include <vector>

#include <git2.h>

#include "BlameController.hpp"
#include "DiffController.hpp"

namespace fs = std::filesystem;

using MAYBE_BLAME = std::optional<BlameController>;

class GitWrapper{
private:
    git_repository* repo = nullptr;
    git_signature* auth_sign = nullptr;
    git_index* idx = nullptr;

public:
    
    bool branch(const std::string& branch_name = "new_branch");
    // checkout: 브랜치명 지정, 기본값 master
    bool checkout(const std::string& branch = "master");
    // merge: 브랜치명 지정, 기본값 origin/master
    bool merge(const std::string& branch = "origin/master");

    bool pull(const std::string& remote = "origin", const std::string& branch = "master");
    bool push(const std::string& remote = "origin", const std::string& branch = "master");
    bool add(const std::string& path = "");
    
    DiffResult diff(const GitCommit& oldCommit, const GitCommit& newCommit);

    MAYBE_BLAME Blame(const std::string& filepath, const GitCommit& newCommit, const GitCommit& oldCommit);

    bool open(const std::string& path);
    bool close();
    GitWrapper();
    ~GitWrapper();
};


class PDJE_GitHandler{
private:
    GitWrapper gw;
public:
    
    bool Save(const std::string& tracingFile);
    bool Checkout();
    std::string GetLogWithMermaidGraph();
    bool GetDiff();
    
    bool DeleteGIT(const std::string& path);
    bool Open(const std::string& path);
    bool Close();



    PDJE_GitHandler(const std::string& auth_name, const std::string& auth_email);
    ~PDJE_GitHandler();

};