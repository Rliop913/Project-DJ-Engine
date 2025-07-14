#include "fileNameSanitizer.hpp"
#include "PDJE_LOG_SETTER.hpp"

std::optional<SANITIZED> 
PDJE_Name_Sanitizer::sanitizeFileName(const std::string& fileName)
{
    if(cbase::encoded_size(fileName.size()) >= 255){
        critlog("failed to sanitize filename. from PDJE_Name_Sanitizer sanitizeFileName. ErrfileName: ");
        critlog(fileName);
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