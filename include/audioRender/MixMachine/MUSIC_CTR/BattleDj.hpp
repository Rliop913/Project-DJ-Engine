#pragma once

#include "SoundTouch.h"

#include <optional>
#include <vector>

#include "MixTranslator.hpp"
#include "MUSIC_CTR.hpp"

enum SoundTouchType{
    MASTER,
    RAW,
    PITCH
};

struct DJ_JOBS{
    unsigned long sourcePoint;
    unsigned long attachIn;
    unsigned long attachOut;
    float SPEED = 1.0;
    SoundTouchType STT;
    bool getFromOrigin = true;
};

class BattleDj{
private:
    soundtouch::SoundTouch* st;
    Decoder* D;
    std::vector<DJ_JOBS> jobs;
public:
    BattleDj();
    ~BattleDj();
    bool GetDataFrom(MUSIC_CTR& mc);
    bool Spin(MixStruct& ms);
    bool Rev(MixStruct& ms);
    bool Scratch(MixStruct& ms);
    bool Pitch(MixStruct& ms);
    std::optional<unsigned long> StartPos;

    std::optional<SIMD_FLOAT*>
    operator<<(std::optional<SIMD_FLOAT*> Array);
};
