#pragma once

#include <optional>

#include <SoundTouch.h>

#include "dbRoot.hpp"
#include "DeckData.hpp"
#include "MixTranslator.hpp"
#include "Decoder.hpp"

constexpr unsigned long BPM_WINDOWS_SIZE = SAMPLERATE / 1000;

#define GET_BIGGER(A, B) (A < B ? B : A)


class MUSIC_CTR{
private:
    std::optional<soundtouch::SoundTouch> st;
    std::optional<Decoder> D;
    std::vector<float> timeStretchBuffer;
    bool checkUsable();
    bool TimeStretch(const unsigned long Frame, float*& masterPTR);
    bool Render(const double bpm, const unsigned long FrameRange, float*& masterPTR);
    void ChangeBpm(double bpm);
public:
    MUSIC_CTR();
    ~MUSIC_CTR();
    bool setLOAD(MBData::Reader& RP, litedb& db, unsigned long FrameIn);
    std::optional<unsigned long> StartPos;
    std::optional<unsigned long> PausePos;
    std::optional<unsigned long> FullPos;
    std::optional<unsigned long> FirstBarPos;
    std::optional<double> originBpm;
    std::optional<std::string> songPath;

    std::optional<std::vector<float>*> 
    Execute(const BPM& bpms, std::vector<float>* PCMS);
    bool SendData(soundtouch::SoundTouch*& stp, Decoder*& dp);
};
