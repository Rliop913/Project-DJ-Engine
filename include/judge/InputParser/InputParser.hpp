#pragma once

#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_RAIL.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace PDJE_JUDGE {

using INPUT_RAW = std::vector<PDJE_Input_Log>;
using MIDI_RAW  = std::vector<PDJE_MIDI::MIDI_EV>;
using DEV_ID    = std::string;
struct PARSE_OUT {
    std::vector<PDJE_Input_Log>     logs;
    std::vector<PDJE_MIDI::MIDI_EV> midi_logs;
    uint64_t                        lowest;
    uint64_t                        highest;
};

static void
Parse(PARSE_OUT &out, const RAIL_DB &raildb, const INPUT_RAW &raw)
{
    out.logs.clear();
    out.logs.reserve(raw.size());
    if (raw.size() == 0) {
        return;
    }

    int64_t     off;
    std::string offsetkey;

    for (const auto &rawp : raw) {

        offsetkey.assign(rawp.id, rawp.id_len);

        auto it = raildb.offset.find(offsetkey);
        if (it != raildb.offset.end()) {

            off = it->second;
        } else {
            off = 0;
        }
        out.logs.push_back(rawp);
        out.logs.back().microSecond += off;
    }
    std::sort(out.logs.begin(),
              out.logs.end(),
              [](const PDJE_Input_Log &a, const PDJE_Input_Log &b) {
                  return a.microSecond < b.microSecond;
              });
    out.highest = out.logs.back().microSecond;
    out.lowest  = out.logs.front().microSecond;
    return;
}

static void
Parse(PARSE_OUT &out, const RAIL_DB &raildb, const MIDI_RAW &midi_raw)
{
    out.midi_logs.clear();
    out.midi_logs.reserve(midi_raw.size());
    if (midi_raw.size() == 0) {
        return;
    }
    int64_t     off;
    std::string offsetkey;
    for (const auto &rawp : midi_raw) {

        offsetkey.assign(rawp.port_name, rawp.port_name_len);
        auto it = raildb.offset.find(offsetkey);
        if (it != raildb.offset.end()) {
            off = it->second;
        } else {
            off = 0;
        }
        out.midi_logs.push_back(rawp);
        out.midi_logs.back().highres_time += off;
    }
    std::sort(out.midi_logs.begin(),
              out.midi_logs.end(),
              [](const PDJE_MIDI::MIDI_EV &a, const PDJE_MIDI::MIDI_EV &b) {
                  return a.highres_time < b.highres_time;
              });
    out.highest = out.midi_logs.back().highres_time;
    out.lowest  = out.midi_logs.front().highres_time;
    return;
}

} // namespace PDJE_JUDGE