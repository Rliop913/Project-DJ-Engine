Input_Engine
======================

.. warning:: 
    Current project version is now on 0.6.x.

    Currently only Windows implementation is supported.

Usage
--------

.. doxygenclass:: PDJE_Input
    :project: Project_DJ_Engine


.. doxygenfunction:: PDJE_Input::Init()
    :project: Project_DJ_Engine


.. doxygenfunction:: PDJE_Input::Config
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
            case PDJE_Dev_Type::HID:
                std::cout << "type: hid" << std::endl;
                break;
            case PDJE_Dev_Type::UNKNOWN:
                std::cout << "type: unknown" << std::endl;
                break;
            default:
                break;
            }

            std::cout << "dev path: " << i.device_specific_id << std::endl;
        }

        input.Config(set_targets);
        auto dline = input.PullOutDataLine();
        input.Run();
        //after use
        input.Kill();


        

    .. code-block:: c#

        //no impl

    .. code-block:: python

        #no impl

    .. code-block:: gdscript

    .. code-block:: gdscript

        #with InputLine Node
        
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
            
            IM.InitializeInputLine($InputLine)#connect InputLine Node. you can find it in "Add Child Node" tab
            IM.Run()

        func _process():
            $InputLine.emit_input_signal()

        #connect this function with InputLine Node. you can find it in the signals tab. beside the inspector tab.
        func _on_input_line_pdje_input_keyboard_signal(device_id:String, device_name:String, microsecond_string:String, keyboard_key:int, isPressed:bool):
            print(device_id, device_name, microsecond_string)
        
        
        #IM.Kill()#use kill function to deactivate input module. use after game stage.


To use PDJE without Input Engine module, use :doc:`/Data_Lines`.