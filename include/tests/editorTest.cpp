#include "PDJE_interface.hpp"

#include "PDJE_Benchmark.hpp"
#include <iostream>
#include <string>
// #include <NanoLog.hpp>

int
main()
{
    std::cout << "editor tester" << std::endl;

    auto engine = new PDJE(std::string("testRoot.db"));

    if (engine->InitEditor("test", "test", "testEditorProject")) {
        std::cout << "init ok" << std::endl;
        bool Flag_Already_has_music = false;
        engine->editor->getAll<EDIT_ARG_MUSIC>(
            [&Flag_Already_has_music](const EDIT_ARG_MUSIC &margs) {
                if (margs.musicName == "testMiku") {
                    Flag_Already_has_music = true;
                }
            });
        if (!Flag_Already_has_music) {
            if (engine->editor->ConfigNewMusic(
                    "testMiku",
                    "Camellia",
                    "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav",
                    "41280")) {

                EDIT_ARG_MUSIC temp;
                temp.musicName    = "testMiku";
                temp.arg.beat     = 0;
                temp.arg.subBeat  = 0;
                temp.arg.separate = 4;
                temp.arg.bpm      = "138";
                engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);

                EDIT_ARG_MIX bpmSet;
                bpmSet.beat     = 0;
                bpmSet.subBeat  = 0;
                bpmSet.type     = TypeEnum::BPM_CONTROL;
                bpmSet.details  = DetailEnum::TIME_STRETCH;
                bpmSet.separate = 4;
                bpmSet.ID       = 0;
                bpmSet.first    = "138";
                engine->editor->AddLine<EDIT_ARG_MIX>(bpmSet);

                EDIT_ARG_MIX loadMusic;
                loadMusic.beat    = 0;
                loadMusic.subBeat = 0;
                loadMusic.type    = TypeEnum::LOAD;

                loadMusic.separate = 4;
                loadMusic.first    = "testMiku";
                loadMusic.second   = "Camellia";
                loadMusic.third    = "138";
                loadMusic.ID       = 0;
                engine->editor->AddLine<EDIT_ARG_MIX>(loadMusic);

                EDIT_ARG_MIX changeBpm;
                changeBpm.beat     = 40;
                changeBpm.subBeat  = 0;
                changeBpm.type     = TypeEnum::BPM_CONTROL;
                changeBpm.details  = DetailEnum::TIME_STRETCH;
                changeBpm.separate = 4;
                changeBpm.ID       = 0;
                changeBpm.first    = "170";
                engine->editor->AddLine<EDIT_ARG_MIX>(changeBpm);

                EDIT_ARG_MIX unloadMusic;
                unloadMusic.beat     = 200;
                unloadMusic.subBeat  = 0;
                unloadMusic.type     = TypeEnum::UNLOAD;
                unloadMusic.ID       = 0;
                unloadMusic.separate = 4;
                engine->editor->AddLine<EDIT_ARG_MIX>(unloadMusic);

                std::cout << "config init ok" << std::endl;
            } else {
                std::cout << "config init failed" << std::endl;
            }
            if (engine->editor->ConfigNewMusic(
                    "ヒアソビ",
                    "Camellia",
                    "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav",
                    "41280")) {
                EDIT_ARG_MUSIC temp;
                temp.musicName    = "ヒアソビ";
                temp.arg.beat     = 0;
                temp.arg.subBeat  = 0;
                temp.arg.separate = 4;
                temp.arg.bpm      = "138";
                engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);
            }
            EDIT_ARG_NOTE notetemp;
            notetemp.railID = 1;
            for (int i = 0; i < 10; ++i) {
                notetemp.beat = i;
                engine->editor->AddLine<EDIT_ARG_NOTE>(notetemp);
            }
            engine->editor->Undo<EDIT_ARG_NOTE>();
            engine->editor->Redo<EDIT_ARG_NOTE>();
        }
        if (engine->SearchMusic("testMiku", "Camellia").empty()) {
            std::string linter_msg;
            bool        renderRes = engine->editor->render(
                "testTrack", *(engine->DBROOT), linter_msg);
            bool pushRes = engine->editor->pushToRootDB(
                *(engine->DBROOT), "testMiku", "Camellia");
            bool pushResSecond = engine->editor->pushToRootDB(
                *(engine->DBROOT), "ヒアソビ", "Camellia");
            bool trackPushRes =
                engine->editor->pushToRootDB(*(engine->DBROOT), "testTrack");
            if (pushRes)
                std::cout << "pushRes ok" << std::endl;
            if (renderRes)
                std::cout << "renderRes ok" << std::endl;
            if (trackPushRes)
                std::cout << "trackPushRes ok" << std::endl;
            if (pushResSecond)
                std::cout << "pushResSecond ok" << std::endl;
            if (pushRes && renderRes && trackPushRes && pushResSecond)
                std::cout << "push ok" << std::endl;
            else
                std::cout << "push failed" << std::endl;

            // std::shared_ptr<audioPlayer> ap;
            // engine->editor->demoPlayInit(ap, 48, "testTrack");
            // if (!ap) {
            //     std::cout << "failed to init demo player. " << std::endl;
            // }
            // if (ap->Activate()) {
            //     std::cout << "Activated demo" << std::endl;
            // }
            // getchar();
            // if (ap->Deactivate()) {
            //     std::cout << "DeActivated demo" << std::endl;
            // }
        }
        trackdata td;
        td             = engine->SearchTrack("testTrack").front();
        auto mode      = PLAY_MODE::HYBRID_RENDER;
        auto initres   = engine->InitPlayer(mode, td, 480);
        auto activeres = engine->player->Activate();
        if (mode == PLAY_MODE::FULL_PRE_RENDER) {
            getchar();
            engine->player->Deactivate();
            delete engine;
            return 0;
        }
        WBCH("FLAG GetMusicControlPanel()")
        auto musPanel = engine->player->GetMusicControlPanel();
        WBCH("FLAG SearchMusic()")
        auto muses = engine->SearchMusic("ヒアソビ", "Camellia");
        WBCH("FLAG LoadMusic()")
        musPanel->LoadMusic(*(engine->DBROOT), muses.front());
        getchar();
        WBCH("FLAG SetMusic()")
        musPanel->SetMusic("ヒアソビ", true);

        // musPanel->
        getchar();
        WBCH("FLAG getFXHandle()")
        auto Fxhandle = musPanel->getFXHandle("ヒアソビ");
        WBCH("FLAG FX_ON_OFF1()")
        Fxhandle->FX_ON_OFF(FXList::OCSFILTER, true);
        WBCH("FLAG FX_ON_OFF2()")
        Fxhandle->FX_ON_OFF(FXList::EQ, true);
        WBCH("FLAG GetArgSetter()")
        auto ocshandle = Fxhandle->GetArgSetter(FXList::OCSFILTER);
        WBCH("FLAG SetArgSetter1()")
        ocshandle["OCSFilterHighLowSW"](1);
        WBCH("FLAG SetArgSetter2()")
        ocshandle["RangeFreqHalf"](2500);
        WBCH("FLAG SetArgSetter3()")
        ocshandle["MiddleFreq"](5000);
        WBCH("FLAG SetArgSetter4()")
        ocshandle["Bps"](2.2333333);
        WBCH("FLAG SetArgSetter5()")
        ocshandle["OCSFilterDryWet"](0.7);
        getchar();
        WBCH("FLAG ChangeBpm()")
        musPanel->ChangeBpm("ヒアソビ", 120, 60);
        WBCH("FLAG GetArgSetter2()")
        auto eqhandle = Fxhandle->GetArgSetter(FXList::EQ);
        WBCH("FLAG SetArgSetter6()")
        eqhandle["EQHigh"](-20);
        WBCH("FLAG SetArgSetter7()")
        eqhandle["EQMid"](-20);
        WBCH("FLAG SetArgSetter8()")
        eqhandle["EQLow"](20);

        getchar();
        WBCH("FLAG Deactivate()")
        auto deactres = engine->player->Deactivate();

        // auto editor = engine->GetEditorObject();
        // editor->UpdateLog<EDIT_ARG_MIX>();
        // editor->UpdateLog<EDIT_ARG_KEY_VALUE>();
        // editor->UpdateLog<EDIT_ARG_NOTE>();
        // editor->UpdateLog<EDIT_ARG_MUSIC>();

        // editor->GetLogWithJSONGraph<EDIT_ARG_MIX>();
        // editor->GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
        // editor->GetLogWithJSONGraph<EDIT_ARG_NOTE>();
        // editor->GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music name");
        // auto core_line = engine->PullOutDataLine();
        // core_line.preRenderedData;
        // core_line.maxCursor;
        // core_line.nowCursor;
        // core_line.used_frame;

    } else {
        std::cout << "init failed " << std::endl;
        delete engine;
        return 1;
    }
    delete engine;
    // std::cout<<engine.InitEditor("test", "test", "./testEditorProject") <<
    // std::endl; engine.editor->ConfigNewMusic("testMiku", "Camellia", "")
    return 0;
}