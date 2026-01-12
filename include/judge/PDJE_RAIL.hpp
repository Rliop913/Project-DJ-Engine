#pragma once
#include "PDJE_Rule.hpp"

static inline uint64_t
splitmix64_mix(uint64_t x) noexcept
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

static inline void
hash_combine64(uint64_t &seed, uint64_t v) noexcept
{
    seed = splitmix64_mix(seed ^ v);
}

template <class T>
static inline uint64_t
h64(const T &v) noexcept
{
    return static_cast<uint64_t>(std::hash<T>{}(v));
}
namespace std {
template <> struct std::hash<PDJE_JUDGE::RAIL_KEY::KB_MOUSE> {
    std::size_t
    operator()(const PDJE_JUDGE::RAIL_KEY::KB_MOUSE &rule) const noexcept
    {
        uint64_t seed = 0;
        hash_combine64(seed, h64(rule.Device_Name));
        hash_combine64(seed, h64(rule.DeviceKey));
        return static_cast<size_t>(seed);
    }
};

template <> struct std::hash<PDJE_JUDGE::RAIL_KEY::MIDI> {
    std::size_t
    operator()(const PDJE_JUDGE::RAIL_KEY::MIDI &rule) const noexcept
    {
        uint64_t seed = 0;
        hash_combine64(seed, h64(rule.ch));
        hash_combine64(seed, h64(rule.port_name));
        hash_combine64(seed, h64(rule.pos));
        hash_combine64(seed, h64(rule.type));
        return static_cast<size_t>(seed);
    }
};

}; // namespace std

namespace PDJE_JUDGE {
struct RAIL_DB {
    std::unordered_map<std::string, int64_t>                  offset;
    std::unordered_map<uint64_t, std::vector<RAIL_KEY::META>> meta;
    std::unordered_map<RAIL_KEY::KB_MOUSE, uint64_t>          kb_mouse_raildata;
    std::unordered_map<RAIL_KEY::MIDI, uint64_t>              midi_raildata;

    void
    Add(const RAIL_KEY::KB_MOUSE &key,
        const PDJE_Dev_Type      &type,
        const uint64_t           &id,
        const int64_t             offset);
    void
    Add(const RAIL_KEY::MIDI &key, const uint64_t &id, const int64_t offset);
    void
    Delete(const RAIL_KEY::KB_MOUSE &key);
    void
    Delete(const RAIL_KEY::MIDI &key);

    bool
    Empty();
    void
    Clear();

    std::optional<uint64_t>
    GetID(const RAIL_KEY::KB_MOUSE &key);
    std::optional<uint64_t>
    GetID(const RAIL_KEY::MIDI &key);

    std::optional<std::vector<RAIL_KEY::META>>
    GetMETA(const uint64_t id);
};

}; // namespace PDJE_JUDGE