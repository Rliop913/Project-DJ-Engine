#include "fileNameSanitizer.hpp"
#include <cppcodec/base64_default_url.hpp>

/// @brief Type alias for the base64 URL unpadded codec.
using cbase = cppcodec::base64_url;
std::optional<SANITIZED>
PDJE_Name_Sanitizer::sanitizeFileName(const std::string &fileName)
{
    if (cbase::encoded_size(fileName.size()) >= 255) {
        return std::nullopt;
    }
    return cbase::encode(fileName);
}

std::string
PDJE_Name_Sanitizer::getFileName(const SANITIZED &sanitized)
{
    auto decoded = cbase::decode(sanitized);
    return std::string(decoded.begin(), decoded.end());
}