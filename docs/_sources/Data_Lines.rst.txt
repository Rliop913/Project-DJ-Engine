Data_Lines
==========

A data line is a lightweight transport lane that exposes addresses of live
engine state to another module. PDJE uses data lines when another subsystem
needs the latest playback or input state without copying whole buffers or
rebuilding a second timing model.

Why Data Lines Exist
--------------------

The main reason to use a data line is zero-copy synchronization:

- the producer keeps ownership of the live state
- the consumer receives a small struct of non-owning pointers
- polling remains cheap enough for UI, gameplay, or wrapper-side bridge code

This is the mechanism used between the core, input, and judge modules.

Typical Use Cases
-----------------

- timeline UI that follows `nowCursor` and `maxCursor`
- waveform preview, meters, or spectrogram-style visualizers that read
  `preRenderedData`
- lightweight synchronization between playback and an external gameplay or tool
  loop
- judge startup, where the judge needs both the core timing line and the input
  event line

General Rules
-------------

- All members are non-owning pointers.
- The producing engine owns the storage behind every pointer.
- Resetting or recreating the producer can invalidate prior addresses.
- Read quickly and copy out what you need if another thread will use the data
  later.
- Treat these structs as read-mostly bridges. Use control-panel APIs or module
  methods for mutations instead of writing through raw pointers.

Lifetime, Threading, And ABI Notes
----------------------------------

- Data lines are only valid while the producing object still owns the pointed
  storage.
- If you reset or recreate a player or input module, call `PullOutDataLine()`
  again instead of caching old pointers.
- Updates can happen on audio threads, backend threads, or subprocess bridges.
- If another thread needs long-lived access, copy the values into your own
  snapshot structure rather than keeping raw pointers.
- The struct layout can evolve with the project. If you expose these structs
  across another ABI boundary, treat them as version-sensitive.

.. warning::

   A data line is a set of non-owning pointers. Do not free the memory behind
   them, and do not hold them across module teardown or reset boundaries.

Core Data Line
--------------

.. doxygenfunction:: PDJE::PullOutDataLine
   :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_CORE_DATA_LINE
   :project: Project_DJ_Engine

.. doxygenstruct:: audioSyncData
   :project: Project_DJ_Engine

Field meanings in the current implementation:

- `nowCursor`
  current playback position in PCM frames
- `maxCursor`
  total rendered frame count
- `preRenderedData`
  interleaved stereo PCM buffer when pre-rendered content exists
- `syncD`
  points at `audioSyncData`, which carries `consumed_frames`,
  `pre_calculated_unused_frames`, and the latest `microsecond` timestamp

This is the minimal common bridge for playback-side synchronization.

.. code-block:: c++

   auto line = engine.PullOutDataLine();
   if (line.nowCursor && line.maxCursor) {
       auto current = *line.nowCursor;
       auto total = *line.maxCursor;
       (void)current;
       (void)total;
   }

   if (line.preRenderedData && line.nowCursor) {
       auto frame_index = *line.nowCursor;
       float left = line.preRenderedData[frame_index * 2];
       float right = line.preRenderedData[frame_index * 2 + 1];
       (void)left;
       (void)right;
   }

   if (line.syncD) {
       auto sync = line.syncD->load(std::memory_order_acquire);
       (void)sync.consumed_frames;
       (void)sync.pre_calculated_unused_frames;
       (void)sync.microsecond;
   }

Input Data Line
---------------

.. doxygenfunction:: PDJE_Input::PullOutDataLine
   :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_INPUT_DATA_LINE
   :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_Input_Log
   :project: Project_DJ_Engine

`PDJE_INPUT_DATA_LINE` exposes two independent channels:

- `input_arena`
  standard keyboard and mouse transport. Call `Receive()` and then inspect the
  `datas` vector of `PDJE_Input_Log`.
- `midi_datas`
  atomic double buffer of `PDJE_MIDI::MIDI_EV`. Call `Get()` to swap the
  active buffer and inspect the returned vector snapshot.

This split matters operationally:

- `input_arena` is the current judge-facing standard-input bridge
- `midi_datas` is the MIDI event stream and can be consumed independently

.. code-block:: c++

   auto line = input.PullOutDataLine();

   if (line.input_arena) {
       line.input_arena->Receive();
       for (const auto &event : line.input_arena->datas) {
           std::string name(event.name, event.name_len);
           (void)name;
           (void)event.microSecond;
       }
   }

   if (line.midi_datas) {
       auto *midi_events = line.midi_datas->Get();
       for (const auto &event : *midi_events) {
           std::string port_name(event.port_name, event.port_name_len);
           (void)port_name;
           (void)event.highres_time;
       }
   }

Binding Notes
-------------

The older docs also described wrapper access patterns. The current state is:

- native C++ can read the struct fields directly
- the current SWIG C# and Python core bindings return opaque
  `SWIGTYPE_p_PDJE_CORE_DATA_LINE` handles from `PullOutDataLine()` rather than
  field-by-field wrapper objects
- the Godot-facing wrapper path exposes helper accessors for the core line and
  uses `InputLine` signals for input delivery instead of raw
  `PDJE_INPUT_DATA_LINE` pointers

Example Godot-side core access:

.. code-block:: gdscript

   var engine:PDJE_Wrapper = PDJE_Wrapper.new()
   engine.InitEngine("res://database/path")

   var core_line = engine.PullOutCoreLine()
   var frames = core_line.GetPreRenderedFrames()
   var max_cursor = core_line.GetMaxCursor()
   var now_cursor = core_line.GetNowCursor()

   print(frames.size(), max_cursor, now_cursor)

For the Godot-side input wrapper path, see :doc:`Input_Engine`.

Threading Notes
---------------

- On the core side, the audio thread updates the cursor and sync data.
- On the input side, backend threads or subprocess bridges update event
  storage.
- Consumers should not hold the raw pointers longer than needed.
- If you rebuild the player or input module, reacquire the line instead of
  caching old pointers.
