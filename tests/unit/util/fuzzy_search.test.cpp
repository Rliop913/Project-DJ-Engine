#include <doctest/doctest.h>

#include "util/common/StatusCode.hpp"
#include "util/function/fuzzy/FuzzySearch.hpp"

#include <string_view>
#include <vector>

using PDJE_UTIL::common::StatusCode;
using PDJE_UTIL::function::fuzzy::FuzzySearch;
using PDJE_UTIL::function::fuzzy::FuzzySearchOptions;

TEST_CASE("util fuzzy search scores exact matches")
{
    const FuzzySearch search;

    auto score = search.score("Project DJ Engine", "Project DJ Engine");

    REQUIRE(score.ok());
    CHECK(score.value() == doctest::Approx(100.0));
}

TEST_CASE("util fuzzy search ranks matches and preserves source index")
{
    const FuzzySearch              search;
    std::vector<std::string_view> candidates{ "alpah", "alpha", "beta" };

    auto matches = search.search("alpha",
                                 candidates,
                                 FuzzySearchOptions{ .score_cutoff = 70.0 });

    REQUIRE(matches.ok());
    REQUIRE(matches.value().size() == 2);
    CHECK(matches.value()[0].index == 1);
    CHECK(matches.value()[0].score == doctest::Approx(100.0));
    CHECK(matches.value()[1].index == 0);
    CHECK(matches.value()[0].score >= matches.value()[1].score);
}

TEST_CASE("util fuzzy search limits result count")
{
    const FuzzySearch              search;
    std::vector<std::string_view> candidates{ "alpha", "alpah", "alfa" };

    auto matches = search.search("alpha",
                                 candidates,
                                 FuzzySearchOptions{ .score_cutoff = 0.0,
                                                     .max_results = 1 });

    REQUIRE(matches.ok());
    REQUIRE(matches.value().size() == 1);
    CHECK(matches.value()[0].index == 0);
}

TEST_CASE("util fuzzy search accepts empty candidate lists")
{
    const FuzzySearch                    search;
    const std::vector<std::string_view> candidates;

    auto matches = search.search("alpha", candidates);

    REQUIRE(matches.ok());
    CHECK(matches.value().empty());
}

TEST_CASE("util fuzzy search rejects invalid score cutoffs")
{
    const FuzzySearch search;

    auto low = search.score("a", "a", -1.0);
    CHECK_FALSE(low.ok());
    CHECK(low.status().code == StatusCode::invalid_argument);

    const std::vector<std::string_view> candidates;
    auto high =
        search.search("a", candidates, FuzzySearchOptions{ .score_cutoff = 101.0 });
    CHECK_FALSE(high.ok());
    CHECK(high.status().code == StatusCode::invalid_argument);
}
