Input_Engine
======================

The PDJE Input Engine is a cross-platform, low-latency module for handling input from keyboards, mice, and MIDI devices. It is designed for high-performance, real-time applications where responsive input is critical.

Platform Support
-----------------

The Input Engine provides platform-specific backends to achieve the best possible performance:

*   **Windows:** Uses the **Raw Input API** to capture keyboard and mouse events. To ensure low latency and isolation, input is handled in a separate child process, with data securely transmitted back to the main process using a robust Inter-Process Communication (IPC) system.
*   **Linux:** Uses the **`evdev`** (event device) interface to directly read events from input devices, bypassing higher-level abstractions for lower latency.
*   **macOS:** Planned, but not implemented in the current source tree.

Usage Overview
--------------

The primary interface for the Input Engine is the `PDJE_Input` class. Its typical lifecycle involves:

1.  **Initialization:** Calling `Init()` to prepare the input system.
2.  **Device Discovery:** Using `GetDevs()` and `GetMIDIDevs()` to identify available input devices.
3.  **Configuration:** Selecting specific devices and passing them to `Config()` to enable input processing for those devices.
4.  **Data Line Connection:** Obtaining an `PDJE_INPUT_DATA_LINE` via `PullOutDataLine()` to receive processed input events.
5.  **Running the Loop:** Starting the input processing with `Run()`.
6.  **Event Consumption:** Continuously reading events from the `PDJE_INPUT_DATA_LINE`.
7.  **Termination:** Stopping the input loop with `Kill()` when input is no longer needed.

Classes & Functions
-------------------

.. doxygenclass:: PDJE_Input
    :project: Project_DJ_Engine

    The `PDJE_Input` class manages all aspects of input device interaction. It abstracts away platform-specific implementations and provides a unified API for keyboard, mouse, and MIDI input.

.. doxygenstruct:: DeviceData
    :project: Project_DJ_Engine

    The `DeviceData` struct provides a standardized way to describe an input device, including its type (e.g., `MOUSE`, `KEYBOARD`), a human-readable name, and a platform-specific identifier.

.. doxygenenum:: PDJE_Dev_Type
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::Init()
    :project: Project_DJ_Engine

    Initializes the internal components of the input engine, including platform-specific backends and the MIDI engine. This must be called before discovering or configuring devices.

.. doxygenfunction:: PDJE_Input::Config
    :project: Project_DJ_Engine

    Configures the input engine with the selected `DeviceData` for standard input (keyboard/mouse) and `libremidi::input_port` for MIDI devices. Only configured devices will generate input events.
    
    :param devs: A vector of `DeviceData` structs representing the keyboard and mouse devices to enable. Pass an empty vector if no standard input is required.
    :param midi_dev: A vector of `libremidi::input_port` objects representing the MIDI devices to enable. Pass an empty vector if no MIDI input is required.
    :returns: `true` if configuration was successful, `false` otherwise.

.. doxygenfunction:: PDJE_Input::GetDevs
    :project: Project_DJ_Engine

    Discovers all currently connected standard input devices (keyboards and mice) and returns them as a list of `DeviceData` structs.

.. doxygenfunction:: PDJE_Input::GetMIDIDevs
    :project: Project_DJ_Engine
    
    Discovers all currently connected MIDI input devices and returns them as a list of `libremidi::input_port` objects.
    
.. doxygenfunction:: PDJE_Input::Run()
    :project: Project_DJ_Engine

    Starts the input processing loops for all configured devices. Once running, input events will be available via the `PDJE_INPUT_DATA_LINE`.

.. doxygenfunction:: PDJE_Input::Kill()
    :project: Project_DJ_Engine

    Stops all input processing loops and releases associated resources. It is good practice to call this when input is no longer needed (e.g., at application shutdown).

.. doxygenfunction:: PDJE_Input::GetState()
    :project: Project_DJ_Engine

    Returns the current operational state of the `PDJE_Input` module (e.g., `DEVICE_CONFIG_STATE`, `INPUT_LOOP_RUNNING`).

.. doxygenfunction:: PDJE_Input::PullOutDataLine
    :project: Project_DJ_Engine

    Provides access to the `PDJE_INPUT_DATA_LINE`, a zero-copy mechanism for receiving input events from the engine. This data line contains pointers to buffers where processed keyboard, mouse, and MIDI events are stored.

Example Usage
-------------
    
.. tab-set-code:: 

    .. code-block:: c++

        PDJE_Input input;
        input.Init(); // 1. Initialize the input system

        // 2. Discover available devices
        auto     devs = input.GetDevs();
        auto     midi_devs = input.GetMIDIDevs();

        // Prepare target lists for configuration
        DEV_LIST set_targets;
        std::vector<libremidi::input_port> target_midi;

        std::cout << "Available Devices:" << std::endl;
        for (auto &d : devs) {
            std::cout << "  Name: " << d.Name << ", Type: " << (d.Type == PDJE_Dev_Type::MOUSE ? "mouse" : (d.Type == PDJE_Dev_Type::KEYBOARD ? "keyboard" : "unknown")) << ", ID: " << d.device_specific_id << std::endl;
            // Example: Add all keyboards to target
            if (d.Type == PDJE_Dev_Type::KEYBOARD) {
                set_targets.push_back(d);
            }
        }
        std::cout << "Available MIDI Devices:" << std::endl;
        for (auto &m : midi_devs){
            std::cout << "  Port Name: " << m.port_name << ", API: " << m.api << ", ID: " << m.port_id << std::endl;
            // Example: Select a specific MIDI port
            // if(m.port_name == "My MIDI Controller") {
            //     target_midi.push_back(m);
            // }
            target_midi.push_back(m); // For demonstration, add all MIDI devices
        }

        // 3. Configure selected devices
        input.Config(set_targets, target_midi); 
        // input.Config(set_targets, std::vector<libremidi::input_port>()); // Config only keyboards/mice
        // input.Config(std::vector<DeviceData>(), target_midi);             // Config only MIDI

        // 4. Connect to the data line
        auto dline = input.PullOutDataLine();

        // 5. Run the input loop
        input.Run();

        std::cout << "Input loop running. Press Ctrl+C to stop." << std::endl;

        // 6. Continuously consume input events (example loop)
        while (input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_RUNNING) {
            // Handle Keyboard/Mouse Input (if configured)
            if (dline.input_arena) {
                dline.input_arena->Receive(); // On Windows, this signals and waits for data; on Linux, it gets the latest buffer
                for (const auto &i : dline.input_arena->datas) {
                    std::cout << "KM Event - Dev: " << (std::string(i.name, i.name_len)) << ", Time: " << i.microSecond;
                    if (i.type == PDJE_Dev_Type::KEYBOARD) {
                        std::cout << ", Key: " << static_cast<int>(i.event.keyboard.k) << ", Pressed: " << (i.event.keyboard.pressed ? "true" : "false");
                    } else if (i.type == PDJE_Dev_Type::MOUSE) {
                        std::cout << ", Mouse Btn: " << i.event.mouse.button_type << ", Wheel: " << i.event.mouse.wheel_move << ", X: " << i.event.mouse.x << ", Y: " << i.event.mouse.y;
                    }
                    std::cout << std::endl;
                }
            }

            // Handle MIDI Input (if configured)
            if (dline.midi_datas) {
                auto midi_events = dline.midi_datas->Get(); // Get the latest buffered MIDI events
                for (const auto &midi_ev : *midi_events) {
                    std::cout << "MIDI Event - Port: " << (std::string(midi_ev.port_name, midi_ev.port_name_len)) << ", Type: " << static_cast<int>(midi_ev.type) << ", Ch: " << static_cast<int>(midi_ev.ch) << ", Pos: " << static_cast<int>(midi_ev.pos) << ", Val: " << midi_ev.value << ", Time: " << midi_ev.highres_time << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Prevent busy-waiting
        }


        // 7. Termination
        input.Kill();
        std::cout << "Input loop terminated." << std::endl;
    
    .. code-block:: c#

        //no impl

    .. code-block:: python

        #no impl
        
    .. code-block:: gdscript
        
        extends Node
        var IM:PDJE_Input_Module

        func _ready():
            IM = PDJE_Input_Module.new()
            IM.Init()
            var configlist:Array
            for i in dev:
                if i["type"] == "KEYBOARD":
                    configlist.push_back(i)
            var mididevs = IM.GetMIDIDevs()
            print(mididevs)
            IM.Config(dev, mididevs)#if you don't need kb&mouse or midi, send blank array.

            IM.InitializeInputLine($InputLine)#connect InputLine Node. you can find it in "Add Child Node" tab
            IM.Run()

        func _process():
            $InputLine.emit_input_signal()

        #connect this function with InputLine Node. you can find it in the signals tab. beside the inspector tab.
        func _on_input_line_pdje_input_keyboard_signal(device_id:String, device_name:String, microsecond_string:String, keyboard_key:int, isPressed:bool):
            print(device_id, device_name, microsecond_string)
        
        func _on_input_line_pdje_midi_input_signal(port_name: String, input_type: String, channel: int, position: int, value: int, microsecond_string: String) -> void:
	        print(port_name, input_type, position, microsecond_string)
        
        #IM.Kill()#use kill function to deactivate input module. use after game stage.


To use PDJE without Input Engine module, use :doc:`/Data_Lines`.
