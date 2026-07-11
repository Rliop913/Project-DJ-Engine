#pragma once

#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::function::fuzzy {

struct FuzzySearchOptions {
    double      score_cutoff = 0.0;
    std::size_t max_results  = 0;
};

struct FuzzyMatch {
    std::size_t index = 0;
    double      score = 0.0;
};

class FuzzySearch {
  public:
    double
    score(std::string_view query,
          std::string_view candidate,
          double           score_cutoff = 0.0) const;

    std::vector<FuzzyMatch>
    search(std::string_view                  query,
           std::span<const std::string_view> candidates,
           FuzzySearchOptions                options = {}) const;
};

} // namespace PDJE_UTIL::function::fuzzy
