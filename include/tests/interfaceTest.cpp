#include "PDJE_interface.hpp"

#include <iostream>
int 
main()
{
    auto testpdje = new PDJE("./tempdb.db");
    auto searchResult = testpdje->SearchMusic(u8"WTC", u8"");
    if(searchResult.empty()){
        std::cout << "can't search" << std::endl;
        delete testpdje;
        return 1;
    }
    for(auto i : searchResult){
        std::cout << "title: " << std::string(i.title.begin(), i.title.end()) 
        << "path: "<< std::string(i.musicPath.begin(), i.musicPath.end()) << std::endl;
    }

    auto trackSearch = testpdje->SearchTrack(u8"testmix111");
    if(trackSearch.empty()){
        std::cout << "can't search track" << std::endl;
        delete testpdje;
        return 1;
    }
    for(auto i: trackSearch){
        std::cout 
        << " track title: "
        << TO_STR(i.trackTitle)
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
    mus->LoadMusic(testpdje->DBROOT.value(), searchResult[0]);
    std::cout << TO_STR(mus->GetLoadedMusicList()[0]);
    mus->SetMusic(u8"WTC", true);
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