#include "MusicControlPannel.hpp"

// #undef HWY_TARGET_INCLUDE
// #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
// #include "hwy/foreach_target.h"
// #include <hwy/highway.h>
#include "Decoder.hpp"
#include "MusicControlPannel-inl.h"

MusicControlPannel::~MusicControlPannel()
{

}

bool
MusicControlPannel::LoadMusic(litedb& ROOTDB, const musdata& Mus)
{
    if(!deck.try_emplace(Mus.title).second){
        return false;
    }
    return deck[Mus.title].dec.init(ROOTDB, Mus.musicPath);
}


bool
MusicControlPannel::CueMusic(const UNSANITIZED& title, const unsigned long long newPos)
{
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
    if(!safeTitle){
        return false;
    }
    if(deck.find(safeTitle.value()) == deck.end()){
        return false;
    }
    deck[safeTitle.value()].dec.changePos(newPos * CHANNEL);
    return true;
}



bool
MusicControlPannel::SetMusic(const UNSANITIZED& title, const bool onOff)
{
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
    if(!safeTitle){
        return false;
    }
    if(deck.find(safeTitle.value()) == deck.end()){
        return false;
    }
    deck[safeTitle.value()].play = onOff;
    return true;
}


LOADED_LIST
MusicControlPannel::GetLoadedMusicList()
{
    LOADED_LIST list;
    for(auto& i : deck){
        UNSANITIZED originTitle = PDJE_Name_Sanitizer::getFileName(i.first);
        list.push_back(originTitle);
    }
    return std::move(list);
}


bool
MusicControlPannel::UnloadMusic(const UNSANITIZED& title)
{
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
    if(!safeTitle){
        return false;
    }
    return deck.erase(safeTitle.value()) != 0;
}



HWY_EXPORT(GetPCMFramesSIMD);

bool
MusicControlPannel::GetPCMFrames(float* array, const unsigned long FrameSize)
{
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
}

FXControlPannel*
MusicControlPannel::getFXHandle(const UNSANITIZED& title)
{
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
    if(!safeTitle){
        return nullptr;
    }
    if(deck.find(safeTitle.value()) == deck.end()){
        return nullptr;
    }
    else{
        return deck[safeTitle.value()].fxP;

    }
}
