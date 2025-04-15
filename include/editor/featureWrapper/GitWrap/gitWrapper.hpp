#pragma once
#include <string>
#include <optional>
#include <filesystem>
#include <sstream>
#include <vector>

#include <git2.h>
namespace fs = std::filesystem;



class GitWrapper{
private:
    git_repository* repo = nullptr;
    git_index* idx = nullptr;
    git_blame* gbm = nullptr;
public:
    
    bool add();
    bool push();
    bool pull();
    bool branch();
    bool checkout();
    bool merge();
    bool diff();

    
    std::vector<const git_blame_hunk*> 
        blame(const std::string& filepath);

    bool open(const std::string& path);
    GitWrapper() = default;
    ~GitWrapper();
};


class PDJE_GitHandler{
private:
    std::optional<git_repository*> repo;
    std::optional<git_index*> idx;
public:
    
    bool Save(const std::string& tracingFile);
    bool Checkout();
    std::string GetLogWithMermaidGraph();
    bool GetDiff();
    
    bool DeleteGIT(const std::string& path);
    bool Open(const std::string& path);
    bool Close();



    PDJE_GitHandler();
    ~PDJE_GitHandler();

};