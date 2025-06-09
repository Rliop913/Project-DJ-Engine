#include "PDJE_interface.hpp"

#include <iostream>

int 
main()
{
    std::cout<<"editor tester"<<std::endl;
    auto engine = new PDJE(u8"testRoot.db");

    if(engine->InitEditor("test", "test", u8"testEditorProject")){
        std::cout<<"init ok"<<std::endl;
        bool Flag_Already_has_music = false;
        engine->editor->getAll<EDIT_ARG_MUSIC>([&Flag_Already_has_music](const EDIT_ARG_MUSIC& margs){
            if(margs.musicName == u8"testMiku"){
                Flag_Already_has_music = true;
            }
        });
        if(!Flag_Already_has_music){
            if(engine->editor->ConfigNewMusic(u8"testMiku", u8"Camellia", u8"../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav")){
                
                EDIT_ARG_MUSIC temp;
                temp.musicName = u8"testMiku";
                temp.arg.bar=0;
                temp.arg.beat=0;
                temp.arg.separate=4;
                temp.arg.bpm="138";
                engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);

                EDIT_ARG_MIX bpmSet;
                bpmSet.bar=0;
                bpmSet.beat=0;
                bpmSet.type=TypeEnum::BPM_CONTROL;
                bpmSet.details=DetailEnum::TIME_STRETCH;
                bpmSet.separate=4;
                bpmSet.ID=0;
                bpmSet.first="138";
                engine->editor->AddLine<EDIT_ARG_MIX>(bpmSet);

                EDIT_ARG_MIX loadMusic;
                loadMusic.bar=0;
                loadMusic.beat=0;
                loadMusic.type=TypeEnum::LOAD;
                
                loadMusic.separate=4;
                loadMusic.first=u8"testMiku";
                loadMusic.second=u8"Camellia";
                loadMusic.third="138";
                loadMusic.ID=0;
                engine->editor->AddLine<EDIT_ARG_MIX>(loadMusic);
                
                EDIT_ARG_MIX changeBpm;
                changeBpm.bar=40;
                changeBpm.beat=0;
                changeBpm.type=TypeEnum::BPM_CONTROL;
                changeBpm.details=DetailEnum::TIME_STRETCH;
                changeBpm.separate=4;
                changeBpm.ID=0;
                changeBpm.first="170";
                engine->editor->AddLine<EDIT_ARG_MIX>(changeBpm);

                EDIT_ARG_MIX unloadMusic;
                unloadMusic.bar=200;
                unloadMusic.beat=0;
                unloadMusic.type=TypeEnum::UNLOAD;
                unloadMusic.ID=0;
                unloadMusic.separate=4;
                engine->editor->AddLine<EDIT_ARG_MIX>(unloadMusic);


                std::cout << "config init ok" << std::endl;
            }
            else{
                std::cout << "config init failed" << std::endl;
            }
            if(engine->editor->ConfigNewMusic(u8"ヒアソビ", u8"Camellia", u8"../../DMCA_FREE_DEMO_MUSIC/ヒアソビ（重音テトカバー).wav")){
                EDIT_ARG_MUSIC temp;
                temp.musicName = u8"ヒアソビ";
                temp.arg.bar=0;
                temp.arg.beat=0;
                temp.arg.separate=4;
                temp.arg.bpm="134";
                engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);
            }
        }
        if(engine->SearchMusic(u8"testMiku", u8"Camellia").empty()){
            bool renderRes = engine->editor->render(u8"testTrack", engine->DBROOT.value());
            bool pushRes = engine->editor->pushToRootDB(engine->DBROOT.value(), u8"testMiku", u8"Camellia");
            bool pushResSecond = engine->editor->pushToRootDB(engine->DBROOT.value(), u8"ヒアソビ", u8"Camellia");
            bool trackPushRes = engine->editor->pushToRootDB(engine->DBROOT.value(), u8"testTrack");
            if(pushRes && renderRes && trackPushRes && pushResSecond) std::cout << "push ok" << std::endl;
            else std::cout << "push failed" << std::endl;
        }
        trackdata td;
        td = engine->SearchTrack(u8"testTrack").front();
        
        auto initres = engine->InitPlayer(PLAY_MODE::HYBRID_RENDER, td, 48);
        auto activeres = engine->player->Activate();
        auto musPannel = engine->player->GetMusicControlPannel();
        auto muses = engine->SearchMusic(u8"ヒアソビ", u8"Camellia");
        musPannel->LoadMusic(muses.front());
        
        getchar();
        musPannel->SetMusic(u8"ヒアソビ", true);
        getchar();
        auto Fxhandle = musPannel->getFXHandle(u8"ヒアソビ");
        Fxhandle->FX_ON_OFF(FXList::OCSFILTER, true);
        auto ocshandle = Fxhandle->GetArgSetter(FXList::OCSFILTER);
        ocshandle["ocsFilterHighLowSW"](1);
        ocshandle["rangeFreqHalf"](2500);
        ocshandle["middleFreq"](5000);
        
        ocshandle["bps"](2.2333333);
        ocshandle["OCSFilterDryWet"](0.7);
        getchar();
        auto deactres = engine->player->Deactivate();


        // engine
        
    }
    else{
        std::cout << "init failed " << std::endl;
        delete engine;
        return 1;
    }
    delete engine;
    // std::cout<<engine.InitEditor("test", "test", "./testEditorProject") << std::endl;
    // engine.editor->ConfigNewMusic("testMiku", "Camellia", "")
    return 0;
}