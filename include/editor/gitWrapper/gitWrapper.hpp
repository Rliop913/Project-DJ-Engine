#pragma once
#include <string>
#include <optional>
#include <filesystem>
#include <sstream>
#include <git2.h>
namespace fs = std::filesystem;



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