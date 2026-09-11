#include "FuzzySearch.hpp"

#include <algorithm>
#include <cmath>
#include <rapidfuzz/fuzz.hpp>

namespace PDJE_UTIL::function::fuzzy {

void
FuzzySearch::store(const std::string &store_string)
{
    candidates.push_back(store_string);
}

std::vector<std::pair<int, std::string>>
FuzzySearch::query(const std::string &query_string, int max_candidates) const
{
    if (max_candidates <= 0) {
        return {};
    }

    std::vector<std::pair<int, std::string>> matches;
    matches.reserve(candidates.size());

    for (const auto &candidate : candidates) {
        const auto similarity = static_cast<int>(
            std::lround(rapidfuzz::fuzz::ratio(query_string, candidate)));
        matches.emplace_back(similarity, candidate);
    }

    std::stable_sort(matches.begin(),
                     matches.end(),
                     [](const auto &left, const auto &right) {
                         return left.first > right.first;
                     });

    const auto limit = static_cast<std::size_t>(max_candidates);
    if (matches.size() > limit) {
        matches.resize(limit);
    }

    return matches;
}

} // namespace PDJE_UTIL::function::fuzzy
