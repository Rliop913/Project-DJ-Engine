Input_Engine
======================

.. warning:: 
    Current project version is now on 0.7.x.

    Currently only Windows implementation is supported.

Usage
--------

.. doxygenclass:: PDJE_Input
    :project: Project_DJ_Engine


.. doxygenfunction:: PDJE_Input::Init()
    :project: Project_DJ_Engine


.. doxygenfunction:: PDJE_Input::Config
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetDevs
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetMIDIDevs
    :project: Project_DJ_Engine
    

.. doxygenfunction:: PDJE_Input::Run()
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::Kill()
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE_Input::GetState()
    :project: Project_DJ_Engine

    
    
.. tab-set-code:: 

    .. code-block:: c++

        PDJE_Input input;
        input.Init();

        auto     devs = input.GetDevs();
        DEV_LIST set_targets;
        for (auto i : devs) {
            std::cout << "name: " << i.Name << std::endl;
            switch (i.Type) {
            case PDJE_Dev_Type::MOUSE:
                std::cout << "type: mouse" << std::endl;
                break;
            case PDJE_Dev_Type::KEYBOARD:
                std::cout << "type: keyboard" << std::endl;
                set_targets.push_back(i);//add all keyboards
                break;
            default:
                break;
            }

            std::cout << "dev path: " << i.device_specific_id << std::endl;
        }// configure keyboards, mouses.

        auto midi_devs = input.GetMIDIDevs();
        std::vector<libremidi::input_port> target_midi;
        for(auto i : midi_devs){
            if(i.port_name == "target midi port name"){
            target_midi.push_back(i);
            }
        }

        input.Config(set_targets, target_midi);//config keyboard, mouse, midi
        //input.Config(set_targets, std::vector<libremidi::input_port>()); //config only keyboard, mouse
        //input.Config(std::vector<DeviceData>(), target_midi); //config only midi
        auto dline = input.PullOutDataLine();
        input.Run();
        //after use
        input.Kill();


        

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