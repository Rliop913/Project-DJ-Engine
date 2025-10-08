
.. _program_listing_file_include_input_PDJE_Input.cpp:

Program Listing for File PDJE_Input.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.cpp>` (``include/input/PDJE_Input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Input.hpp"
   
   #define PDJE_INPUT_DEFAULT_TRY_CATCH(CODE)                                     \
       try {                                                                      \
           CODE                                                                   \
       } catch (const std::exception &e) {                                        \
           ResetOneShot(config_promise, data.config_data);                        \
           ResetOneShot(run_command, data.run_ok);                                \
                                                                                  \
           state = PDJE_INPUT_STATE::DEAD;                                        \
           ErrLog += e.what();                                                    \
           ErrLog += "\n";                                                        \
           return false;                                                          \
       }
   
   bool
   PDJE_Input::Init()
   {
       if (state != PDJE_INPUT_STATE::DEAD) {
           return false;
       }
       PDJE_INPUT_DEFAULT_TRY_CATCH(InitOneShot(config_promise, data.config_data);
                                    InitOneShot(run_command, data.run_ok);
                                    data.TrigLoop();
                                    state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
                                    return true;)
   }
   
   bool
   PDJE_Input::Config(std::vector<DeviceData> &devs)
   {
       if (state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
           return false;
       }
       PDJE_INPUT_DEFAULT_TRY_CATCH(config_promise->set_value(devs);)
   
       state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
       return true;
   }
   
   bool
   PDJE_Input::Run()
   {
       if (state != PDJE_INPUT_STATE::INPUT_LOOP_READY) {
           return false;
       }
   
       PDJE_INPUT_DEFAULT_TRY_CATCH(run_command->set_value(true);)
   
       state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
       return true;
   }
   
   bool
   PDJE_Input::Kill()
   {
       if (state != PDJE_INPUT_STATE::INPUT_LOOP_RUNNING) {
           return false;
       }
   
       bool flagOK = data.kill();
       if (!flagOK) {
           return false;
       }
       data.ResetLoop();
       state = PDJE_INPUT_STATE::DEAD;
       ResetOneShot(config_promise, data.config_data);
       ResetOneShot(run_command, data.run_ok);
       return true;
   }
   
   std::vector<DeviceData>
   PDJE_Input::GetDevs()
   {
       return data.getDevices();
   }
   
   void
   PDJE_Input::SetDevs(const std::vector<DeviceData> &devs)
   {
       activated_devices = devs;
   }
   
   PDJE_INPUT_STATE
   PDJE_Input::GetState()
   {
       return state;
   }
   
   PDJE_INPUT_STATE
   PDJE_Input::NEXT()
   {
       switch (state) {
       case PDJE_INPUT_STATE::DEAD:
           Init();
           return state;
       case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE:
           if (activated_devices.empty()) {
               return state;
           }
           Config(activated_devices);
           return state;
   
       case PDJE_INPUT_STATE::INPUT_LOOP_READY:
           Run();
           return state;
       case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING:
           Kill();
           return state;
   
       default:
           throw std::logic_error("State machine broken.");
       }
   }
   
   PDJE_INPUT_DATA_LINE
   PDJE_Input::PullOutDataLine()
   {
       return data.PullOutDataLine();
   }
