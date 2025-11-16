#pragma once
#include <string>
#include <filesystem>
#include "PDJE_LOG_SETTER.hpp"
#include "LineVersion.hpp"
#include "EventMarker.hpp"
#include "GitDatas.hpp"
namespace PDJE_TIMELINE{
    namespace fs = std::filesystem;
    class TimeLine{
        private:
            std::shared_ptr<GitData> git;
            LineVersion line;
            EventMarker mark;
        public:
            bool Save();
            bool Undo();
            bool Redo();
            bool Go(const std::string& OID);
            void Diff(const OID& origin, const OID& compare);//todo-impl
            
            TimeLine(
                const fs::path& git_repo_root, 
                const std::string& file_name,
                const std::string& auth_name,
                const std::string& auth_email
            ):
                git(
                std::make_shared<GitData>(
                    git_repo_root,
                    file_name, 
                    auth_name,
                    auth_email
                )), line(git), mark(git)
            {
                startlog();
            }
            ~TimeLine();
    };
};