#pragma once

#include "DeckData.hpp"

struct Control{
    Position Pause;
};


using TITLE = STR;
using COMPOSER = STR;
using __BPM__ = double;
using SPEED = double;
using ON_OFF = bool;
using __VOID__ = bool;
using START_FRAME = unsigned long;
using END_FRAME = unsigned long;
struct Load{
    std::vector<Values<TITLE, COMPOSER, __BPM__>> Single;
};

struct Unload{
    Position Single;
};

struct BattleDJ{
    std::vector<Values<SPEED, ON_OFF, __VOID__>> Spin;
    std::vector<Values<SPEED, ON_OFF, __VOID__>> BackSpin;
    std::vector<Values<SPEED, ON_OFF, __VOID__>> Rev;
    std::vector<Values<START_FRAME, SPEED, END_FRAME>> SCRATCH;
    std::vector<Values<START_FRAME, SPEED, END_FRAME>> BackSCRATCH;  
};


struct MUSIC_CTR{
    Control controls;
    Load loads;
    Unload unloads;
    BattleDJ battles;
};