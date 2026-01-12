Judge_Engine
=====================

The judge module consumes note objects from the core engine and timestamped
inputs from the input engine, aligns them on a shared microsecond clock, and
applies hit/miss rules while emitting callbacks. The snippets below mirror the
tested integration flow in ``include/tests/JUDGE_TESTS/judgeTest.cpp`` so that
you can follow the same order of operations.

Runtime Control
-----------------

.. doxygenclass:: PDJE_JUDGE::JUDGE
    :project: Project_DJ_Engine

.. doxygenenum:: PDJE_JUDGE::JUDGE_STATUS
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::JUDGE::Start
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::JUDGE::End
    :project: Project_DJ_Engine



Start will return an error status when the required init data is missing (core
line, input line, note objects, event rule, or input rules). End stops the
internal event loop thread and clears the init data for the next run.

- Typical order (as used in tests): call ``Start`` **after** configuring rail
  mappings, event rules, custom callbacks, note collection, and wiring both the
  core and input data lines. On shutdown, call ``End`` **after** stopping input
  and playback.


Initialization
-----------------

.. doxygenclass:: PDJE_JUDGE::Judge_Init
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCoreLine
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetInputLine
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetRail
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetEventRule
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCustomEvents
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::NoteObjectCollector
    :project: Project_DJ_Engine

- `SetCoreLine` and `SetInputLine` connect to :doc:`/Data_Lines` produced by the
  core and input engines. Null pointers are ignored.
- `SetRail` registers an `INPUT_CONFIG` (device id, type, key) and maps it
  to a rail plus an optional input offset in microseconds.
- `SetEventRule` defines the hit window in microseconds:
  `use_range_microsecond` for successful hits and `miss_range_microsecond` for
  late/early checks.
- `SetCustomEvents` stores callbacks (see below) and optional sleep times
  between callback pulls.
- `NoteObjectCollector` fills the internal note buffer. Frame positions
  (Y\_Axis, Y\_Axis\_2) are converted to microseconds using
  ``Convert_Frame_Into_MicroSecond`` and routed to the rail that matches the
  registered input rule. If no device rule exists for the rail, the call is
  ignored.

Input mapping
~~~~~~~~~~~~~~

- ``SetRail(device, key, offset, rail)`` is the test-backed helper that binds a
  discovered device/key to a rail with an optional offset (microseconds). Use
  this when you want per-device offsets without manually filling a struct.

- Offsets let you compensate for device latency; negative values advance the
  input timestamp, positive values delay it.


Rules & Callbacks
-------------------

.. doxygenstruct:: PDJE_JUDGE::INPUT_CONFIG
    :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_JUDGE::EVENT_RULE
    :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_JUDGE::Custom_Events
    :project: Project_DJ_Engine

Custom event callbacks are optional:

- `missed_event` receives a map of rail id -> note vector when notes fall out of
  the miss window.
- `used_event` receives (rail id, pressed?, late?, diff\_microsecond) for each
  judged input.
- `custom_mouse_parse` is called for mouse axis events with
  (microsecond, found events, rail id, x, y, axis type).
- `use_event_sleep_time` and `miss_event_sleep_time` control the pull interval
  for the two queues (default 100 ms / 200 ms).

Practical callback example (from the integration test):

.. code-block:: c++

    int miss_count = 0;
    PDJE_JUDGE::MISS_CALLBACK missed =
        [&miss_count](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> misses) {
            std::cout << "missed!!!" << miss_count++ << std::endl;
        };

    PDJE_JUDGE::USE_CALLBACK used = [](uint64_t railid,
                                       bool     Pressed,
                                       bool     IsLate,
                                       uint64_t diff) {
        std::cout << "used!!! " << diff / 1000
                  << (IsLate ? " late " : " early ") << std::endl;
    };

    PDJE_JUDGE::MOUSE_CUSTOM_PARSE_CALLBACK mouse_parse =
        [](uint64_t                      microSecond,
           const PDJE_JUDGE::P_NOTE_VEC &found_events,
           uint64_t                      railID,
           int                           x,
           int                           y,
           PDJE_Mouse_Axis_Type          axis_type) { return; };

    judge.inits.SetCustomEvents({
        .missed_event          = missed,
        .used_event            = used,
        .custom_mouse_parse    = mouse_parse,
        .use_event_sleep_time  = std::chrono::milliseconds(1),
        .miss_event_sleep_time = std::chrono::milliseconds(1)
    });

In practice, the callbacks are a convenient place to add audio/visual feedback
or metrics. Reduce the sleep times (as above) for more responsive feedback.


Integration flow (mirrors test)
---------------------------------

.. tab-set-code::

    .. code-block:: c++

        // 1) Prepare core and player
        PDJE engine("testRoot.db");
        auto td = engine.SearchTrack("");
        engine.InitPlayer(PLAY_MODE::FULL_PRE_RENDER, td.front(), 480);

        // 2) Discover input devices and map to rails
        auto input = PDJE_Input();
        input.Init();
        auto devs = input.GetDevs();
        auto midis = input.GetMIDIDevs();
        DEV_LIST keyboards;
        PDJE_JUDGE::JUDGE judge;

        for (auto &d : devs) {
            if (d.Type == PDJE_Dev_Type::KEYBOARD) {
                keyboards.push_back(d);
                judge.inits.SetRail(d, PDJE_KEY::A, 0, 1); // device, key, offset, rail
            }
        }

        for (auto &m : midis) {
        
        judge.inits.SetRail(
            m,
            1,//match railid
            static_cast<const uint8_t>(libremidi::message_type::NOTE_ON),//type. NOTE_ON, CONTROL_CHANGE(not implemented on judge logic), PITCH_BEND(not implemented on judge logic) is available.
            1,//channel
            48,//note position
            0);//offset(microsecond)
        }   

        input.Config(keyboards, midis);
        //if you don't want to use midi or kb&mouse, just send blank vector.

        // 3) Collect notes from track data
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

        // 4) Configure event rules and callbacks
        judge.inits.SetEventRule({ .miss_range_microsecond = 1'000'005,
                                   .use_range_microsecond  = 1'000'000 });
        // (See callback example above.)

        // 5) Wire data lines
        judge.inits.SetInputLine(input.PullOutDataLine());
        judge.inits.SetCoreLine(engine.PullOutDataLine());

        // 6) Start judge → input → playback (tested order)
        if (judge.Start() != PDJE_JUDGE::JUDGE_STATUS::OK) {
            std::cerr << "Failed to start judge" << std::endl;
        }
        input.Run();
        engine.player->Activate();

        // 7) Shutdown
        engine.player->Deactivate();
        input.Kill();
        judge.End();
    
    .. code-block:: gdscript

      #input module init phase
      $PDJE_Input_Module.Init()
      var device_list:Array = $PDJE_Input_Module.GetDevs()
      
      var selected_devices:Array
      for device in device_list:
        if device["type"] == "MOUSE":
          selected_devices.push_back(device)
      print(selected_devices)
      var selected_midi_devices = $PDJE_Input_Module.GetMIDIDevs()
      
      $PDJE_Input_Module.Config(selected_devices, selected_midi_devices)

      #judge module init phase
      $PDJE_Judge_Module.AddDataLines($PDJE_Input_Module, engine)
      for dev in selected_devices:
        $PDJE_Judge_Module.DeviceAdd(dev, 4, 0, InputLine.BTN_L, 0, 5)#link mouse left button into rail id 5
        #if dev's type is keyboard
        #$PDJE_Judge_Module.DeviceAdd(dev, InputLine.PDJE_KEY.A, 0, 5)
        #$PDJE_Judge_Module.DeviceAdd(dev, InputLine.PDJE_KEY.S, 0, 5)
        #$PDJE_Judge_Module.DeviceAdd(dev, InputLine.PDJE_KEY.D, 0, 5)
        #$PDJE_Judge_Module.DeviceAdd(dev, InputLine.PDJE_KEY.F, 0, 5)
        #link all keyboard's "ASDF" into rail id 5
      for midis in selected_midi_devices:
        $PDJE_Judge_Module.MIDI_DeviceAdd(midis, 5, "NOTE_ON", 1, 48, 0)# link midi type "NOTE_ON" channel 1, position 48 into rail 5 with 0 microsecond offset
      var use_range= 60 * 1000 # use range +- 60ms
      var miss_range= 61 * 1000 # miss range +-61ms
      var use_sleep=1 #use evnet thread sleeps 1ms on every loop
      var miss_sleep=3 #miss event thread sleeps 3ms on every loop
      var custom_mouse_event=false #deactivate mouse custom event. you can use axis data with this.
      
      $PDJE_Judge_Module.SetRule(
        use_range,
        miss_range,
        use_sleep,
        miss_sleep,
        custom_mouse_event
      )# set judge rules
      
      print($PDJE_Judge_Module.SetNotes(engine, "sample_track"))
      
      #Start Game
      $PDJE_Judge_Module.StartJudge()
      $PDJE_Input_Module.Run()#input module
	    player.Activate()#core module music player
      #Start Judge -> Start Input -> Start Core music player
      
      #End Phase
      player.Deactivate()#core module music player
      $PDJE_Input_Module.Kill()#input module
      $PDJE_Judge_Module.EndJudge()

      #End Core music player -> End Input -> End Judge