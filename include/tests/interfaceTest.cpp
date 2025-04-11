#include "PDJE_interface.hpp"

#include <iostream>
int 
main()
{
    auto testpdje = new PDJE("./tempdb.db");
    auto searchResult = testpdje->SearchMusic("WTC", "");
    if(searchResult.empty()){
        std::cout << "can't search" << std::endl;
        delete testpdje;
        return 1;
    }
    for(auto i : searchResult){
        std::cout << "title: " << i.title 
        << "path: "<< i.musicPath << std::endl;
    }

    auto trackSearch = testpdje->SearchTrack("testmix111");
    if(trackSearch.empty()){
        std::cout << "can't search track" << std::endl;
        delete testpdje;
        return 1;
    }
    for(auto i: trackSearch){
        std::cout 
        << " track title: "
        << i.trackTitle
        << " note binary size: "
        << i.noteBinary.size()
        << " mix binary size: "
        << i.mixBinary.size()
        << std::endl;
    }
    testpdje->InitPlayer(PLAY_MODE::HYBRID_RENDER, trackSearch[0], 48);
    if(!testpdje->player.has_value()){
        std::cout << "can't search track" << std::endl;
        delete testpdje;
        return 1;
    }
    testpdje->player->Activate();
    getchar();
    testpdje->player->GetFXControlPannel()->FX_ON_OFF(FXList::DISTORTION, true);
    auto pannel = testpdje->player->GetFXControlPannel()->GetArgSetter(FXList::DISTORTION);
    pannel["distortionValue"](2);
    
    getchar();
    auto mus = testpdje->player->GetMusicControlPannel();
    mus->LoadMusic(searchResult[0]);
    std::cout << mus->GetLoadedMusicList()[0];
    mus->SetMusic("WTC", true);
    getchar();
    for(auto i : (pannel)){
        std::cout 
        << i.first
        << " "
        << std::endl;
    }
    testpdje->player->Deactivate();
    getchar();
    delete testpdje;
    return 0;
}