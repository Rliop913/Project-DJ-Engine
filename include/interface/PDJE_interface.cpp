#include "PDJE_interface.hpp"

PDJE::PDJE(const std::string& rootPath)
{
    DBROOT.emplace();
    DBROOT->openDB(rootPath);
    
}


MAYBE_TRACK_VEC
PDJE::SearchTrack(const std::string& Title)
{
    trackdata td;
    td.trackTitle = Title;
    return DBROOT.value() << td;
}


MAYBE_MUS_VEC
PDJE::SearchMusic(
    const std::string& Title, 
    const std::string& composer, 
    const double bpm)
{
    musdata md;
    md.title = Title;
    md.composer = composer;
    md.bpm = bpm;
    return DBROOT.value() << md;
}

bool
PDJE::InitPlayer(
    PLAY_MODE mode, 
    trackdata& td, 
    const unsigned int FrameBufferSize)
{
    switch (mode)
    {
    case PLAY_MODE::FULL_PRE_RENDER:
        player.emplace(
            DBROOT.value(),
            td,
            FrameBufferSize,
            false
            );
        break;
    case PLAY_MODE::HYBRID_RENDER:
        player.emplace(
            DBROOT.value(),
            td,
            FrameBufferSize,
            true
        );
        break;
    case PLAY_MODE::FULL_MANUAL_RENDER:
        player.emplace(
            FrameBufferSize
        );
        break;
    
    default:
        break;
    }

    if(!player.has_value()){
        return false;
    }
    else{
        return true;
    }
    
}