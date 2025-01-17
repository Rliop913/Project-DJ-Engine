#pragma once

#include <optional>

#include <SoundTouch.h>

#include "dbRoot.hpp"
#include "DeckData.hpp"
#include "MixTranslator.hpp"
#include "Decoder.hpp"
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


constexpr unsigned long BPM_WINDOWS_SIZE = SAMPLERATE / 1000;

class MUSIC_CTR{
private:
    std::optional<soundtouch::SoundTouch> st;
    std::optional<Decoder> D;
    std::vector<float> solaBuffer;
    bool checkUsable();
    bool TimeStretch(const unsigned long Frame, float*& masterPTR);
    bool Render(const double bpm, const unsigned long FrameRange, float*& masterPTR);
    void ChangeBpm(double bpm);
public:
    MUSIC_CTR();
    ~MUSIC_CTR();
    std::optional<unsigned long> StartPos;
    std::optional<unsigned long> PausePos;
    std::optional<unsigned long> FullPos;
    std::optional<unsigned long> FirstBarPos;
    std::optional<double> originBpm;
    std::optional<std::string> songPath;

    std::optional<std::vector<float>> 
    Execute(const BPM& bpms);
};