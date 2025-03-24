#include "ManualMix.hpp"


MusicControlPannel::MusicControlPannel()
{
// NO_IMPL_CRASH
}

MusicControlPannel::~MusicControlPannel()
{
    
}


int
MusicControlPannel::LoadMusic(const musdata& Mus)
{
    if(!deck.try_emplace(Mus.title).second){
        return -1;
    }

    ma_decoder_config decConf = 
        ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
    
    return 
        ma_decoder_init_file(
            Mus.musicPath.c_str(), 
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
    for(auto i : deck){
        list.push_back(i.first);
    }
    return std::move(list);
}


bool 
MusicControlPannel::UnloadMusic(const TITLE& title)
{
    return deck.erase(title) != 0;
}

bool
MusicControlPannel::GetPCMFrames(std::vector<float, hwy::AlignedAllocator<float>>& array, unsigned long FrameSize)
{
    unsigned long long RAWFrameSize = FrameSize * CHANNEL;
    
    std::vector<float, hwy::AlignedAllocator<float>> tempFrames(RAWFrameSize);
    
    const hn::ScalableTag<float> hwyFTag;
    auto laneSize = hn::Lanes(hwyFTag);
    auto times = RAWFrameSize / laneSize;
    auto remained = RAWFrameSize % laneSize;

    for(auto i : deck){
        if(i.second.play){
            
            if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
                return false;
            }
            float* opoint = array.data();
            float* tpoint = tempFrames.data();
            
            for(size_t j = 0; j < times; ++j){
                auto simdtemp = hn::Load(hwyFTag, tpoint);
                auto simdorigin = hn::Load(hwyFTag, opoint);
                auto res = simdtemp + simdorigin;
                hn::Store(res, hwyFTag, opoint);
                opoint += laneSize;
                tpoint += laneSize;
            }
            
            for(size_t j=0; j<remained; ++j){
                (*(opoint++)) += (*(tpoint++));
            }
        }
    }
    return true;
}
