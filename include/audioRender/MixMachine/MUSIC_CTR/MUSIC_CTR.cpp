#include "MUSIC_CTR.hpp"

MUSIC_CTR::MUSIC_CTR()
{
    st.emplace();
    st->setChannels(CHANNEL);
    st->setSampleRate(SAMPLERATE);
    st->setSetting(0, 1);
    st->setSetting(2, 0);
    st->setTempo(1.0);
    D.emplace();
}

void
MUSIC_CTR::ChangeBpm(double bpm)
{
    st->setTempo(bpm / originBpm.value());
}


bool
MUSIC_CTR::checkUsable()
{
    if(!PausePos.has_value() && FullPos.has_value()){
        PausePos = FullPos.value();
    }
    return
    StartPos.has_value() &&
    FullPos.has_value() &&
    originBpm.has_value() &&
    FirstBarPos.has_value() &&
    D.has_value() &&
    songPath.has_value();
}

bool
MUSIC_CTR::TimeStretch(const unsigned long Frame, float*& masterPTR)
{
    
    const unsigned long Sola = Frame / st->getInputOutputSampleRatio();
    if(!D->getRange(Sola, solaBuffer)){
        return false;
    }
    st->putSamples(solaBuffer.data(), Sola);
    st->receiveSamples(masterPTR, Frame);
    masterPTR+=(Frame * CHANNEL);
    return true;
}

bool
MUSIC_CTR::Render(const double bpm, const unsigned long FrameRange, float*& masterPTR)
{
    ChangeBpm(bpm);
    unsigned long ItrTimes = FrameRange / BPM_WINDOWS_SIZE;
    unsigned long remainLast = FrameRange % BPM_WINDOWS_SIZE;
    for(auto j=0; j<ItrTimes-1; ++j){
        if(!TimeStretch(BPM_WINDOWS_SIZE, masterPTR)){
            return false;
        }
    }
    if(!TimeStretch(BPM_WINDOWS_SIZE + remainLast, masterPTR)){
        return false;
    }
    return true;
}


std::optional<std::vector<float>>
MUSIC_CTR::Execute(const BPM& bpms)
{
    if(!checkUsable()){
        return std::nullopt;
    }
    if(!D->init(songPath.value())){
        return std::nullopt;
    }
    BpmStruct S;
    BpmStruct P;
    BpmStruct F;
    S.frame_to_here = StartPos.value();
    P.frame_to_here = PausePos.value();
    F.frame_to_here = FullPos.value();
    unsigned long RfullFrameSize = (FullPos.value() - StartPos.value()) * CHANNEL;

    std::vector<float> PCMS(RfullFrameSize);
    auto masterPTR = PCMS.data();
    
    auto idxGetter = [](const BpmStruct& first, const BpmStruct& second){
            return first.frame_to_here < second.frame_to_here;
        };
    auto StartItr = std::upper_bound(
        bpms.bpmVec.begin(),
        bpms.bpmVec.end(),
        S,
        idxGetter
    ); --StartItr;
    auto PauseItr = std::upper_bound(
        bpms.bpmVec.begin(),
        bpms.bpmVec.end(),
        P,
        idxGetter
    ); --PauseItr;

    if(!D->changePos(FirstBarPos.value())){
        return std::nullopt;
    }
    for(auto i = StartItr; i != PauseItr; ++i){
        auto next = i + 1;
        unsigned long FrameRange =
        next->frame_to_here -
        (
            i->frame_to_here < S.frame_to_here ?
                S.frame_to_here :
                i->frame_to_here
        );
        if(!Render(i->bpm, FrameRange, masterPTR)){
            return std::nullopt;
        }
    }
    unsigned int LastRange =
    P.frame_to_here -
    (
        PauseItr->frame_to_here < S.frame_to_here ?
            S.frame_to_here :
            PauseItr->frame_to_here
    );
    if(!Render(PauseItr->bpm, LastRange, masterPTR)){
        return std::nullopt;
    }
    
    return PCMS;
}


MUSIC_CTR::~MUSIC_CTR()
{
    ;
}