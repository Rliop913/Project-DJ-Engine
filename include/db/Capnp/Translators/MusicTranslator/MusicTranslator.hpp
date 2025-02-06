#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "MusicBinary.capnp.h"

#include "FrameCalc.hpp"

class MusicTranslator{
public:
    BpmStruct bpms;
    bool Read(const CapReader<MusicBinaryCapnpData>& binary, unsigned long long startFrame);

    MusicTranslator() = default;
    ~MusicTranslator() = default;
};