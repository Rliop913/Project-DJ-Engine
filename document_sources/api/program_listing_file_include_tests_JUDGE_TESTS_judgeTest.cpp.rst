
.. _program_listing_file_include_tests_JUDGE_TESTS_judgeTest.cpp:

Program Listing for File judgeTest.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_JUDGE_TESTS_judgeTest.cpp>` (``include/tests/JUDGE_TESTS/judgeTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Input.hpp"
   #include "PDJE_Judge.hpp"
   #include "PDJE_interface.hpp"
   #include <iostream>
   int
   main()
   {
       auto engine = PDJE("testRoot.db");
       auto td     = engine.SearchTrack("");
   
       engine.InitPlayer(PLAY_MODE::FULL_PRE_RENDER, td.front(), 48);
       // engine.player->Activate();
       // getchar();
       // engine.player->Deactivate();
       // return 0;
       auto input = PDJE_Input();
       input.Init();
       auto     devs  = input.GetDevs();
       auto     judge = PDJE_JUDGE::JUDGE();
       DEV_LIST list;
       for (auto &d : devs) {
           if (d.Type == PDJE_Dev_Type::KEYBOARD) {
               std::cout << "DEVICE ID: " << d.device_specific_id << std::endl;
               std::cout << "DEVICE NAME: " << d.Name << std::endl;
               list.push_back(d);
   
               PDJE_JUDGE::INPUT_CONFIG conf;
   
               conf.Device_ID  = d.device_specific_id;
               conf.DeviceType = d.Type;
               conf.DeviceKey  = PDJE_KEY::A;
               conf.MatchRail  = 1;
   
               judge.inits.SetInputRule(conf);
           }
       }
       input.Config(list);
       OBJ_SETTER_CALLBACK cb = [&](const std::string        noteType,
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
       };
       engine.GetNoteObjects(td.front(), cb);
       judge.inits.SetEventRule(
           { .miss_range_microsecond = 100000, .use_range_microsecond = 40000 });
       judge.inits.SetInputLine(input.PullOutDataLine());
       PDJE_JUDGE::MISS_CALLBACK missed =
           [](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> misses) {
               std::cout << "missed!!!" << std::endl;
           };
       PDJE_JUDGE::USE_CALLBACK used =
           [](uint64_t railid, bool Pressed, bool IsLate, uint64_t diff) {
               std::cout << "used!!!" << std::endl;
           };
       PDJE_JUDGE::MOUSE_AXIS_PARSE_CALLBACK mouse_parse =
           [](uint64_t                      microSecond,
              const PDJE_JUDGE::P_NOTE_VEC &found_events,
              uint64_t                      railID,
              int                           x,
              int                           y,
              PDJE_Mouse_Axis_Type          axis_type) { return; };
       judge.inits.SetCustomEvents({ .missed_event      = missed,
                                     .used_event        = used,
                                     .custom_axis_parse = mouse_parse });
       judge.inits.SetCoreLine(engine.PullOutDataLine());
       judge.Start();
       input.Run();
       engine.player->Activate();
   
       // end
       getchar();
       engine.player->Deactivate();
       input.Kill();
       judge.End();
   
       return 0;
   }
