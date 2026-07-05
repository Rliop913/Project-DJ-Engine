#include "FuzzySearch.hpp"

#include "util/common/StatusCode.hpp"

#include <algorithm>
#include <rapidfuzz/fuzz.hpp>
#include <utility>

namespace PDJE_UTIL::function::fuzzy {

namespace {

common::Result<void>
validate_score_cutoff(double score_cutoff)
{
    if (score_cutoff < 0.0 || score_cutoff > 100.0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "FuzzySearch score_cutoff must be between 0 and 100." });
    }
    return common::Result<void>::success();
}

} // namespace

common::Result<double>
FuzzySearch::score(std::string_view query,
                   std::string_view candidate,
                   double           score_cutoff) const
{
    auto valid_cutoff = validate_score_cutoff(score_cutoff);
    if (!valid_cutoff.ok()) {
        return common::Result<double>::failure(valid_cutoff.status());
    }

    return common::Result<double>::success(
        rapidfuzz::fuzz::ratio(query, candidate, score_cutoff));
}

common::Result<std::vector<FuzzyMatch>>
FuzzySearch::search(std::string_view                  query,
                    std::span<const std::string_view> candidates,
                    FuzzySearchOptions                options) const
{
    auto valid_cutoff = validate_score_cutoff(options.score_cutoff);
    if (!valid_cutoff.ok()) {
        return common::Result<std::vector<FuzzyMatch>>::failure(
            valid_cutoff.status());
    }

    std::vector<FuzzyMatch> matches;
    matches.reserve(candidates.size());
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        const double candidate_score =
            rapidfuzz::fuzz::ratio(query, candidates[i], options.score_cutoff);
        if (candidate_score >= options.score_cutoff) {
            matches.push_back({ i, candidate_score });
        }
    }

    std::sort(matches.begin(), matches.end(), [](const FuzzyMatch &a,
                                                 const FuzzyMatch &b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        return a.index < b.index;
    });

    if (options.max_results > 0 && matches.size() > options.max_results) {
        matches.resize(options.max_results);
    }

    return common::Result<std::vector<FuzzyMatch>>::success(std::move(matches));
}

} // namespace PDJE_UTIL::function::fuzzy
