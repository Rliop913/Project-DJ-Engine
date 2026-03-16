#include "cppcodec/base64_url.hpp"
#include "fileNameSanitizer.hpp"
#include <doctest/doctest.h>

#include <string>

TEST_CASE("core: name sanitizer roundtrip preserves ascii content")
{
    const std::string original = "track title [mix]_v1.2";
    const auto        encoded = PDJE_Name_Sanitizer::sanitizeFileName(original);

    REQUIRE(encoded.has_value());
    CHECK_FALSE(encoded->empty());
    CHECK(PDJE_Name_Sanitizer::getFileName(*encoded) == original);
}

TEST_CASE(
    "core: name sanitizer roundtrip preserves empty and punctuation strings")
{
    const std::string empty  = "";
    const auto encoded_empty = PDJE_Name_Sanitizer::sanitizeFileName(empty);
    REQUIRE(encoded_empty.has_value());
    CHECK(PDJE_Name_Sanitizer::getFileName(*encoded_empty) == empty);

    const std::string original = "[]{}()!@#$%^&*()_+-=,.;'~` ascii only";
    const auto        encoded = PDJE_Name_Sanitizer::sanitizeFileName(original);
    REQUIRE(encoded.has_value());
    CHECK(PDJE_Name_Sanitizer::getFileName(*encoded) == original);
}

TEST_CASE("core: name sanitizer enforces encoded length boundary")
{
    std::size_t accepted_len = 0;
    std::size_t rejected_len = 0;

    for (std::size_t n = 0; n < 512; ++n) {
        if (cppcodec::base64_url::encoded_size(n) < 255) {
            accepted_len = n;
        } else {
            rejected_len = n;
            break;
        }
    }

    REQUIRE(rejected_len > 0);
    CHECK(cppcodec::base64_url::encoded_size(accepted_len) < 255);
    CHECK(cppcodec::base64_url::encoded_size(rejected_len) >= 255);

    CHECK(PDJE_Name_Sanitizer::sanitizeFileName(std::string(accepted_len, 'a'))
              .has_value());
    CHECK_FALSE(
        PDJE_Name_Sanitizer::sanitizeFileName(std::string(rejected_len, 'a'))
            .has_value());
}

TEST_CASE("core: name sanitizer rejects overly long encoded names")
{
    const std::string too_long(300, 'a');
    const auto        encoded = PDJE_Name_Sanitizer::sanitizeFileName(too_long);

    CHECK_FALSE(encoded.has_value());
}
