#pragma once

#include "MixMachine.hpp"
#include "ManualMix.hpp"

class audioRender{
public:
    std::optional<std::vector<float>> rendered_frames;

    bool LoadTrack(litedb& db, trackdata& td);
    audioRender() = default;
    ~audioRender() = default;

};