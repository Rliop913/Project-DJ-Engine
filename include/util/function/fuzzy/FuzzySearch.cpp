#include "FuzzySearch.hpp"

#include <algorithm>
#include <cmath>
#include <rapidfuzz/fuzz.hpp>
#include <stdexcept>
#include <utility>

namespace PDJE_UTIL::function::fuzzy {

namespace {

void
validate_score_cutoff(double score_cutoff)
{
    if (!std::isfinite(score_cutoff) || score_cutoff < 0.0 ||
        score_cutoff > 100.0) {
        throw std::invalid_argument(
            "FuzzySearch score_cutoff must be between 0 and 100.");
    }
}

} // namespace

double
FuzzySearch::score(std::string_view query,
                   std::string_view candidate,
                   double           score_cutoff) const
{
    validate_score_cutoff(score_cutoff);
    return rapidfuzz::fuzz::ratio(query, candidate, score_cutoff);
}

std::vector<FuzzyMatch>
FuzzySearch::search(std::string_view                  query,
                    std::span<const std::string_view> candidates,
                    FuzzySearchOptions                options) const
{
    validate_score_cutoff(options.score_cutoff);

    std::vector<FuzzyMatch> matches;
    matches.reserve(candidates.size());
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        const double candidate_score =
            rapidfuzz::fuzz::ratio(query, candidates[i], options.score_cutoff);
        if (candidate_score >= options.score_cutoff) {
            matches.push_back({ i, candidate_score });
        }
    }

    std::sort(matches.begin(),
              matches.end(),
              [](const FuzzyMatch &a, const FuzzyMatch &b) {
                  if (a.score != b.score) {
                      return a.score > b.score;
                  }
                  return a.index < b.index;
              });

    if (options.max_results > 0 && matches.size() > options.max_results) {
        matches.resize(options.max_results);
    }

    return matches;
}

} // namespace PDJE_UTIL::function::fuzzy
