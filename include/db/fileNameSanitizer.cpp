#include "fileNameSanitizer.hpp"

std::optional<SANITIZED> 
PDJE_Name_Sanitizer::sanitizeFileName(const std::string& fileName)
{
    if(cbase::encoded_size(fileName.size()) >= 255){
        PDJE_SANITIZE_ERROR = +"sanitize filename failed: result filename length exceeded 254";
        return std::nullopt;
    }
    return cbase::encode(fileName);
}

bool 
PDJE_Name_Sanitizer::CheckPath(fs::path& origin)
{
#ifdef _WIN32
    origin = origin.string().size() > 260 ?  fs::path(R"(\\?\)") / origin : origin;
    if(origin.string().size() > 32767){
        PDJE_SANITIZE_ERROR += "Absolute path sanitize failed: result path length exceeded 32767";
        return false;
    }
#else
    if(origin.string().size() > 4096){
        PDJE_SANITIZE_ERROR += "Absolute path sanitize failed: result path length exceeded 4096";
        return false;
    }
#endif
    
    return true;
    
}


std::string
PDJE_Name_Sanitizer::getFileName(const SANITIZED& sanitized)
{
    auto decoded = cbase::decode(sanitized);
    return std::string(decoded.begin(), decoded.end());
}