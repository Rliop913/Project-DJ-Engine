#pragma once
#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace PDJE_JUDGE {
constexpr int KEY_IN       = 0;
constexpr int KEY_OUT      = 1;
constexpr int AXIS_DEV_IN = 2;
constexpr int AXIS_DEV_OUT = 3;
constexpr int HID_DEV  = 3;
struct NOTE {
    PDJE_Dev_Type      type;
    BITMASK            detail;
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
using DEV_AND_NOTE = std::unordered_map<uint64_t, NOTE_ITR>;
class OBJ {
  private:
    DEV_AND_NOTE key_in;
    DEV_AND_NOTE key_out;
    DEV_AND_NOTE axis_in;
    DEV_AND_NOTE axis_out;
    DEV_AND_NOTE hid;

    template <int I>
    DEV_AND_NOTE *
    pick_dan()
    {
        if constexpr (I == KEY_IN) {
            return &key_in;
        } else if constexpr (I == KEY_OUT) {
            return &key_out;
        } else if constexpr (I == AXIS_DEV_IN) {
            return &axis_in;
        } else if constexpr (I == AXIS_DEV_OUT) {
            return &axis_out;
        } else {
            return &hid;
        }
    }

  public:
    void
    Sort(); // use only for init

    template <int I>
    void
    Fill(const NOTE &data, uint64_t rail_id)
    {
        static_assert(I == IN || I == OUT || I == AXIS_DEV || I == HID_DEV,
                      "invalid use of fill.");
        DEV_AND_NOTE *dan = pick_dan<I>();
        (*dan)[rail_id].vec.push_back(data);
    }

    template <int I>
    void
    Get(const uint64_t limit, uint64_t railID, P_NOTE_VEC &found)
    {
        static_assert(I == IN || I == OUT || I == AXIS_DEV || I == HID_DEV,
                      "invalid use of get.");
        DEV_AND_NOTE *dan = pick_dan<I>();

        found.clear();
        auto &note = (*dan)[railID];

        while (true) { // pull iterator
            if (note.itr != note.vec.end() && note.itr->used) {
                ++note.itr;
            } else {
                break;
            }
        }

        auto titr = (*dan)[railID].itr;
        while (true) {
            if ((titr != note.vec.end()) && titr->pos <= limit && !titr->used) {
                found.push_back(std::addressof(*titr));
                ++titr;
            } else {
                break;
            }
        }
    }

    template <int I>
    void
    Cut(const unsigned long long                limit,
        std::unordered_map<uint64_t, NOTE_VEC> &cuts)
    {
        static_assert(I == IN || I == OUT || I == AXIS_DEV || I == HID_DEV,
                      "invalid use of cut.");
        DEV_AND_NOTE *dan = pick_dan<I>();

        cuts.clear();
        for (auto &rail : *dan) {
            auto titr = rail.second.itr;
            while (titr != rail.second.vec.end() && titr->pos <= limit) {
                if (!titr->used) {
                    cuts[rail.first].push_back(*titr);
                    titr->used = true;
                }
                ++titr;
            }
        }
    }
    OBJ()  = default;
    ~OBJ() = default;
};
}; // namespace PDJE_JUDGE