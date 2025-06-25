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


bool
MUSIC_CTR::SendData(soundtouch::SoundTouch*& stp, Decoder*& dp)
{
    if(st.has_value() && D.has_value()){
        stp = &(st.value());
        dp = &(D.value());
        return true;
    }
    else{
        return false;
    }
}

void
MUSIC_CTR::ChangeBpm(double targetbpm, double originBpm)
{
    st->setTempo(targetbpm / originBpm);
}


bool
MUSIC_CTR::checkUsable()
{
    // if(!PausePos.has_value() && FullPos.has_value()){
    //     PausePos = FullPos.value();
    // }
    return
    D.has_value() &&
    songPath.has_value();
}




bool
MUSIC_CTR::TimeStretch(const FRAME_POS Frame, float*& masterPTR)
{
    
    const FRAME_POS Sola = Frame / st->getInputOutputSampleRatio();
    if(!D->getRange(Sola, timeStretchBuffer)){
        return false;
    }
    st->putSamples(timeStretchBuffer.data(), Sola);
    st->receiveSamples(masterPTR, Frame);
    masterPTR+=(Frame * CHANNEL);
    return true;
}

bool
MUSIC_CTR::Render(
    const double targetBpm,
    const double originBpm, 
    const LOCAL_POS LocalIDX,
    const GLOBAL_POS RenderAmount, 
    float*& masterPTR)
{
    D->changePos(LocalIDX);
    ChangeBpm(targetBpm, originBpm);
    FRAME_POS ItrTimes = RenderAmount / BPM_WINDOWS_SIZE;
    FRAME_POS remainLast = RenderAmount % BPM_WINDOWS_SIZE;
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



std::optional<SIMD_FLOAT*>
MUSIC_CTR::Execute(const BPM& bpms, SIMD_FLOAT* PCMS, const fs::path& dbRoot)
{
    if(!checkUsable()){
        return std::nullopt;
    }
    if(!D->init(songPath.value(), dbRoot)){
        return std::nullopt;
    }
    QDatas.Ready(bpms.bpmVec, Mus.bpms);

    GLOBAL_POS RfullFrameSize = 
        QDatas.pos.back().Gidx - QDatas.pos.front().Gidx;
    

    PCMS->resize(RfullFrameSize * CHANNEL);
    auto masterPTR = PCMS->data();
    for(unsigned int i=0; i<QDatas.pos.size() - 1; ++i){
        if(QDatas.pos[i].status == PLAY){
            GLOBAL_POS range = 
                QDatas.pos[i + 1].Gidx - QDatas.pos[i].Gidx;
            Render(
                QDatas.pos[i].TargetBPM,
                QDatas.pos[i].OriginBPM,
                QDatas.pos[i].Lidx,
                range,
                masterPTR
            );

        }
    }
    
    return PCMS;
}
bool
MUSIC_CTR::setLOAD(MBData::Reader& RP, litedb& db, FRAME_POS FrameIn)
{
    musdata md;
    auto tempTitle = RP.getFirst();
    auto tempComposer = RP.getSecond();
    
    md.title.resize(tempTitle.size());
    std::transform(
        tempTitle.begin(), tempTitle.end(),
        md.title.begin(),
        [](char c){return static_cast<char8_t>(c);}
    );
    //  = TO_USTR(tempTitle);
    md.composer.resize(tempComposer.size());
    std::transform(
        tempComposer.begin(), tempComposer.end(),
        md.composer.begin(),
        [](char c){return static_cast<char8_t>(c);}
    );
    // = TO_USTR(tempComposer);
    md.bpm = std::stod(RP.getThird().cStr());
    
    auto searchRes = db << md;
    if(!searchRes.has_value()){
        return false;
    }
    songPath = fs::path(searchRes.value()[0].musicPath);
    PlayPosition startpos;
    startpos.Gidx = FrameIn;
    
    startpos.Lidx = std::stoull(searchRes.value()[0].firstBar);
    startpos.status = MIXSTATE::PLAY;
    QDatas.pos.push_back(startpos);
    
    if(!capnpMus.open(searchRes.value()[0].bpmBinary)){
        return false;
    }
    if(!Mus.Read(capnpMus, startpos.Lidx)){
        return false;
    }
    return true;
}
