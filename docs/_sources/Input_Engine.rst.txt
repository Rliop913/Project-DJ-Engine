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

        var pdjeinput:PDJE_Input_Module = PDJE_Input_Module.new()
        pdjeinput.Init()
        var devs:Array = pdjeinput.GetDevs()
        print(devs)
        Config(devs)
        var input_line:InputLine = pdjeinput.PullOutDataLine()
        pdjeinput.Run()

        #after use

        pdjeinput.Kill()

see you soon.

To use PDJE without Input Engine module, use :doc:`/Data_Lines`.