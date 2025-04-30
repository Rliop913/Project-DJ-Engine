#pragma once
#include <git2.h>
#include <string>
#include <optional>
struct BlameResult{

    git_oid commitID;
    size_t editStartLine = 0;
    size_t editedLines = 0;
    
    git_oid originID;
    size_t originStartLine = 0;
    
};

using MAYBE_BLAME_RESULT = std::optional<BlameResult>;

class BlameController{
    private:
        git_blame* blame = nullptr;
        unsigned int blameAmount = 0;
    public:
        BlameController() = default;
        bool BlameOpen(
            git_repository *repo, 
            const std::string& path, 
            git_blame_options *options = nullptr);
        
        MAYBE_BLAME_RESULT operator[](unsigned int idx);
        ~BlameController();
};
