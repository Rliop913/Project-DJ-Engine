#pragma once
#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace PDJE_JUDGE {
constexpr int BUFFER_MAIN = 0;
constexpr int BUFFER_SUB  = 1;

using GLOBAL_TIME = uint64_t;
using LOCAL_TIME  = uint64_t;
/** @brief Judgable note metadata stored in buffers. */
struct PDJE_API NOTE {
    std::string type;
    uint16_t    detail;
    std::string first;
    std::string second;
    std::string third;
    LOCAL_TIME  microsecond = 0;
    bool        used        = false;
    bool        isDown      = true;
};

using NOTE_VEC   = std::vector<NOTE>;
using P_NOTE_VEC = std::vector<NOTE *>;

/** @brief Iterator wrapper keeping note vector and current cursor. */
struct PDJE_API NOTE_ITR {
    NOTE_VEC           vec;
    NOTE_VEC::iterator itr;
};

using RAILID_TO_NOTE = std::unordered_map<uint64_t, NOTE_ITR>;
using RAILID_TO_OFFSET = std::unordered_map<uint64_t, uint64_t>;

/** @brief Note buffer manager used during initialization and playback. */
class PDJE_API OBJ {
  private:
    RAILID_TO_NOTE Buffer_Main;
    RAILID_TO_NOTE Buffer_Sub;
    RAILID_TO_OFFSET id_offset;

    template <int I>
    RAILID_TO_NOTE *
    pick_buffer()
    {
        if constexpr (I == BUFFER_MAIN) {
            return &Buffer_Main;
        } else {
            return &Buffer_Sub;
        }
    }

  public:
    void
    SetOffsets(const INPUT_SETTING& setting){
        //todo - impl with multiple offset setting
        id_offset[setting.MatchRail] = setting.offset_microsecond;
    }

    /** @brief Sort internal buffers and reset iterators. */
    void
    Sort(); // use only for init

    template <int I>
    /** @brief Push note data into main or sub buffer. */
    void
    Fill(const NOTE &data, uint64_t rail_id)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of fill.");
        RAILID_TO_NOTE *buffer = pick_buffer<I>();
        (*buffer)[rail_id].vec.push_back(data);
    }

    template <int I>
    /** @brief Get notes up to the given time limit for a rail. */
    void
    Get(const LOCAL_TIME limit, uint64_t railID, P_NOTE_VEC &found)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of get.");
        RAILID_TO_NOTE *buffer = pick_buffer<I>();

        found.clear();
        auto &note = (*buffer)[railID];
        if (note.vec.empty()) {
            return;
        }
        while (true) { // pull iterator
            if (note.itr != note.vec.end() && note.itr->used) {

                ++note.itr;
            } else {
                break;
            }
        }

        auto titr = note.itr;

        while (true) {
            if ((titr != note.vec.end()) && titr->microsecond <= limit &&
                !titr->used) {

                found.push_back(std::addressof(*titr));
                ++titr;
            } else {
                break;
            }
        }
    }

    template <int I>
    /** @brief Move expired notes into the provided cut buffer. */
    void
    Cut(const LOCAL_TIME limit, std::unordered_map<uint64_t, NOTE_VEC> &cuts)
    {
        static_assert(I == BUFFER_MAIN || I == BUFFER_SUB,
                      "invalid use of cut.");
        RAILID_TO_NOTE *buffer = pick_buffer<I>();
        
        for (auto &rail : *buffer) {
            if (rail.second.vec.empty()) {
                continue;
            }
            auto titr = rail.second.itr;
            
            while (titr != rail.second.vec.end() &&
                   titr->microsecond <= (limit + id_offset[rail.first])) {
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
