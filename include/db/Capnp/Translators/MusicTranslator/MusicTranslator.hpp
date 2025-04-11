#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "MusicBinary.capnp.h"

#include "FrameCalc.hpp"

/**
 * @brief This Translate capnp data into cpp data sturct.
 * \dot
 * digraph MusicTranslator{
 *      MusicTranslator -> BpmStruct
 * }
 * \enddot
 */
class MusicTranslator{
public:
    BpmStruct bpms;
    /**
     * @brief reads capnp music binary data and fills BpmStruct.
     * 
     * @param binary capnp binary data
     * @param startFrame the start beat's frame position.
     * @return true 
     * @return false 
     */
    bool Read(const CapReader<MusicBinaryCapnpData>& binary, unsigned long long startFrame);

    MusicTranslator() = default;
    ~MusicTranslator() = default;
};