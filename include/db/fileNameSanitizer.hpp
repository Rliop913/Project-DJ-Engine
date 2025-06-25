#pragma once
#include <string>
#include <filesystem>
#include <optional>
#include <cppcodec/base64_url_unpadded.hpp>
namespace fs = std::filesystem;

using SANITIZED = std::string;
using cbase = cppcodec::base64_url_unpadded;


class PDJE_Name_Sanitizer{
    public:
    static std::optional<SANITIZED> sanitizeFileName(const std::string& fileName);
    static std::string getFileName(const SANITIZED& sanitized);
    static std::string PDJE_SANITIZE_ERROR;
    
    static bool CheckPath(fs::path& origin);
    PDJE_Name_Sanitizer() = delete;
    ~PDJE_Name_Sanitizer() = delete;
};
