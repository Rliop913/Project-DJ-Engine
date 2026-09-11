#pragma once

#include <string>
#include <utility>
#include <vector>
#include "global/PDJE_EXPORT_SETTER.hpp"
namespace PDJE_UTIL::function::fuzzy {

class PDJE_API FuzzySearch {
  public:
    void
    store(const std::string &store_string);

    std::vector<std::pair<int, std::string>>
    query(const std::string &query, int max_candidates) const;

  private:
    std::vector<std::string> candidates;
};

} // namespace PDJE_UTIL::function::fuzzy
