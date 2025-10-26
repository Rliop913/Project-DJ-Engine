
.. _program_listing_file_include_input_PDJE_Input.cpp:

Program Listing for File PDJE_Input.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.cpp>` (``include/input/PDJE_Input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Input.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #define PDJE_INPUT_DEFAULT_TRY_CATCH(CODE)                                     \
       try {                                                                      \
           CODE                                                                   \
       } catch (const std::exception &e) {                                        \
           ResetOneShot(config_promise, data.config_data, data.config_sync);      \
           ResetOneShot(run_command, data.run_ok, data.run_sync);                 \
                                                                                  \
           state = PDJE_INPUT_STATE::DEAD;                                        \
           critlog("failed to execute code. WHY: ");                              \
           critlog(e.what());                                                     \
           ErrLog += e.what();                                                    \
           ErrLog += "\n";                                                        \
           return false;                                                          \
       }
   
   PDJE_Input::PDJE_Input()
   {
       startlog();
   }
   
   bool
   PDJE_Input::Init()
   {
       if (state != PDJE_INPUT_STATE::DEAD) {
           warnlog("pdje input module init failed. pdje input state is not dead. "
                   "maybe input module is running or configuring.");
           return false;
       }
       PDJE_INPUT_DEFAULT_TRY_CATCH(
           InitOneShot(config_promise, data.config_data, data.config_sync);
           InitOneShot(run_command, data.run_ok, data.run_sync);
           data.TrigLoop();
           state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
           return true;)
   }
   
   bool
   PDJE_Input::Config(std::vector<DeviceData> &devs)
   {
       std::vector<DeviceData> sanitized_devs;
       for (const auto &d : devs) {
           if (d.Name != "" && d.device_specific_id != "" &&
               d.Type != PDJE_Dev_Type::UNKNOWN) {
               sanitized_devs.push_back(d);
           }
       }
       if (state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
           warnlog("pdje input module config failed. pdje input state is not on "
                   "device config state. Init it first.");
           return false;
       }
       PDJE_INPUT_DEFAULT_TRY_CATCH(config_promise->set_value(sanitized_devs);
                                    data.config_sync->arrive_and_wait();)
   
       state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
       return true;
   }
   
   bool
   PDJE_Input::Run()
   {
       if (state != PDJE_INPUT_STATE::INPUT_LOOP_READY) {
           warnlog("pdje init module run failed. pdje input state is not on loop "
                   "ready state. config it first.");
           return false;
       }
   
       PDJE_INPUT_DEFAULT_TRY_CATCH(run_command->set_value(true);
                                    data.run_sync->arrive_and_wait();)
   
       state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
       return true;
   }
   
   bool
   PDJE_Input::Kill()
   {
       switch (state) {
       case PDJE_INPUT_STATE::DEAD:
           return true;
   
       case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE: {
           auto empty_devs = DEV_LIST();
           Config(empty_devs);
           data.config_sync->arrive_and_wait();
           break;
       }
       case PDJE_INPUT_STATE::INPUT_LOOP_READY:
           PDJE_INPUT_DEFAULT_TRY_CATCH(run_command->set_value(false);
                                        data.run_sync->arrive_and_wait();)
           break;
       case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING: {
           if (!data.kill()) {
               critlog("failed to kill pdje input module. maybe thread is broken. "
                       "issue this.");
               return false;
           }
       } break;
       default:
           critlog("the pdje input module state is broken...why?");
           return false;
       }
       data.ResetLoop();
       state = PDJE_INPUT_STATE::DEAD;
       ResetOneShot(config_promise, data.config_data, data.config_sync);
       ResetOneShot(run_command, data.run_ok, data.run_sync);
       return true;
   }
   
   std::vector<DeviceData>
   PDJE_Input::GetDevs()
   {
       return data.getDevices();
   }
   
   PDJE_INPUT_STATE
   PDJE_Input::GetState()
   {
       return state;
   }
   
   PDJE_INPUT_DATA_LINE
   PDJE_Input::PullOutDataLine()
   {
       return data.PullOutDataLine();
   }
