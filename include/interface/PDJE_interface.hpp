#pragma once

#include "audioPlayer.hpp"
#include "dbRoot.hpp"


enum PLAY_MODE{
    FULL_PRE_RENDER,
    HYBRID_RENDER,
    FULL_MANUAL_RENDER
};

class PDJE{
private:
    std::optional<litedb> DBROOT;
public:
    PDJE(const std::string& rootPath);
    ~PDJE();

    std::optional<audioPlayer> player;

    bool
    InitPlayer(
        PLAY_MODE mode, 
        trackdata& td, 
        const unsigned int FrameBufferSize);

    
    MAYBE_MUS_VEC 
    SearchMusic(
        const std::string& Title, 
        const std::string& composer, 
        const double bpm = -1);
    
    MAYBE_TRACK_VEC SearchTrack(const std::string& Title);


};