#pragma once
#include <string>
#include <filesystem>
#include "PDJE_LOG_SETTER.hpp"
#include "LineVersion.hpp"

namespace PDJE_TIMELINE{
    namespace fs = std::filesystem;
    class TimeLine{

        public:
            bool Redo();
            bool Undo();
            bool Save();
            bool Go();
            void Diff();
            bool Sign(const std::string &auth_name, const std::string &auth_email);
            bool Open(const fs::path& root, const std::string& filename);
            bool MakeNew();
            bool Close();
            TimeLine(){
                startlog();
            }
            ~TimeLine();
    };
};