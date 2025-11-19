#pragma once
#include "EditorArgs.hpp"
#include "jsonWrapper.hpp"
#include <filesystem>
#include <string>

using STRVIEW = const std::string &;
static inline void
ConfigMusicJsonData(PDJE_JSONHandler<MUSIC_W> &jsonHandle,
                    STRVIEW                    title,
                    STRVIEW                    composer,
                    STRVIEW                    firstBeat,
                    const fs::path            &location)
{
    jsonHandle[PDJE_JSON_TITLE]      = title;
    jsonHandle[PDJE_JSON_COMPOSER]   = composer;
    jsonHandle[PDJE_JSON_FIRST_BEAT] = firstBeat;
    jsonHandle[PDJE_JSON_PATH] =
        fs::absolute(location).lexically_normal().generic_string();
}

static inline std::string
GetTitle(PDJE_JSONHandler<MUSIC_W> &jsonHandle)
{
    return jsonHandle[PDJE_JSON_TITLE];
}
static inline std::string
GetComposer(PDJE_JSONHandler<MUSIC_W> &jsonHandle)
{
    return jsonHandle[PDJE_JSON_COMPOSER];
}
static inline void
SetFirstBeat(PDJE_JSONHandler<MUSIC_W> &jsonHandle, STRVIEW firstBeat)
{
    jsonHandle[PDJE_JSON_FIRST_BEAT] = firstBeat;
}
static inline std::string
GetFirstBeat(PDJE_JSONHandler<MUSIC_W> &jsonHandle)
{
    return jsonHandle[PDJE_JSON_FIRST_BEAT];
}
static inline std::string
GetMusicABSLocation(PDJE_JSONHandler<MUSIC_W> &jsonHandle)
{
    return jsonHandle[PDJE_JSON_PATH];
}
