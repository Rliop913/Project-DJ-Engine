#include "fileNameSanitizer.hpp"

std::string PDJE_Name_Sanitizer::PDJE_SANITIZE_ERROR = "";

std::optional<SANITIZED> 
PDJE_Name_Sanitizer::sanitizeFileName(const std::string& fileName)
{
    if(cbase::encoded_size(fileName.size()) >= 255){
        PDJE_SANITIZE_ERROR += "sanitize filename failed: result filename length exceeded 254";
        return std::nullopt;
    }
    return cbase::encode(fileName);
}


std::string
PDJE_Name_Sanitizer::getFileName(const SANITIZED& sanitized)
{
    auto decoded = cbase::decode(sanitized);
    return std::string(decoded.begin(), decoded.end());
}