#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "MusicBinary.capnp.h"

#include "FrameCalc.hpp"

class MusicTranslator{
public:
    BpmStruct bpms;
    bool Read(const CapReader<MusicBinaryCapnpData>& binary);

    MusicTranslator() = default;
    ~MusicTranslator() = default;
};