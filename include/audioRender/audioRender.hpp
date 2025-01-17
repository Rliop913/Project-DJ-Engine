#pragma once

#include "MixMachine.hpp"

class audioRender{
private:
    std::optional<std::vector<float>> rendered_frames;
public:

    bool LoadTrack(litedb& db, trackdata& td);
    audioRender();
    ~audioRender();

};