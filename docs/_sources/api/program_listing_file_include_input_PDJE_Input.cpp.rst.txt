
.. _program_listing_file_include_input_PDJE_Input.cpp:

Program Listing for File PDJE_Input.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.cpp>` (``include/input/PDJE_Input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Input.hpp"
   #include "PDJE_Input_StateLogic.hpp"
   #include "NameGen.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   PDJE_Input::PDJE_Input()
   {
   }
   
   bool
   PDJE_Input::Init(void *platform_ctx0, void *platform_ctx1, bool use_internal_window)
   {
       try {
           startlog();
           if (!PDJE_INPUT_STATE_LOGIC::CanInit(state)) {
               critlog(
                   "pdje input module init failed. pdje input state is not dead. "
                   "maybe input module is running or configuring.");
               return false;
           }
           platform_ctx0_ = platform_ctx0;
           platform_ctx1_ = platform_ctx1;
           use_internal_window_ = use_internal_window;
           default_devs.emplace();
           default_devs->SetPlatformContexts(platform_ctx0_,
                                             platform_ctx1_,
                                             use_internal_window_);
           default_devs->Ready();
   
           // #ifdef WIN32
   
           // PDJE_IPC::Input_Transfer_Metadata cfg;
           // PDJE_IPC::RANDOM_GEN              rg;
           // cfg.max_length              = 2048;
           // cfg.lenname                 = rg.Gen("PDJE_INPUT_LEN_");
           // cfg.bodyname                = rg.Gen("PDJE_INPUT_BODY_");
           // cfg.hmacname                = rg.Gen("PDJE_INPUT_HMAC_");
           // cfg.data_request_event_name = rg.Gen("PDJE_INPUT_REQ_EVENT_");
           // cfg.data_stored_event_name  = rg.Gen("PDJE_INPUT_STORED_EVENT_");
           // input_buffer.emplace(cfg);
           // default_devs->SendInputTransfer(input_buffer.value());
           // default_devs->InitEvents();
           // #endif
           midi_engine.emplace();
           state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
           return true;
       } catch (const std::exception &e) {
           critlog("failed to execute code. WHY: ");
           critlog(e.what());
           return false;
       }
   }
   
   bool
   PDJE_Input::Config(std::vector<DeviceData>                  &devs,
                      const std::vector<libremidi::input_port> &midi_dev)
   {
       try {
           if (!midi_dev.empty()) {
               midi_engine->configed_devices = midi_dev;
               FLAG_MIDI_ON                  = true;
           }
   
           if (!PDJE_INPUT_STATE_LOGIC::CanConfig(state)) {
               critlog(
                   "pdje input module config failed. pdje input state is not on "
                   "device config state. Init it first.");
               return false;
           }
           std::vector<DeviceData> sanitized_devs =
               PDJE_INPUT_STATE_LOGIC::SanitizeConfigDevices(devs);
           const bool has_valid_input = !sanitized_devs.empty();
           bool       backend_ok      = false;
   
           if (has_valid_input) {
               backend_ok = default_devs->Config(sanitized_devs);
           }
   
           const auto decision = PDJE_INPUT_STATE_LOGIC::DecideConfigOutcome(
               has_valid_input,
               FLAG_MIDI_ON,
               backend_ok);
           FLAG_INPUT_ON = decision.flag_input_on;
   
           if (!decision.success) {
               if (decision.backend_fail_path) {
                   critlog("failed to configure devices.");
               }
               return false;
           }
   
           state = decision.next_state;
           if (decision.should_call_kill) { // fallback: only midi devices.
               return Kill();
           }
           return true;
       } catch (const std::exception &e) {
           critlog("failed to config. WHY: ");
           critlog(e.what());
           return false;
       }
   }
   
   bool
   PDJE_Input::Run()
   {
       if (!PDJE_INPUT_STATE_LOGIC::CanRun(state)) {
           warnlog("pdje init module run failed. pdje input state is not on loop "
                   "ready state. config it first.");
           return false;
       }
   
       default_devs->RunLoop();
       if (!FLAG_INPUT_ON) { // terminate if input flag is off.
           default_devs->TerminateLoop();
       }
   
       if (FLAG_MIDI_ON) { // run midi engine if midi flag is on.
           midi_engine->Run();
       }
   
       state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
       return true;
   }
   
   bool
   PDJE_Input::Kill()
   {
       bool ok = true;
       switch (PDJE_INPUT_STATE_LOGIC::ClassifyKillAction(state)) {
       case PDJE_INPUT_STATE_LOGIC::KillAction::NoOp:
           return true;
   
       case PDJE_INPUT_STATE_LOGIC::KillAction::BackendKill: {
           if (default_devs) {
               // compatibility no-op for windows parity
               ok = default_devs->Kill();
           }
           break;
       }
       case PDJE_INPUT_STATE_LOGIC::KillAction::TerminateLoop: {
           if (default_devs) {
               default_devs->TerminateLoop();
           }
           break;
       }
       case PDJE_INPUT_STATE_LOGIC::KillAction::BrokenState:
           critlog("the pdje input module state is broken...why?");
           ok = false;
           break;
       }
       // reset datas.
       midi_engine.reset();
   
       default_devs.reset();
       FLAG_INPUT_ON = false;
       FLAG_MIDI_ON  = false;
       platform_ctx0_ = nullptr;
       platform_ctx1_ = nullptr;
       use_internal_window_ = false;
       state         = PDJE_INPUT_STATE::DEAD;
       return ok;
   }
   
   std::vector<DeviceData>
   PDJE_Input::GetDevs()
   {
   
       return default_devs->GetDevices();
   }
   
   std::vector<libremidi::input_port>
   PDJE_Input::GetMIDIDevs()
   {
       return midi_engine->GetDevices();
   }
   
   PDJE_INPUT_STATE
   PDJE_Input::GetState()
   {
       return state;
   }
   
   std::string
   PDJE_Input::GetCurrentInputBackend() const
   {
       if (!default_devs) {
           return "none";
       }
       return default_devs->GetCurrentBackendString();
   }
   
   PDJE_INPUT_DATA_LINE
   PDJE_Input::PullOutDataLine()
   {
       PDJE_INPUT_DATA_LINE line;
       if (FLAG_INPUT_ON) {
           line.input_arena = default_devs->GetInputBufferPTR();
       }
       if (FLAG_MIDI_ON) {
           line.midi_datas = &midi_engine->evlog;
       }
       return line; // you should check nullptr before use.
   }
