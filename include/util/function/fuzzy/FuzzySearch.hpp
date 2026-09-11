#pragma once

#include <string>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::fuzzy {

class FuzzySearch {
  public:
    void
    store(const std::string &store_string);

    std::vector<std::pair<int, std::string>>
    query(const std::string &query, int max_candidates) const;

  private:
    std::vector<std::string> candidates;
};

} // namespace PDJE_UTIL::function::fuzzy
