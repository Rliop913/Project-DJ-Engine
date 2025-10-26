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
    uint64_t       beat      = 0;
    uint64_t       subBeat   = 0;
    uint64_t       separate  = 0;
    uint64_t       Ebeat     = 0;
    uint64_t       EsubBeat  = 0;
    uint64_t       Eseparate = 0;
};

/// Arguments describing a note entry
struct PDJE_API NoteArgs {
    SANITIZED_ORNOT Note_Type   = "";
    uint16_t        Note_Detail = 0;
    SANITIZED_ORNOT first       = "";
    SANITIZED_ORNOT second      = "";
    SANITIZED_ORNOT third       = "";
    uint64_t       beat        = 0;
    uint64_t       subBeat     = 0;
    uint64_t       separate    = 0;
    uint64_t       Ebeat       = 0;
    uint64_t       EsubBeat    = 0;
    uint64_t       Eseparate   = 0;
    uint64_t        railID      = 0;
};

/// Arguments describing a music entry
struct PDJE_API MusicArgs {
    DONT_SANITIZE bpm      = "";
    uint64_t     beat     = -1;
    uint64_t     subBeat  = -1;
    uint64_t     separate = -1;
};

using MIX_W   = CapWriter<MixBinaryCapnpData>;
using NOTE_W  = CapWriter<NoteBinaryCapnpData>;
using MUSIC_W = CapWriter<MusicBinaryCapnpData>;

using KEY       = DONT_SANITIZE;
using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
using KV_W      = std::vector<KEY_VALUE>;
