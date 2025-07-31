#include "PDJE_interface.hpp"

#include <iostream>
#include <string>
// #include <NanoLog.hpp>

int 
main()
{
    std::cout<<"editor tester"<<std::endl;
    
    auto engine = new PDJE(std::string("testRoot.db"));

    if(engine->InitEditor("test", "test", "testEditorProject")){
        std::cout<<"init ok"<<std::endl;
        bool Flag_Already_has_music = false;
        engine->editor->getAll<EDIT_ARG_MUSIC>([&Flag_Already_has_music](const EDIT_ARG_MUSIC& margs){
            if(margs.musicName == "testMiku"){
                Flag_Already_has_music = true;
            }
        });
        if(!Flag_Already_has_music){
            if(engine->editor->ConfigNewMusic("testMiku", "Camellia", "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav")){
                
                EDIT_ARG_MUSIC temp;
                temp.musicName = "testMiku";
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
                loadMusic.first="testMiku";
                loadMusic.second="Camellia";
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
            if(engine->editor->ConfigNewMusic("ヒアソビ", "Camellia", "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav")){
                EDIT_ARG_MUSIC temp;
                temp.musicName = "ヒアソビ";
                temp.arg.bar=0;
                temp.arg.beat=0;
                temp.arg.separate=4;
                temp.arg.bpm="134";
                engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);
            }
        }
        if(engine->SearchMusic("testMiku", "Camellia").empty()){
            bool renderRes = engine->editor->render("testTrack", *(engine->DBROOT));
            bool pushRes = engine->editor->pushToRootDB(*(engine->DBROOT), "testMiku", "Camellia");
            bool pushResSecond = engine->editor->pushToRootDB(*(engine->DBROOT), "ヒアソビ", "Camellia");
            bool trackPushRes = engine->editor->pushToRootDB(*(engine->DBROOT), "testTrack");
            if(pushRes) std::cout << "pushRes ok" << std::endl;
            if(renderRes) std::cout << "renderRes ok" << std::endl;
            if(trackPushRes) std::cout << "trackPushRes ok" << std::endl;
            if(pushResSecond) std::cout << "pushResSecond ok" << std::endl;
            if(pushRes && renderRes && trackPushRes && pushResSecond) std::cout << "push ok" << std::endl;
            else std::cout << "push failed" << std::endl;
        }
        trackdata td;
        td = engine->SearchTrack("testTrack").front();
        
        auto initres = engine->InitPlayer(PLAY_MODE::HYBRID_RENDER, td, 48);
        auto activeres = engine->player->Activate();
        auto musPannel = engine->player->GetMusicControlPannel();
        auto muses = engine->SearchMusic("ヒアソビ", "Camellia");
        musPannel->LoadMusic(*(engine->DBROOT), muses.front());
        
        getchar();
        musPannel->SetMusic("ヒアソビ", true);
        
        // musPannel->
        getchar();
        auto Fxhandle = musPannel->getFXHandle("ヒアソビ");
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