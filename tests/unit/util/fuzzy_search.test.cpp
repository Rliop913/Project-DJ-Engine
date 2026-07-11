#include <doctest/doctest.h>

#include "util/function/fuzzy/FuzzySearch.hpp"

#include <stdexcept>
#include <string_view>
#include <vector>

using PDJE_UTIL::function::fuzzy::FuzzySearch;
using PDJE_UTIL::function::fuzzy::FuzzySearchOptions;

TEST_CASE("util fuzzy search scores and ranks matches")
{
    const FuzzySearch search;
    CHECK(search.score("Project DJ Engine", "Project DJ Engine") ==
          doctest::Approx(100.0));

    const std::vector<std::string_view> candidates { "alpah", "alpha", "beta" };
    auto matches = search.search(
        "alpha", candidates, FuzzySearchOptions { .score_cutoff = 70.0 });
    REQUIRE(matches.size() == 2);
    CHECK(matches[0].index == 1);
    CHECK(matches[0].score == doctest::Approx(100.0));
    CHECK(matches[1].index == 0);
}

TEST_CASE("util fuzzy search limits results and accepts empty candidates")
{
    const FuzzySearch search;
    const std::vector<std::string_view> candidates { "alpha", "alpah", "alfa" };
    const auto limited = search.search(
        "alpha", candidates, { .score_cutoff = 0.0, .max_results = 1 });
    REQUIRE(limited.size() == 1);
    CHECK(limited.front().index == 0);
    CHECK(search.search("alpha", {}).empty());
}

TEST_CASE("util fuzzy search rejects invalid score cutoffs")
{
    const FuzzySearch search;
    CHECK_THROWS_AS(search.score("a", "a", -1.0), std::invalid_argument);
    CHECK_THROWS_AS(search.search("a", {}, { .score_cutoff = 101.0 }),
                    std::invalid_argument);
}
