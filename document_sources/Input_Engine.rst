Input_Engine
============

`PDJE_Input` is the current public entry point for low-latency keyboard, mouse,
and MIDI ingestion.

Public Types
------------

.. doxygenclass:: PDJE_Input
   :project: Project_DJ_Engine

.. doxygenstruct:: DeviceData
   :project: Project_DJ_Engine

.. doxygenenum:: PDJE_Dev_Type
   :project: Project_DJ_Engine

.. doxygenenum:: PDJE_INPUT_STATE
   :project: Project_DJ_Engine

Platform Behavior
-----------------

- **Windows** uses the Raw Input subprocess and IPC path and currently reports
  the backend string `rawinput-ipc`.
- **Linux** can use the evdev path directly or a Wayland-backed path when
  device selection requires it.
- **macOS** is not implemented in the current source tree.

Transport Notes
---------------

The older docs spent more time on transport details, and that context is still
useful:

- On Windows, standard input is intentionally isolated behind a child-process
  Raw Input path so the engine can collect low-latency keyboard and mouse data
  and ship it back through IPC.
- On Linux, the default low-level path is evdev. When evdev is not the path the
  current environment can use, the engine can also operate through the
  Wayland-backed route described by `Init(...)`.
- MIDI is handled as a separate stream and surfaces through `midi_datas` even
  though the standard input backend and MIDI engine are configured together.

Binding Status
--------------

The older docs also covered non-C++ integration paths. The current split is:

- the in-tree SWIG C# and Python bindings do not expose `PDJE_Input`
- the Godot-facing wrapper path is still the documented non-C++ route for this
  module
- that Godot wrapper uses `PDJE_Input_Module` together with an `InputLine` node
  that emits keyboard and MIDI signals instead of handing out raw transport
  pointers

Initialization Signature
------------------------

.. doxygenfunction:: PDJE_Input::Init
   :project: Project_DJ_Engine

`Init()` accepts optional platform handles:

- `platform_ctx0`
  on Linux is expected to be `wl_display*` when the host already owns the
  Wayland connection
- `platform_ctx1`
  on Linux is expected to be `wl_surface*` when the host already owns the
  Wayland surface
- `use_internal_window`
  allows PDJE to create its own internal Wayland window when a Wayland fallback
  is needed and host handles are not available

Windows keeps the same signature for parity but ignores these parameters in the
current implementation.

Current Lifecycle
-----------------

The tested integration path in the current tree is:

1. Call `Init(...)`.
2. Discover devices with `GetDevs()` and `GetMIDIDevs()`.
3. Select at least one keyboard or mouse device for the standard backend, then
   add any desired MIDI ports.
4. Call `Config(...)`.
5. Inspect `GetCurrentInputBackend()` if you need to know which backend was
   selected.
6. Acquire a `PDJE_INPUT_DATA_LINE` with `PullOutDataLine()`.
7. Call `Run()`.
8. Consume `input_arena` and `midi_datas`.
9. Call `Kill()` on shutdown.

Selected methods:

.. doxygenfunction:: PDJE_Input::GetDevs
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetMIDIDevs
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::Config
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetState
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetCurrentInputBackend
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::Run
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::Kill
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::PullOutDataLine
   :project: Project_DJ_Engine

Data Line Semantics
-------------------

.. doxygenstruct:: PDJE_INPUT_DATA_LINE
   :project: Project_DJ_Engine

`PDJE_INPUT_DATA_LINE` exposes two optional pointers:

- `input_arena`
  points at the standard keyboard and mouse event transport. Call
  `input_arena->Receive()` before reading `input_arena->datas`.
- `midi_datas`
  points at `Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV>`. Call `Get()` to swap
  buffers and read the current vector snapshot.

Operationally:

- `input_arena` is the keyboard and mouse lane the judge currently expects when
  `Judge_Init::SetInputLine()` validates the input path
- `midi_datas` is a separate MIDI lane and can be consumed independently of the
  standard input events

Example
-------

.. code-block:: c++

   PDJE_Input input;
   if (!input.Init(nullptr, nullptr, false)) {
       return;
   }

   auto devs = input.GetDevs();
   auto midi_ports = input.GetMIDIDevs();

   DEV_LIST selected_standard;
   std::vector<libremidi::input_port> selected_midi;

   for (const auto &dev : devs) {
       if (dev.Type == PDJE_Dev_Type::KEYBOARD) {
           selected_standard.push_back(dev);
       }
   }

   for (const auto &port : midi_ports) {
       selected_midi.push_back(port);
   }

   if (!input.Config(selected_standard, selected_midi)) {
       return;
   }

   auto backend = input.GetCurrentInputBackend();
   auto line = input.PullOutDataLine();

   if (!input.Run()) {
       return;
   }

   while (input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_RUNNING) {
       if (line.input_arena) {
           line.input_arena->Receive();
           for (const auto &event : line.input_arena->datas) {
               std::string device_name(event.name, event.name_len);
               (void)device_name;
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

       std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }

   (void)backend;
   input.Kill();

Notes
-----

- `GetCurrentInputBackend()` returns `"none"` until `default_devs` has been
  initialized.
- Reacquire the data line after tearing down and rebuilding the input module.
- The judge module currently expects `SetInputLine()` to receive a non-null
  `input_arena`, so the tested path includes a configured standard backend even
  when MIDI rails are also used.
- Keep device selection explicit. `GetDevs()` is for standard devices,
  `GetMIDIDevs()` is for MIDI ports, and `Config(...)` is where the two streams
  are joined into one input runtime.

Godot Wrapper Example
---------------------

.. code-block:: gdscript

   extends Node

   var input_module:PDJE_Input_Module

   func _ready():
       input_module = PDJE_Input_Module.new()
       input_module.Init()

       var selected_devices:Array = []
       for device in input_module.GetDevs():
           if device["type"] == "KEYBOARD":
               selected_devices.push_back(device)

       var selected_midi_devices = input_module.GetMIDIDevs()
       input_module.Config(selected_devices, selected_midi_devices)

       input_module.InitializeInputLine($InputLine)
       input_module.Run()

   func _process(_delta):
       $InputLine.emit_input_signal()

   func _on_input_line_pdje_input_keyboard_signal(device_id, device_name,
                                                  microsecond_string,
                                                  keyboard_key, is_pressed):
       print(device_id, device_name, microsecond_string, keyboard_key,
             is_pressed)

   func _on_input_line_pdje_midi_input_signal(port_name, input_type, channel,
                                              position, value,
                                              microsecond_string):
       print(port_name, input_type, channel, position, value,
             microsecond_string)
