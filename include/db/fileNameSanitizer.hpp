/**
 * @file fileNameSanitizer.hpp
 * @brief Provides utilities for sanitizing and de-sanitizing file names.
 */
#pragma once
#include <cppcodec/base64_url_unpadded.hpp>
#include <filesystem>
#include <optional>
#include <string>
namespace fs = std::filesystem;

/// @brief Type alias for a sanitized string.
using SANITIZED = std::string;

/// @brief Type alias for an unsanitized string.
using UNSANITIZED = std::string;

/// @brief Type alias for a string that should not be sanitized.
using DONT_SANITIZE = std::string;

/// @brief Type alias for a string that may or may not be sanitized.
using SANITIZED_ORNOT = std::string;

/// @brief Type alias for the base64 URL unpadded codec.
using cbase = cppcodec::base64_url_unpadded;

/**
 * @brief A utility class for sanitizing and de-sanitizing file names.
 *
 * This class provides static methods to encode and decode file names using a
 * URL-safe base64 encoding. This is useful for ensuring that file names do not
 * contain characters that are incompatible with the file system.
 */
class PDJE_Name_Sanitizer {
  public:
    /**
     * @brief Sanitizes a file name using base64 URL-unpadded encoding.
     *
     * @param fileName The file name to sanitize.
     * @return An optional containing the sanitized file name, or `std::nullopt`
     * if an error occurs.
     */
    static std::optional<SANITIZED>
    sanitizeFileName(const std::string &fileName);

    /**
     * @brief De-sanitizes a file name that was sanitized with
     * `sanitizeFileName`.
     *
     * @param sanitized The sanitized file name.
     * @return The original, de-sanitized file name.
     */
    static std::string
    getFileName(const SANITIZED &sanitized);

    PDJE_Name_Sanitizer()  = delete;
    ~PDJE_Name_Sanitizer() = delete;
};
