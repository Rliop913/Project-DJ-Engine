#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "NoteBinary.capnp.h"

#include "FrameCalc.hpp"

class NoteTranslator{
public:
    BpmStruct bpms;
    bool Read(const CapReader<NoteBinaryCapnpData>& binary);

    NoteTranslator() = default;
    ~NoteTranslator() = default;
};