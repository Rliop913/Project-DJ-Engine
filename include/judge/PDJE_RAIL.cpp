#include "PDJE_RAIL.hpp"

namespace PDJE_JUDGE {
using namespace RAIL_KEY;

void
RAIL_DB::Add(const RAIL_KEY::KB_MOUSE &key,
             const PDJE_Dev_Type      &type,
             const uint64_t           &id,
             const int64_t             __offset)
{
    if (kb_mouse_raildata.contains(key)) {
        return;
    }
    kb_mouse_raildata[key]  = id;
    offset[key.Device_Name] = __offset;
    META tempmeta;
    tempmeta.key  = key;
    tempmeta.type = type;
    meta[id].push_back(tempmeta);
}
void
RAIL_DB::Add(const RAIL_KEY::MIDI &key,
             const uint64_t       &id,
             const int64_t         __offset)
{
    if (midi_raildata.contains(key)) {
        return;
    }
    midi_raildata[key]    = id;
    offset[key.port_name] = __offset;
    META tempmeta;
    tempmeta.key  = key;
    tempmeta.type = key.type;
    meta[id].push_back(tempmeta);
}

void
RAIL_DB::Delete(const RAIL_KEY::KB_MOUSE &key)
{
    if (kb_mouse_raildata.contains(key)) {
        auto id = kb_mouse_raildata[key];
        kb_mouse_raildata.erase(key);
        offset.erase(key.Device_Name);
        if (meta.contains(id)) {
            auto &vec = meta[id];
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                if (auto metakey = std::get_if<RAIL_KEY::KB_MOUSE>(&it->key)) {
                    if (*metakey == key) {
                        vec.erase(it);
                        break;
                    }
                }
            }
        }
    }
}

void
RAIL_DB::Delete(const RAIL_KEY::MIDI &key)
{
    if (midi_raildata.contains(key)) {
        auto id = midi_raildata[key];
        midi_raildata.erase(key);
        offset.erase(key.port_name);
        if (meta.contains(id)) {
            auto &vec = meta[id];
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                if (auto metakey = std::get_if<RAIL_KEY::MIDI>(&it->key)) {
                    if (*metakey == key) {
                        vec.erase(it);
                        break;
                    }
                }
            }
        }
    }
}

bool
RAIL_DB::Empty()
{
    return kb_mouse_raildata.empty() && midi_raildata.empty();
}

void
RAIL_DB::Clear()
{
    kb_mouse_raildata.clear();
    midi_raildata.clear();
    offset.clear();
    meta.clear();
}

std::optional<uint64_t>
RAIL_DB::GetID(const RAIL_KEY::KB_MOUSE &key)
{
    auto it = kb_mouse_raildata.find(key);
    if (it == kb_mouse_raildata.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

std::optional<uint64_t>
RAIL_DB::GetID(const RAIL_KEY::MIDI &key)
{
    auto it = midi_raildata.find(key);
    if (it == midi_raildata.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

std::optional<std::vector<RAIL_KEY::META>>
RAIL_DB::GetMETA(const uint64_t id)
{
    auto it = meta.find(id);
    if (it == meta.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

}; // namespace PDJE_JUDGE