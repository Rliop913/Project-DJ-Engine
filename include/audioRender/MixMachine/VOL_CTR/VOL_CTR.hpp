#pragma once

#include "DeckData.hpp"

using VOLUME_VALUE = double;
using VOLUME_VALUE_INTERPOLATE = double;
using ON_OFF = bool;


struct Vol{
    std::vector<Values<VOLUME_VALUE, VOLUME_VALUE_INTERPOLATE, ON_OFF>> Trim;
    std::vector<Values<VOLUME_VALUE, VOLUME_VALUE_INTERPOLATE, ON_OFF>> Fader;
};

struct VOL_CTR{
    Vol vols;
};