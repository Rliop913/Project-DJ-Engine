#pragma once
#include "PDJE_Input_Device_Data.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace PDJE_JUDGE {
constexpr bool IN  = true;
constexpr bool OUT = false;
struct NOTE {
    std::string        type;
    std::string        detail;
    std::string        first;
    std::string        second;
    std::string        third;
    unsigned long long pos  = 0;
    bool               used = false;
};
using NOTE_VEC   = std::vector<NOTE>;
using P_NOTE_VEC = std::vector<NOTE *>;
struct NOTE_ITR {
    NOTE_VEC           vec;
    NOTE_VEC::iterator itr;
};
using RAIL = std::unordered_map<uint64_t, NOTE_ITR>;
class OBJ {
  private:
    RAIL in;
    RAIL out;

  public:
    void
    Sort();

    template <bool>
    void
    Fill(const NOTE &data, uint64_t rail_id);

    template <bool>
    void
    Get(const uint64_t limit, uint64_t railID, P_NOTE_VEC &found);

    template <bool>
    void
    Cut(const unsigned long long                limit,
        std::unordered_map<uint64_t, NOTE_VEC> &cuts);
    OBJ()  = default;
    ~OBJ() = default;
};
}; // namespace PDJE_JUDGE