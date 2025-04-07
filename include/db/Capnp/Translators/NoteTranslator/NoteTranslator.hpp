#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "NoteBinary.capnp.h"

#include "FrameCalc.hpp"

#include <functional>



/*
Args describe

1> noteType, the note type. e.g) hold, tap, slide
2> noteDetail, the detail
3> firstArg
4> secondArg
5> thirdArg

1~5 these are meta datas for note.

6> Y axis position, the first logical position of note
7> Y axis position 2, the second logical position of note, use for long notes or else

*/
using OBJ_SETTER_CALLBACK = 
std::function<void(
    const std::string, //noteType
    const std::string, //noteDetail
    const std::string, //firstArg
    const std::string, //secondArg
    const std::string, //thirdArg
    const unsigned long long, //Y axis position
    const unsigned long long //Y axis position 2, 0 as default
)>;



class NoteTranslator{
public:
    BpmStruct noteBpms;
    bool Read(
        const CapReader<NoteBinaryCapnpData>& binary, 
        const BpmStruct& mainBpm,
        OBJ_SETTER_CALLBACK& lambdaCallback);

    NoteTranslator() = default;
    ~NoteTranslator() = default;
};