
.. _program_listing_file_include_input_PDJE_Input.cpp:

Program Listing for File PDJE_Input.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.cpp>` (``include/input/PDJE_Input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Input.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   PDJE_Input::PDJE_Input() : input_buffer(2048)
   {
       startlog();
   }
   
   bool
   PDJE_Input::Init()
   {
       try {
           if (state != PDJE_INPUT_STATE::DEAD) {
               critlog(
                   "pdje input module init failed. pdje input state is not dead. "
                   "maybe input module is running or configuring.");
               return false;
           }
           int port;
           {
               httplib::Server portGetter;
               port = portGetter.bind_to_any_port("0.0.0.0");
           }
   
           http_bridge.emplace(port);
   
           http_bridge->SendBufferArena(input_buffer);
   
           spinlock_run.MakeIPCSharedMemory(std::string("PDJE_SPINLOCK"), 1);
           (*spinlock_run.ptr) = 0;
           http_bridge->SendIPCSharedMemory(
               spinlock_run, std::string("PDJE_SPINLOCK"), "spinlock");
           state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
           return true;
       } catch (const std::exception &e) {
           critlog("failed to execute code. WHY: ");
           critlog(e.what());
           return false;
       }
   }
   
   bool
   PDJE_Input::Config(std::vector<DeviceData> &devs)
   {
       try {
           std::vector<DeviceData> sanitized_devs;
           for (const auto &d : devs) {
               if (d.Name != "" && d.device_specific_id != "" &&
                   d.Type != PDJE_Dev_Type::UNKNOWN) {
                   sanitized_devs.push_back(d);
               }
           }
           if (state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE) {
               critlog(
                   "pdje input module config failed. pdje input state is not on "
                   "device config state. Init it first.");
               return false;
           }
           nlohmann::json nj;
           nj["body"] = nlohmann::json::array();
           for (const auto &dev : sanitized_devs) {
               std::unordered_map<std::string, std::string> kv;
               kv["id"]   = dev.device_specific_id;
               kv["name"] = dev.Name;
               switch (dev.Type) {
               case PDJE_Dev_Type::KEYBOARD:
                   kv["type"] = "KEYBOARD";
                   nj["body"].push_back(kv);
                   break;
               case PDJE_Dev_Type::MOUSE:
                   kv["type"] = "MOUSE";
                   nj["body"].push_back(kv);
                   break;
               case PDJE_Dev_Type::MIDI:
                   kv["type"] = "MIDI";
                   nj["body"].push_back(kv);
                   break;
               case PDJE_Dev_Type::HID:
                   kv["type"] = "HID";
                   nj["body"].push_back(kv);
                   break;
               default:
                   break;
               }
           }
           http_bridge->QueryConfig(nj.dump());
           state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
           return http_bridge->EndTransmission();
   
       } catch (const std::exception &e) {
           critlog("failed to config. WHY: ");
           critlog(e.what());
           return false;
       }
   }
   
   bool
   PDJE_Input::Run()
   {
       if (state != PDJE_INPUT_STATE::INPUT_LOOP_READY) {
           warnlog("pdje init module run failed. pdje input state is not on loop "
                   "ready state. config it first.");
           return false;
       }
       (*spinlock_run.ptr) = 1;
   
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
           return http_bridge->Kill();
       }
       case PDJE_INPUT_STATE::INPUT_LOOP_READY:
           (*spinlock_run.ptr) = -1;
           break;
       case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING: {
           (*spinlock_run.ptr) = 0;
           break;
       } break;
       default:
           critlog("the pdje input module state is broken...why?");
           return false;
       }
       state = PDJE_INPUT_STATE::DEAD;
       return true;
   }
   
   std::vector<DeviceData>
   PDJE_Input::GetDevs()
   {
       return http_bridge->GetDevices();
   }
   
   PDJE_INPUT_STATE
   PDJE_Input::GetState()
   {
       return state;
   }
   
   PDJE_INPUT_DATA_LINE
   PDJE_Input::PullOutDataLine()
   {
       PDJE_INPUT_DATA_LINE line;
       line.input_arena = &input_buffer;
       return line;
   }
