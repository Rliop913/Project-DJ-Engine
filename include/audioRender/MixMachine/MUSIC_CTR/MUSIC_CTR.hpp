#pragma once

#include "DeckData.hpp"

struct Control{
    Position Pause;
};


using TITLE = STR;
using COMPOSER = STR;
using __BPM__ = double;

struct Load{
    std::vector<Values<TITLE, COMPOSER, __BPM__>> Single;
};

struct Unload{
    Position Single;
};

