Judge_Engine
=====================

.. warning::
    Current project version is now on 0.6.x.

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

.. doxygenfunction:: PDJE_JUDGE::JUDGE::CheckStatus
    :project: Project_DJ_Engine

Start will return an error status when the required init data is missing (core
line, input line, note objects, event rule, or input rules). End stops the
internal event loop thread and clears the init data for the next run.


Initialization
-----------------

.. doxygenclass:: PDJE_JUDGE::Judge_Init
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCoreLine
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetInputLine
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetInputRule
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetEventRule
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCustomEvents
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::NoteObjectCollector
    :project: Project_DJ_Engine

- `SetCoreLine` and `SetInputLine` connect to :doc:`/Data_Lines` produced by the
  core and input engines. Null pointers are ignored.
- `SetInputRule` registers an `INPUT_CONFIG` (device id, type, key) and maps it
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


Rules & Callbacks
-------------------

.. doxygenstruct:: PDJE_JUDGE::INPUT_RULE
    :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_JUDGE::INPUT_SETTING
    :project: Project_DJ_Engine

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


Minimal Flow
--------------

.. tab-set-code::

    .. code-block:: c++

        PDJE_JUDGE::JUDGE judge;
        PDJE_Input input;
        input.Init();
        // data lines
        judge.inits.SetCoreLine(core_line);
        judge.inits.SetInputLine(input.PullOutDataLine());

        // device mapping
        auto devs = input.GetDevs();
        DEV_LIST keyboards;
        for(auto i : devs){
            switch(i.Type){
            case PDJE_Dev_Type::KEYBOARD:
                keyboards.push_back(i);
                break;
            default:
                break;
            }
        }
        for(auto dev : devs){
            PDJE_JUDGE::INPUT_CONFIG kb{};
            kb.Device_ID = dev.device_specific_id;
            kb.DeviceType = PDJE_Dev_Type::KEYBOARD;
            kb.DeviceKey = PDJE_KEY::A;
            kb.MatchRail = 1;
            kb.offset_microsecond = -1000;//-1ms
            judge.inits.SetInputRule(kb);
            PDJE_JUDGE::INPUT_CONFIG kb_second{};
            kb_second.Device_ID = dev.device_specific_id;
            kb_second.DeviceType = PDJE_Dev_Type::KEYBOARD;
            kb_second.DeviceKey = PDJE_KEY::S;
            kb_second.MatchRail = 2;
            kb_second.offset_microsecond = 2000;//+2ms
            judge.inits.SetInputRule(kb_second);
        }

        // hit window
        judge.inits.SetEventRule(
            PDJE_JUDGE::EVENT_RULE{50'000, 30'000}); // miss/use in microseconds

        // callbacks (optional)
        PDJE_JUDGE::Custom_Events events{};
        events.used_event = [](uint64_t rail, bool pressed, bool late, uint64_t diff){};
        events.missed_event = [](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> missed){};
        judge.inits.SetCustomEvents(events);

        // notes
        judge.inits.NoteObjectCollector("SIMPLE", 0, "", "", "", y_frame, y2_frame, 1);
        // OR
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
        };//make callback
        engine.GetNoteObjects(td.front(), cb);//td is Trackdata.
        //this function extracts note datas from track data.

        // run
        auto status = judge.Start(); // returns JUDGE_STATUS
        input.Run();//run input module
        engine.player->Activate();//run music player
        // ...
        engine.player->Deactivate();
        input.Kill();
        judge.End();
