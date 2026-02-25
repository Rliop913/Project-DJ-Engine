#include "JsonDiffFriendlyIO.hpp"
#include <doctest/doctest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

std::filesystem::path
MakeTempPath(const char *suffix)
{
    const auto ticks = std::chrono::steady_clock::now().time_since_epoch().count();
    auto base = std::filesystem::temp_directory_path() /
                ("pdje-json-io-" + std::to_string(ticks));
    if (suffix != nullptr && suffix[0] != '\0') {
        base += suffix;
    }
    return base;
}

std::string
ReadAll(const std::filesystem::path &p)
{
    std::ifstream ifs(p);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

} // namespace

TEST_CASE("core: json diff-friendly formatter serializes scalar and empty containers")
{
    CHECK(PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(nlohmann::json(123)) == "123\n");
    CHECK(PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(nlohmann::json::object()) ==
          "{}\n");
    CHECK(PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(nlohmann::json::array()) ==
          "[]\n");
}

TEST_CASE("core: json diff-friendly formatter uses stable multiline object and array layout")
{
    nlohmann::json root = {
        { "a", 1 },
        { "b", nlohmann::json::array({ 1, 2 }) }
    };

    const std::string expected =
        "{\n"
        "  \"a\": 1\n"
        "  , \"b\": [\n"
        "    1\n"
        "    , 2\n"
        "  ]\n"
        "}\n";

    CHECK(PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(root) == expected);
}

TEST_CASE("core: json diff-friendly formatter appends trailing newline")
{
    nlohmann::json root = { { "k", "v" } };
    const auto out = PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(root);
    REQUIRE_FALSE(out.empty());
    CHECK(out.back() == '\n');
}

TEST_CASE("core: json diff-friendly writer writes exact formatted content")
{
    nlohmann::json root = {
        { "x", 1 },
        { "y", "z" }
    };

    const auto file = MakeTempPath(".json");
    std::error_code ec;
    std::filesystem::remove(file, ec);

    REQUIRE(PDJE_JSON_IO_DETAIL::WriteDiffFriendlyJsonToFile(file, root));
    CHECK(ReadAll(file) == PDJE_JSON_IO_DETAIL::DumpDiffFriendlyJson(root));

    std::filesystem::remove(file, ec);
}

TEST_CASE("core: json diff-friendly writer returns false when parent directory is missing")
{
    const auto dir  = MakeTempPath("");
    const auto file = dir / "child.json";

    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    REQUIRE_FALSE(std::filesystem::exists(dir));

    CHECK_FALSE(PDJE_JSON_IO_DETAIL::WriteDiffFriendlyJsonToFile(
        file, nlohmann::json::object()));
}
