#pragma once
#include "Input_State.hpp"
#include <iostream>
#include "PDJE_Input_Device_Data.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace PDJE_JUDGE {
constexpr int BUFFER_MAIN = 0;
constexpr int BUFFER_SUB  = 1;

struct NOTE {
    std::string        type;
    uint16_t           detail;
    std::string        first;
    std::string        second;
    std::string        third;
    unsigned long long pos    = 0;
    bool               used   = false;
    bool               isDown = true;
};

using NOTE_VEC   = std::vector<NOTE>;
using P_NOTE_VEC = std::vector<NOTE *>;

struct NOTE_ITR {
    NOTE_VEC           vec;
    NOTE_VEC::iterator itr;
};

constexpr double OneSample = 1000.0 / 48.0;
static
uint64_t
FrameToMicro(uint64_t frame,
                         uint64_t origin_frame,
                         uint64_t origin_microsecond)
{
    if (origin_frame > frame) {
        auto diff = origin_frame - frame;
        diff *= OneSample;
        return origin_microsecond - diff;
    } else {
        auto diff = frame - origin_frame;
        diff *= OneSample;
        return origin_microsecond + diff;
    }
}

using DEVID_TO_NOTE = std::unordered_map<uint64_t, NOTE_ITR>;

class OBJ {
  private:
    DEVID_TO_NOTE Buffer_Main;
    DEVID_TO_NOTE Buffer_Sub;

    template <int I>
    DEVID_TO_NOTE *
    pick_dan()
    {
        if constexpr (I == BUFFER_MAIN) {
            return &Buffer_Main;
        } else {
            return &Buffer_Sub;
        }
    }

  public:
    void
    Sort(); // use only for init

    template <int I>
    void
    Fill(const NOTE &data, uint64_t rail_id)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of fill.");
        DEVID_TO_NOTE *dan = pick_dan<I>();
        (*dan)[rail_id].vec.push_back(data);
    }

    template <int I>
    void
    Get(const uint64_t limit_microsecond, uint64_t railID, P_NOTE_VEC &found, uint64_t origin_frame, uint64_t origin_microsecond)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of get.");
        DEVID_TO_NOTE *dan = pick_dan<I>();

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
        uint64_t note_microsecond;
        while (true) {
            note_microsecond = FrameToMicro(titr->pos, origin_frame, origin_microsecond);
            if ((titr != note.vec.end()) && note_microsecond <= limit_microsecond && !titr->used) {
                found.push_back(std::addressof(*titr));
                ++titr;
            } else {
                break;
            }
        }
    }
    
    template <int I>
    void
    Cut(const unsigned long long                limit_microsecond,
        std::unordered_map<uint64_t, NOTE_VEC> &cuts, uint64_t origin_frame, uint64_t origin_microsecond)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of cut.");
        DEVID_TO_NOTE *dan = pick_dan<I>();
        uint64_t note_microsecond;
        for (auto &rail : *dan) {
            auto titr = rail.second.itr;
            note_microsecond = FrameToMicro(titr->pos, origin_frame, origin_microsecond);
            std::cout << "cutting" << titr->pos << " nmic: " << note_microsecond << " limit: " << limit_microsecond << std::endl;
            while (titr != rail.second.vec.end() && note_microsecond <= limit_microsecond) {
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