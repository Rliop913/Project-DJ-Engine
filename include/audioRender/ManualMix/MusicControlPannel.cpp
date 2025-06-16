#include "MusicControlPannel.hpp"

// #undef HWY_TARGET_INCLUDE
// #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
// #include "hwy/foreach_target.h"
// #include <hwy/highway.h>
#include "MusicControlPannel-inl.h"

MusicControlPannel::~MusicControlPannel()
{

}

int
MusicControlPannel::LoadMusic(litedb& ROOTDB, const musdata& Mus)
{
    if(!deck.try_emplace(Mus.title).second){
        return -1;
    }

    ma_decoder_config decConf =
        ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
    std::string MPath = 
        (fs::path(ROOTDB.getRoot()).parent_path() / fs::path(Mus.musicPath)).lexically_normal().generic_string();
    
    return
        ma_decoder_init_file(
            MPath.c_str(),
            &decConf,
            &deck[Mus.title].dec
        );
}


bool
MusicControlPannel::CueMusic(const TITLE& title, const unsigned long long newPos)
{
    if(deck.find(title) == deck.end()){
        return false;
    }

    ma_decoder_seek_to_pcm_frame(&deck[title].dec, newPos * CHANNEL);
    return true;
}



bool
MusicControlPannel::SetMusic(const TITLE& title, const bool onOff)
{
    if(deck.find(title) == deck.end()){
        return false;
    }
    deck[title].play = onOff;
    return true;
}


LOADED_LIST
MusicControlPannel::GetLoadedMusicList()
{
    LOADED_LIST list;
    for(auto& i : deck){
        list.push_back(i.first);
    }
    return std::move(list);
}


bool
MusicControlPannel::UnloadMusic(const TITLE& title)
{
    return deck.erase(title) != 0;
}



HWY_EXPORT(GetPCMFramesSIMD);

bool
MusicControlPannel::GetPCMFrames(float* array, const unsigned long FrameSize)
{
    // using namespace hwy;
    return
    HWY_DYNAMIC_DISPATCH(GetPCMFramesSIMD)(
        tempFrames,
        L,
        R,
        FaustStyle,
        deck,
        array,
        FrameSize
    );
    // const unsigned long long RAWFrameSize = FrameSize * CHANNEL;

    // tempFrames.resize(RAWFrameSize);
    // L.resize(FrameSize);
    // R.resize(FrameSize);
    // FaustStyle[0] = L.data();
    // FaustStyle[1] = R.data();
    // const hn::ScalableTag<float> hwyFTag;
    // auto laneSize = hn::Lanes(hwyFTag);
    // auto times = RAWFrameSize / laneSize;
    // auto remained = RAWFrameSize % laneSize;

    // for(auto& i : deck){
    //     if(i.second.play){

    //         if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
    //             return false;
    //         }
    //         toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
    //         i.second.fxP->addFX(FaustStyle, FrameSize);
    //         toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);

    //         float* opoint = array;
    //         float* tpoint = tempFrames.data();

    //         for(size_t j = 0; j < times; ++j){
    //             auto simdtemp = hn::Load(hwyFTag, tpoint);
    //             auto simdorigin = hn::LoadU(hwyFTag, opoint);
    //             auto res = simdtemp + simdorigin;
    //             hn::StoreU(res, hwyFTag, opoint);
    //             opoint += laneSize;
    //             tpoint += laneSize;
    //         }

    //         for(size_t j=0; j<remained; ++j){
    //             (*(opoint++)) += (*(tpoint++));
    //         }
    //     }
    // }
    // return true;
}

FXControlPannel*
MusicControlPannel::getFXHandle(const TITLE& title)
{
    if(deck.find(title) == deck.end()){
        return nullptr;
    }
    else{
        return deck[title].fxP;

    }
}
