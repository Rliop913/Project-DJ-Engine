#include "PDJE_Input.hpp"
#include "PDJE_Judge.hpp"
#include "PDJE_interface.hpp"
#include <iostream>

#include "miniaudio.h"

void
beep_cb(ma_device  *pDevice,
        void       *pOutput,
        const void *pInput,
        ma_uint32   frameCount)
{
}
int
main()
{
    auto engine = PDJE("testRoot.db");
    auto td     = engine.SearchTrack("");

    engine.InitPlayer(PLAY_MODE::FULL_PRE_RENDER, td.front(), 480);
    // engine.player->Activate();
    // getchar();
    // engine.player->Deactivate();
    // return 0;
    ma_engine beep;

    if (ma_engine_init(NULL, &beep) != MA_SUCCESS) {
        std::cerr << "failed to init beep dev" << std::endl;
        return 1;
    }

    ma_waveform        wf;
    ma_waveform_config wfConfig =
        ma_waveform_config_init(ma_format_f32,
                                1,
                                ma_engine_get_sample_rate(&beep),
                                ma_waveform_type_sine,
                                1.0f,
                                440.0f);

    ma_waveform_init(&wfConfig, &wf);

    ma_sound bsound;
    ma_sound_init_from_data_source(&beep, &wf, 0, NULL, &bsound);
    ma_sound_set_looping(&bsound, MA_FALSE);

    // ma_device_config conf   = ma_device_config_init(ma_device_type_playback);
    // conf.playback.format    = ma_format_f32;
    // conf.playback.channels  = 2;
    // conf.sampleRate         = 48000;
    // conf.periodSizeInFrames = 48;
    // conf.performanceProfile = ma_performance_profile_low_latency;
    // conf.pUserData = nullptr;
    // conf.dataCallback = beep_cb;

    // ma_device beep_dev;
    // if(ma_device_init(NULL, &conf, &beep_dev) != MA_SUCCESS){
    //     std::cerr << "failed to init beep dev" <<std::endl;
    //     return 1;
    // }
    auto input = PDJE_Input();
    input.Init();
    auto     devs  = input.GetDevs();
    auto     judge = PDJE_JUDGE::JUDGE();
    DEV_LIST list;

    for (auto &d : devs) {
        if (d.Type == PDJE_Dev_Type::KEYBOARD) {

            // std::cout << "DEVICE id: " << d.device_specific_id << std::endl;
            // std::cout << "DEVICE NAME: " << d.Name << std::endl;

            list.push_back(d);
            judge.inits.SetRail(d, PDJE_KEY::A, 0, 1);
        }
        // if (d.Type == PDJE_Dev_Type::MOUSE) {

        //     std::cout << "DEVICE id: " << d.device_specific_id << std::endl;
        //     std::cout << "DEVICE NAME: " << d.Name << std::endl;

        //     list.push_back(d);

        //     // PDJE_JUDGE::INPUT_CONFIG conf;

        //     // conf.Device_ID  = d.Name;

        //     // conf.DeviceKey  = PDJE_JUDGE::DEVICE_MOUSE_EVENT::BTN_L;
        //     // conf.MatchRail  = 1;
        //     // conf.offset_microsecond
        //     judge.inits.SetRail(d, PDJE_JUDGE::DEVICE_MOUSE_EVENT::BTN_L, 0,
        //     1);
        // }
    }
    if (!input.Config(list)) {
        std::cout << "config failed" << std::endl;
    }
    auto                iline          = input.PullOutDataLine();
    int                 note_add_count = 0;
    OBJ_SETTER_CALLBACK cb             = [&](const std::string        noteType,
                                 const uint16_t           noteDetail,
                                 const std::string        firstArg,
                                 const std::string        secondArg,
                                 const std::string        thirdArg,
                                 const unsigned long long Y_Axis,
                                 const unsigned long long Y_Axis_2,
                                 const uint64_t           railID) {
        judge.inits.NoteObjectCollector(noteType,
                                        noteDetail,
                                        firstArg,
                                        secondArg,
                                        thirdArg,
                                        Y_Axis,
                                        Y_Axis_2,
                                        railID);
        note_add_count++;
    };
    engine.GetNoteObjects(td.front(), cb);
    std::cout << "notes: " << note_add_count << std::endl;
    judge.inits.SetEventRule({ .miss_range_microsecond = 60005,
                               .use_range_microsecond  = 60000 });
    judge.inits.SetInputLine(input.PullOutDataLine());

    int                       miss_count = 0;
    PDJE_JUDGE::MISS_CALLBACK missed =
        [&miss_count, &bsound, &beep](
            std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> misses) {
            std::cout << "missed!!!" << miss_count++ << std::endl;
            // ma_uint32 sampleRate = ma_engine_get_sample_rate(&beep);
            // ma_uint64 nowFrames   = ma_engine_get_time_in_pcm_frames(&beep);
            // ma_uint64 durFrames   = (ma_uint64)(0.1f * sampleRate);
            // ma_sound_set_stop_time_in_pcm_frames(&bsound, nowFrames +
            // durFrames); ma_sound_start(&bsound);
        };
    PDJE_JUDGE::USE_CALLBACK used = [&bsound, &beep](uint64_t railid,
                                                     bool     Pressed,
                                                     bool     IsLate,
                                                     uint64_t diff) {
        ma_uint32 sampleRate = ma_engine_get_sample_rate(&beep);
        ma_uint64 nowFrames  = ma_engine_get_time_in_pcm_frames(&beep);
        ma_uint64 durFrames  = (ma_uint64)(0.1f * sampleRate);
        ma_sound_set_stop_time_in_pcm_frames(&bsound, nowFrames + durFrames);
        ma_sound_start(&bsound);
        std::cout << "used!!!" << diff / 1000 << (IsLate ? " late " : " early ")
                  << std::endl;
    };
    PDJE_JUDGE::MOUSE_CUSTOM_PARSE_CALLBACK mouse_parse =
        [](uint64_t                      microSecond,
           const PDJE_JUDGE::P_NOTE_VEC &found_events,
           uint64_t                      railID,
           int                           x,
           int                           y,
           PDJE_Mouse_Axis_Type          axis_type) { return; };
    judge.inits.SetCustomEvents(
        { .missed_event          = missed,
          .used_event            = used,
          .custom_mouse_parse    = mouse_parse,
          .use_event_sleep_time  = std::chrono::milliseconds(1),
          .miss_event_sleep_time = std::chrono::milliseconds(1) });
    judge.inits.SetCoreLine(engine.PullOutDataLine());
    if (judge.Start() != PDJE_JUDGE::JUDGE_STATUS::OK) {
        std::cerr << "Failed to start judge" << std::endl;
    }
    input.Run();
    engine.player->Activate();

    // end
    getchar();
    engine.player->Deactivate();
    input.Kill();
    judge.End();

    return 0;
}