Judge_Engine
=====================

.. _judge-engine-architecture:

Judge Engine Architecture
-------------------------

The Judge Engine is a specialized module for rhythm games, focusing on precise timing logic and real-time judgment of player input against musical notes. Its architecture is designed for high accuracy and responsiveness:

*   **Microsecond Accuracy:** All internal timing and judgment calculations operate at microsecond resolution, synchronized with the audio timeline via `PDJE_HIGHRES_CLOCK` and `audioSyncData`.
*   **Modular Design:** The engine is composed of several key internal components:
    *   **`Judge_Init`:** The central initialization class (`include/judge/Init/Judge_Init.hpp`) that collects and validates all necessary configurations:
        *   `PDJE_CORE_DATA_LINE` and `PDJE_INPUT_DATA_LINE` from the Core and Input Engines.
        *   `OBJ` (Note Objects) which store and manage all judagble `NOTE` instances.
        *   `EVENT_RULE` which defines the hit/miss timing windows (e.g., `use_range_microsecond`).
        *   `RAIL_DB` which manages the mapping of input events to specific game "rails."
        *   `Custom_Events` for registering optional user-defined callbacks for hit/miss events and custom mouse parsing.
    *   **`PDJE_RAIL` (`RAIL_DB`):** Manages the intricate mapping of diverse input events (keyboard, mouse, MIDI) to abstract game "rails" and their associated offsets. It uses custom hash functions for efficient lookups.
    *   **`PDJE_Rule`:** Defines the `EVENT_RULE` (hit/miss windows) and the `RAIL_KEY` structs for representing input events for mapping purposes.
    *   **`InputParser`:** Processes raw input events (from keyboard/mouse and MIDI) by applying device-specific offsets (from `RAIL_DB`) and sorting them chronologically, normalizing input timings for precise judgment.
    *   **`Judge_Loop`:** The main event loop that orchestrates the entire judgment process:
        *   It continuously fetches processed input from `PreProcess`.
        *   It then passes these inputs to `Match` for comparison against notes.
        *   It spawns **asynchronous worker threads** (`use_event_thread`, `miss_event_thread`) managed by `EV_Thread` and `Queues` (using `Atomic_Double_Buffer`s) to handle user-defined hit/miss callbacks, preventing these callbacks from blocking the critical real-time judgment loop.
    *   **`Match`:** The core logic for comparing processed input events with active note objects.
        *   It uses specialized `UseEvent()` methods for `KEYBOARD`, `MOUSE`, and `MIDI` input types.
        *   Mouse input involves detailed parsing of button states and wheel movements, with a placeholder for `AxisModel` for continuous input.
        *   MIDI `CONTROL_CHANGE` and `PITCH_BEND` events are currently skipped, pending the `AxisModel` implementation.
        *   The `Work()` method performs hit window checks and pushes judgment results to the `use_queue`. A known area for improvement is the note matching algorithm to find the closest note rather than the first.
    *   **`NoteOBJ` (`OBJ`):** Manages `NOTE` objects, storing them in `Buffer_Main` (for presses) and `Buffer_Sub` (for releases/sustained notes) per rail ID. It efficiently `Sort()`s, `Fill()`s, `Get()`s (retrieves active notes), and `Cut()`s (identifies and queues missed notes).
    *   **`AxisModel`:** An empty placeholder class (`include/judge/AxisModel/AxisModel.hpp`) intended for future implementation of continuous input handling (e.g., analog sticks, complex MIDI CC).

This page describes the Judge Engine API and provides snippets that mirror the tested integration flow in ``include/tests/JUDGE_TESTS/judgeTest.cpp`` so that you can follow the same order of operations.

---

Runtime Control
-----------------

The `PDJE_JUDGE::JUDGE` class is the main controller for the Judge Engine. It manages the engine's lifecycle, from initialization to starting and stopping the judgment process, which runs in its own dedicated thread.

.. doxygenclass:: PDJE_JUDGE::JUDGE
    :project: Project_DJ_Engine

.. doxygenenum:: PDJE_JUDGE::JUDGE_STATUS
    :project: Project_DJ_Engine

    The `JUDGE_STATUS` enum provides codes indicating the success or specific error conditions encountered during the Judge Engine's operation.

.. doxygenfunction:: PDJE_JUDGE::JUDGE::Start
    :project: Project_DJ_Engine

    Starts the judgment event loop thread. This method performs validation checks on all required initialization data. If any crucial data (e.g., core data line, input data line, note objects, event rules, or input rules) is missing or invalid, it will return an appropriate `JUDGE_STATUS` error code.
    
    **Usage Order:** Call `Start()` **after** all necessary configurations (rail mappings, event rules, custom callbacks, note collection, and wiring both the core and input data lines) have been completed via `Judge_Init`.

.. doxygenfunction:: PDJE_JUDGE::JUDGE::End
    :project: Project_DJ_Engine

    Stops the internal judgment event loop thread, gracefully releases resources, and clears all cached initialization data.
    
    **Usage Order:** Call `End()` **after** the input and playback systems have been stopped or are no longer in use.

Start will return an error status when the required init data is missing (core
line, input line, note objects, event rule, or input rules). End stops the
internal event loop thread and clears the init data for the next run.

- Typical order (as used in tests): call ``Start`` **after** configuring rail
  mappings, event rules, custom callbacks, note collection, and wiring both the
  core and input data lines. On shutdown, call ``End`` **after** stopping input
  and playback.


Initialization
-----------------

The `PDJE_JUDGE::Judge_Init` class is the central point for configuring the Judge Engine. It aggregates all the necessary data lines, rule sets, and note objects required for the judgment process.

.. doxygenclass:: PDJE_JUDGE::Judge_Init
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCoreLine
    :project: Project_DJ_Engine

    Attaches the `PDJE_CORE_DATA_LINE` obtained from the Core Engine. This provides the Judge Engine with access to the audio playback state, enabling synchronization with the audio timeline.

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetInputLine
    :project: Project_DJ_Engine

    Attaches the `PDJE_INPUT_DATA_LINE` obtained from the Input Engine. This provides the Judge Engine with a stream of processed input events (keyboard, mouse, MIDI) for real-time judgment.

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetRail
    :project: Project_DJ_Engine

    These overloaded methods register specific input events (from keyboards, mice, or MIDI devices) and map them to a designated "rail ID" within the game. An optional `offset_microsecond` can be provided to compensate for device latency.
    
    *   **Keyboard/Mouse:** Use `DeviceData` to specify the device and `BITMASK` for mouse buttons or `PDJE_KEY` for keyboard keys.
    *   **MIDI:** Use `libremidi::input_port` or a port name, along with MIDI message type, channel, and position.

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetEventRule
    :project: Project_DJ_Engine

    Configures the judgment window, defining the `use_range_microsecond` (for successful hits) and `miss_range_microsecond` (for missed events).

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::SetCustomEvents
    :project: Project_DJ_Engine

    Registers optional user-defined callbacks for handling judgment outcomes (missed events, successful hits) and custom mouse axis parsing.

.. doxygenfunction:: PDJE_JUDGE::Judge_Init::NoteObjectCollector
    :project: Project_DJ_Engine

    This method is used to feed note data into the Judge Engine. It takes parsed note metadata, converts its beat-based timing into microseconds, and routes it to the appropriate rail based on the provided `railID`. Notes for unregistered `railID`s will be discarded.

Input mapping
~~~~~~~~~~~~~~

- ``SetRail(device, key, offset, rail)`` is the test-backed helper that binds a
  discovered device/key to a rail with an optional offset (microseconds). Use
  this when you want per-device offsets without manually filling a struct.

- Offsets let you compensate for device latency; negative values advance the
  input timestamp, positive values delay it.


Rules & Callbacks
-------------------

This section details the structures used to define judgment rules and the available callback mechanisms for custom game logic.

.. doxygenstruct:: PDJE_JUDGE::EVENT_RULE
    :project: Project_DJ_Engine

    The `EVENT_RULE` struct defines the core timing windows for judgment:
    *   `miss_range_microsecond`: The time window (in microseconds) outside of which a note is considered missed.
    *   `use_range_microsecond`: The time window (in microseconds) within which an input is considered a successful hit for a note.

.. doxygenstruct:: PDJE_JUDGE::Custom_Events
    :project: Project_DJ_Engine

    The `Custom_Events` struct bundles optional user-defined callbacks that are invoked by the Judge Engine for specific judgment outcomes or input processing.

Custom event callbacks are optional:

*   `missed_event`: A callback receiving a map of `railid` to a `NOTE_VEC` (vector of notes) when notes fall out of their miss window. Use this for visual/audio feedback on misses.
*   `used_event`: A callback receiving (`railid`, `bool Pressed`, `bool IsLate`, `uint64_t diff_microsecond`) for each successfully judged input. `Pressed` indicates if the input was a press or release, `IsLate` indicates if the input was after the note's perfect timing, and `diff_microsecond` is the timing difference from perfect.
*   `custom_mouse_parse`: An optional callback specifically for advanced handling of mouse axis events (movements). It receives `microSecond`, a list of `found_events`, `railID`, `x` and `y` coordinates, and the `axis_type`.
*   `use_event_sleep_time` and `miss_event_sleep_time` control the polling interval for the asynchronous use/miss event processing threads. Adjust these to control callback responsiveness versus CPU usage.

Practical callback example (from the integration test):

.. code-block:: c++

    int miss_count = 0;
    PDJE_JUDGE::MISS_CALLBACK missed =
        [&miss_count](std::unordered_map<uint64_t, PDJE_JUDGE::NOTE_VEC> misses) {
            std::cout << "Missed " << misses.size() << " notes!!!" << std::endl;
            // Iterate through 'misses' to get details of each missed note
            miss_count++;
        };

    PDJE_JUDGE::USE_CALLBACK used = [](uint64_t railid,
                                       bool     Pressed,
                                       bool     IsLate,
                                       uint64_t diff) {
        std::cout << "Used!!! Rail: " << railid << ", "
                  << (Pressed ? "Pressed" : "Released") << ", "
                  << (IsLate ? "Late" : "Early") << ", Diff: "
                  << diff << " microseconds" << std::endl;
    };

    PDJE_JUDGE::MOUSE_CUSTOM_PARSE_CALLBACK mouse_parse =
        [](uint64_t                      microSecond,
           const PDJE_JUDGE::P_NOTE_VEC &found_events,
           uint64_t                      railID,
           int                           x,
           int                           y,
           PDJE_Mouse_Axis_Type          axis_type) {
            // Custom logic for mouse axis movement
            // For instance, update a visual element based on x, y, or axis_type
        };

    judge.inits.SetCustomEvents({
        .missed_event          = missed,
        .used_event            = used,
        .custom_mouse_parse    = mouse_parse,
        .use_event_sleep_time  = std::chrono::milliseconds(1), // Example: Very responsive
        .miss_event_sleep_time = std::chrono::milliseconds(100) // Example: Less frequent for misses
    });

In practice, the callbacks are a convenient place to add audio/visual feedback
or metrics. Reduce the sleep times (as above) for more responsive feedback.


Integration flow (mirrors test)
---------------------------------

This section outlines the typical sequence of operations to integrate and use the Judge Engine, mirroring the successful integration test scenario.

.. tab-set-code::

    .. code-block:: c++

        // 1) Prepare core and player
        //    (Assumes PDJE engine is initialized and track data 'td' is loaded)
        PDJE engine("testRoot.db");
        trackdata td = engine.SearchTrack("").front(); // Example: Get first track
        engine.InitPlayer(PLAY_MODE::FULL_PRE_RENDER, td, 480); // Init player with track

        // 2) Initialize Input Engine and discover devices
        PDJE_Input input;
        input.Init(); // Initialize input system

        auto devs = input.GetDevs(); // Get standard input devices (keyboard/mouse)
        auto midis = input.GetMIDIDevs(); // Get MIDI input devices
        
        // Prepare lists for input configuration
        DEV_LIST keyboards_to_config; 
        std::vector<libremidi::input_port> midis_to_config;

        // Example: Map specific keyboard keys to a rail
        PDJE_JUDGE::JUDGE judge; // Create Judge Engine instance

        for (auto &d : devs) {
            if (d.Type == PDJE_Dev_Type::KEYBOARD) {
                keyboards_to_config.push_back(d);
                // Map 'A' key on this keyboard device to rail ID 1 with 0 microsecond offset
                judge.inits.SetRail(d, PDJE_KEY::A, 0, 1); 
            }
        }

        for (auto &m : midis) {
            midis_to_config.push_back(m); // Add all discovered MIDI devices
            // Map MIDI NOTE_ON event on channel 1, note 48 (C3) to rail ID 5 with 0 offset
            judge.inits.SetRail(m, 5, 
                                static_cast<const uint8_t>(libremidi::message_type::NOTE_ON),
                                1, // Channel
                                48, // Note position (e.g., C3)
                                0); // Offset
        }   

        input.Config(keyboards_to_config, midis_to_config); // Configure Input Engine with selected devices
        // Note: If you don't want to use keyboard/mouse or MIDI, send an empty vector for that parameter.

        // 3) Collect notes from track data
        //    The NoteObjectCollector callback will populate judge.inits.note_objects
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
        engine.GetNoteObjects(td, cb); // Populate the judge with notes from the track

        // 4) Configure event rules and custom callbacks
        judge.inits.SetEventRule({ .miss_range_microsecond = 1'000'005, // Example: 1.005 seconds
                                   .use_range_microsecond  = 1'000'000 }); // Example: 1.000 seconds
        // Set custom callbacks for hit/miss/mouse events (see "Practical callback example" above)
        // judge.inits.SetCustomEvents({...});

        // 5) Wire data lines
        judge.inits.SetInputLine(input.PullOutDataLine()); // Connect Judge to Input Engine's data stream
        judge.inits.SetCoreLine(engine.PullOutDataLine()); // Connect Judge to Core Engine's audio state

        // 6) Start Judge → Input → Playback (recommended order for tight synchronization)
        if (judge.Start() != PDJE_JUDGE::JUDGE_STATUS::OK) {
            std::cerr << "Failed to start judge: " << judge.Start() << std::endl;
            // Handle error based on JUDGE_STATUS
        }
        input.Run(); // Start input processing
        engine.player->Activate(); // Start audio playback

        // Your game loop or main application logic continues here...
        // For demonstration, a simple wait or loop to keep the application alive
        std::this_thread::sleep_for(std::chrono::seconds(10)); 

        // 7) Shutdown (Reverse order of startup for graceful termination)
        engine.player->Deactivate(); // Stop audio playback
        input.Kill(); // Stop input processing
        judge.End(); // Stop Judge Engine

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