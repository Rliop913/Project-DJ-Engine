#pragma once

#include <miniaudio.h>

#include <map>

#include "ManualMix.hpp"



using TITLE         = std::string;

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
class MusicControlPannel{
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
    int LoadMusic(const musdata& Mus);

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