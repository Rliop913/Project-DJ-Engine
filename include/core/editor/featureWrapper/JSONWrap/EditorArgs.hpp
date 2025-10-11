#pragma once
#include "MixTranslator.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "fileNameSanitizer.hpp"
#include <cstdint>

/// Arguments describing a mix entry
struct PDJE_API MixArgs {
    TypeEnum        type      = TypeEnum::EQ;
    DetailEnum      details   = DetailEnum::HIGH;
    int             ID        = -1;
    SANITIZED_ORNOT first     = "";
    SANITIZED_ORNOT second    = "";
    SANITIZED_ORNOT third     = "";
    long long       beat      = -1;
    long long       subBeat   = -1;
    long long       separate  = -1;
    long long       Ebeat     = -1;
    long long       EsubBeat  = -1;
    long long       Eseparate = -1;
};

/// Arguments describing a note entry
struct PDJE_API NoteArgs {
    SANITIZED_ORNOT Note_Type   = "";
    SANITIZED_ORNOT Note_Detail = "";
    SANITIZED_ORNOT first       = "";
    SANITIZED_ORNOT second      = "";
    SANITIZED_ORNOT third       = "";
    long long       beat        = -1;
    long long       subBeat     = -1;
    long long       separate    = -1;
    long long       Ebeat       = -1;
    long long       EsubBeat    = -1;
    long long       Eseparate   = -1;
    uint64_t        railID      = 0;
};

/// Arguments describing a music entry
struct PDJE_API MusicArgs {
    DONT_SANITIZE bpm      = "";
    long long     beat     = -1;
    long long     subBeat  = -1;
    long long     separate = -1;
};

using MIX_W   = CapWriter<MixBinaryCapnpData>;
using NOTE_W  = CapWriter<NoteBinaryCapnpData>;
using MUSIC_W = CapWriter<MusicBinaryCapnpData>;

using KEY       = DONT_SANITIZE;
using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
using KV_W      = std::vector<KEY_VALUE>;
