Judge_Engine
============

The Judge Engine converts synchronized audio and input timing into gameplay
events. In the current tree the public control surface is split between a
runtime owner, `PDJE_JUDGE::JUDGE`, and an initialization bundle,
`PDJE_JUDGE::Judge_Init`.

Judge Engine Architecture
-------------------------

The older docs spent more time on the judge internals, and that architecture is
still useful for understanding how the public API is meant to be wired:

- `Judge_Init`
  the setup bundle that collects core and input data lines, note objects, rail
  mappings, event rules, and optional custom callbacks
- `RAIL_DB`
  the internal mapping database that links keyboard, mouse, or MIDI events to
  gameplay rails plus optional microsecond offsets
- `PDJE_Rule`
  the area that defines `EVENT_RULE` and the input-key structures used for rail
  matching
- `InputParser`
  the preprocessing stage that reads input events and normalizes them into a
  form the judge loop can consume
- `Judge_Loop`
  the runtime loop that owns the timing process, input preprocessing, matching,
  and callback worker threads
- `Match`
  the note-comparison logic that turns processed inputs into use or miss events
- `OBJ`
  the note-object storage used by `NoteObjectCollector()` and the matcher
- `AxisModel`
  still a placeholder in the current tree rather than a finished public feature

Runtime Types
-------------

.. doxygenclass:: PDJE_JUDGE::JUDGE
   :project: Project_DJ_Engine

.. doxygenenum:: PDJE_JUDGE::JUDGE_STATUS
   :project: Project_DJ_Engine

.. doxygenclass:: PDJE_JUDGE::Judge_Init
   :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_JUDGE::EVENT_RULE
   :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_JUDGE::Custom_Events
   :project: Project_DJ_Engine

Startup Contract
----------------

`JUDGE::Start()` validates the initialization bundle before spawning the event
loop thread.

.. doxygenfunction:: PDJE_JUDGE::JUDGE::Start
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::JUDGE::End
   :project: Project_DJ_Engine

Start will fail when any of the following are missing:

- a valid core data line
- a valid input data line
- at least one note object
- a populated `EVENT_RULE`
- at least one rail mapping

Important current behavior:

- `Judge_Init::SetInputLine()` only stores the line when `input_arena` is not
  null
- MIDI rail mapping is supported, but the tested startup path still includes a
  configured standard input backend so the input line is considered valid

Initialization API
------------------

Selected setup methods:

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCoreLine
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetInputLine
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetEventRule
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCustomEvents
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::NoteObjectCollector
   :project: Project_DJ_Engine

`Judge_Init::SetRail(...)` has three public overload groups:

- standard devices: `SetRail(const DeviceData&, const BITMASK, ...)`
- MIDI by port name: `SetRail(const std::string&, ...)`
- MIDI by `libremidi::input_port`

All three overload groups map an input event to a gameplay rail plus an
optional microsecond offset.

Timing Model
------------

`NoteObjectCollector()` converts note timing from PCM-frame positions into
microseconds through the 48 kHz conversion helpers defined in
`PDJE_Judge_Init_Structs.hpp`. This lets the judge compare note timing with:

- input timestamps emitted by the input engine
- synchronized audio timing from `audioSyncData`

This is the key reason the judge is described as microsecond-oriented: timing
precision is not limited to the audio callback cadence in the same way the
older buffer-step model was.

`AxisModel` still exists only as a placeholder in the current source tree. Axis
style note handling is not yet a complete public feature.

Binding Status
--------------

The older manual pages also documented a wrapper-facing judge flow:

- the current in-tree SWIG C# and Python outputs do not expose
  `PDJE_JUDGE::JUDGE`
- the non-C++ integration path that still exists in the docs is the Godot
  wrapper layer
- that wrapper uses `PDJE_Judge_Module` and is designed to consume the Godot
  input wrapper plus the core/player wrapper

Reference Integration Order
---------------------------

The order below mirrors the integration flow in
`include/tests/JUDGE_TESTS/judgeTest.cpp`.

.. code-block:: c++

   PDJE engine("testRoot.db");
   auto tracks = engine.SearchTrack("");
   if (tracks.empty()) {
       return;
   }

   (void)engine.InitPlayer(PLAY_MODE::FULL_PRE_RENDER, tracks.front(), 480);

   PDJE_Input input;
   if (!input.Init()) {
       return;
   }

   auto devs = input.GetDevs();
   auto midi_ports = input.GetMIDIDevs();

   PDJE_JUDGE::JUDGE judge;
   DEV_LIST selected_standard;

   for (const auto &dev : devs) {
       if (dev.Type == PDJE_Dev_Type::KEYBOARD) {
           selected_standard.push_back(dev);
           judge.inits.SetRail(dev, PDJE_KEY::A, 0, 1);
       }
   }

   for (const auto &midi : midi_ports) {
       judge.inits.SetRail(
           midi,
           1,
           static_cast<const uint8_t>(libremidi::message_type::NOTE_ON),
           1,
           48,
           0);
   }

   if (!input.Config(selected_standard, midi_ports)) {
       return;
   }

   OBJ_SETTER_CALLBACK collect = [&](const std::string &noteType,
                                     const uint16_t noteDetail,
                                     const std::string &firstArg,
                                     const std::string &secondArg,
                                     const std::string &thirdArg,
                                     const unsigned long long y1,
                                     const unsigned long long y2,
                                     const uint64_t railId) {
       judge.inits.NoteObjectCollector(
           noteType, noteDetail, firstArg, secondArg, thirdArg, y1, y2, railId);
   };

   (void)engine.GetNoteObjects(tracks.front(), collect);

   judge.inits.SetEventRule({
       .miss_range_microsecond = 600005,
       .use_range_microsecond = 600000,
   });

   judge.inits.SetInputLine(input.PullOutDataLine());
   judge.inits.SetCoreLine(engine.PullOutDataLine());
   judge.inits.SetCustomEvents({});

   if (judge.Start() != PDJE_JUDGE::JUDGE_STATUS::OK) {
       return;
   }

   (void)input.Run();
   (void)engine.player->Activate();

   // ...

   (void)engine.player->Deactivate();
   input.Kill();
   judge.End();

Callbacks
---------

`Custom_Events` lets you attach:

- `missed_event`
- `used_event`
- `custom_mouse_parse`

The judge loop keeps these callbacks off the main matching path by dispatching
them through worker queues. Use lightweight handlers or your own downstream
queueing if callback work can grow.

Practical callback example:

.. code-block:: c++

   PDJE_JUDGE::MISS_CALLBACK missed =
       [](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> misses) {
           std::cout << "Miss groups: " << misses.size() << std::endl;
       };

   PDJE_JUDGE::USE_CALLBACK used =
       [](uint64_t railid, bool pressed, bool is_late, uint64_t diff) {
           std::cout << railid << " " << pressed << " " << is_late
                     << " " << diff << std::endl;
       };

   PDJE_JUDGE::MOUSE_CUSTOM_PARSE_CALLBACK mouse_parse =
       [](uint64_t microsecond,
          const PDJE_JUDGE::P_NOTE_VEC &found_events,
          uint64_t railid,
          int x,
          int y,
          PDJE_Mouse_Axis_Type axis_type) {
           (void)microsecond;
           (void)found_events;
           (void)railid;
           (void)x;
           (void)y;
           (void)axis_type;
       };

   judge.inits.SetCustomEvents({
       .missed_event = missed,
       .used_event = used,
       .custom_mouse_parse = mouse_parse,
       .use_event_sleep_time = std::chrono::milliseconds(1),
       .miss_event_sleep_time = std::chrono::milliseconds(1),
   });

Godot Wrapper Flow
------------------

.. code-block:: gdscript

   $PDJE_Input_Module.Init()
   var device_list:Array = $PDJE_Input_Module.GetDevs()
   var selected_devices:Array = []
   for device in device_list:
       if device["type"] == "MOUSE":
           selected_devices.push_back(device)

   var selected_midi_devices = $PDJE_Input_Module.GetMIDIDevs()
   $PDJE_Input_Module.Config(selected_devices, selected_midi_devices)

   $PDJE_Judge_Module.AddDataLines($PDJE_Input_Module, engine)
   for device in selected_devices:
       $PDJE_Judge_Module.DeviceAdd(device, 4, 0, InputLine.BTN_L, 0, 5)

   for midi_port in selected_midi_devices:
       $PDJE_Judge_Module.MIDI_DeviceAdd(
           midi_port, 5, "NOTE_ON", 1, 48, 0)

   $PDJE_Judge_Module.SetRule(60 * 1000, 61 * 1000, 1, 3, false)
   $PDJE_Judge_Module.SetNotes(engine, "sample_track")

   $PDJE_Judge_Module.StartJudge()
   $PDJE_Input_Module.Run()
   player.Activate()

   player.Deactivate()
   $PDJE_Input_Module.Kill()
   $PDJE_Judge_Module.EndJudge()
