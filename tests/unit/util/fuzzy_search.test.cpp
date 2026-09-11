#include <doctest/doctest.h>

#include "util/function/fuzzy/FuzzySearch.hpp"

#include <string>

using PDJE_UTIL::function::fuzzy::FuzzySearch;

TEST_CASE("util fuzzy search stores and ranks candidates")
{
    FuzzySearch search;
    search.store("alpah");
    search.store("alpha");
    search.store("beta");

    const auto matches = search.query("alpha", 2);

    REQUIRE(matches.size() == 2);
    CHECK(matches[0] == std::pair{ 100, std::string{ "alpha" } });
    CHECK(matches[1].second == "alpah");
    CHECK(matches[1].first < matches[0].first);
}

TEST_CASE("util fuzzy search owns stored candidates")
{
    FuzzySearch search;
    std::string candidate = "alpha";
    search.store(candidate);
    candidate = "changed";

    const auto matches = search.query("alpha", 1);

    REQUIRE(matches.size() == 1);
    CHECK(matches[0] == std::pair{ 100, std::string{ "alpha" } });
}

TEST_CASE("util fuzzy search handles empty and non-positive limits")
{
    FuzzySearch search;
    CHECK(search.query("alpha", 1).empty());

    search.store("alpha");
    CHECK(search.query("alpha", 0).empty());
    CHECK(search.query("alpha", -1).empty());
}

TEST_CASE("util fuzzy search preserves storage order for equal scores")
{
    FuzzySearch search;
    search.store("ab");
    search.store("ac");

    const auto matches = search.query("ax", 2);

    REQUIRE(matches.size() == 2);
    CHECK(matches[0] == std::pair{ 50, std::string{ "ab" } });
    CHECK(matches[1] == std::pair{ 50, std::string{ "ac" } });
}
