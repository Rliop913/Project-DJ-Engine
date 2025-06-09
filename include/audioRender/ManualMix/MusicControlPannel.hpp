#pragma once

#include <miniaudio.h>

#include <map>

#include "ManualMix.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "dbRoot.hpp"
#include <filesystem>
namespace fs = std::filesystem;
// #undef HWY_TARGET_INCLUDE
// #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
// #include "hwy/foreach_target.h"
// #include <hwy/highway.h>

using TITLE         = std::u8string;

using LOADED_LIST   = std::vector<TITLE>;





/**
 * @brief Music handling data structure for manual mode
 * 
 */
struct MusicOnDeck{
    bool play = false;
    ma_decoder dec;
    FXControlPannel* fxP;
    MusicOnDeck() : fxP(new FXControlPannel(48000)) {};
    ~MusicOnDeck(){
        ma_decoder_uninit(&dec);
        delete fxP;
    }

};



using LOADS         = std::map<TITLE, MusicOnDeck>;

/**
 * @brief Music handler for manual mode
 * 
 */
class PDJE_API MusicControlPannel{
private:

    LOADS deck; 
    unsigned long fsize;
    std::vector<float> L;
    std::vector<float> R;
    float* FaustStyle[2];
    SIMD_FLOAT tempFrames;

public:
    /**
     * @brief loads music to the deck. doesn't play music
     * 
     * @param Mus Searched music
     * @return int, miniaudio Error code.
     */
    int LoadMusic(litedb& ROOTDB, const musdata& Mus);

    /**
     * @brief Change playback position of the music
     * 
     * @param title the music title
     * @param newPos the new playback position of the music
     * @return true 
     * @return false 
     */
    bool CueMusic(const TITLE& title, const unsigned long long newPos);

    /**
     * @brief turn on, off the music
     * 
     * @param title the music title
     * @param onOff True is on, False is off
     * @return true 
     * @return false 
     */
    bool SetMusic(const TITLE& title, const bool onOff);

    /**
     * @brief get music list on the deck
     * 
     * @return LOADED_LIST
     */
    LOADED_LIST GetLoadedMusicList();

    /**
     * @brief unload music from deck. used to prevent memory leaks.
     * 
     * @param title the target music title
     * @return true 
     * @return false 
     */
    bool UnloadMusic(const TITLE& title);

    /**
     * @brief gets decoded pcm frames
     * 
     * @param array pcm array output
     * @param FrameSize amount of frames
     * @return true 
     * @return false 
     */
    bool GetPCMFrames(float* array, const unsigned long FrameSize);
    
    /**
     * @brief gets FX handler
     * 
     * @param title the title of the music
     * @return FXControlPannel*, the handler pointer
     */
    FXControlPannel* getFXHandle(const TITLE& title);
    MusicControlPannel(const unsigned long FrameSize): fsize(FrameSize){}
    ~MusicControlPannel();

};


// HWY_BEFORE_NAMESPACE();

// namespace hwy{
// namespace HWY_NAMESPACE {

// bool
// GetPCMFramesSIMD(
//     SIMD_FLOAT& tempFrames,
//     std::vector<float>& L,
//     std::vector<float>& R,
//     float** FaustStyle,
//     LOADS& deck,
//     float* array, 
//     const unsigned long FrameSize)
// {
//     const unsigned long long RAWFrameSize = FrameSize * CHANNEL;
    
//     tempFrames.resize(RAWFrameSize);
//     L.resize(FrameSize);
//     R.resize(FrameSize);
//     FaustStyle[0] = L.data();
//     FaustStyle[1] = R.data();
//     const hn::ScalableTag<float> hwyFTag;
//     auto laneSize = hn::Lanes(hwyFTag);
//     auto times = RAWFrameSize / laneSize;
//     auto remained = RAWFrameSize % laneSize;

//     for(auto& i : deck){
//         if(i.second.play){
            
//             if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
//                 return false;
//             }
//             toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
//             i.second.fxP->addFX(FaustStyle, FrameSize);
//             toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);
            
//             float* opoint = array;
//             float* tpoint = tempFrames.data();
            
//             for(size_t j = 0; j < times; ++j){
//                 auto simdtemp = hn::Load(hwyFTag, tpoint);
//                 auto simdorigin = hn::LoadU(hwyFTag, opoint);
//                 auto res = simdtemp + simdorigin;
//                 hn::StoreU(res, hwyFTag, opoint);
//                 opoint += laneSize;
//                 tpoint += laneSize;
//             }
            
//             for(size_t j=0; j<remained; ++j){
//                 (*(opoint++)) += (*(tpoint++));
//             }
//         }
//     }
//     return true;
// }
// }
// }


// HWY_AFTER_NAMESPACE();