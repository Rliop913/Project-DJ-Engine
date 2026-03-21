
.. _program_listing_file_include_input_CPDJE_Input.cpp:

Program Listing for File CPDJE_Input.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_CPDJE_Input.cpp>` (``include/input/CPDJE_Input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "CPDJE_Input.h"
   
   #include "PDJE_CAbi_Input_Private.hpp"
   #include "PDJE_Input.hpp"
   #include "PDJE_Input_Log.hpp"
   #include "PDJE_Input_StateLogic.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   #include <algorithm>
   #include <cstddef>
   #include <cstdint>
   #include <exception>
   #include <string>
   #include <vector>
   
   struct PDJE_InputSnapshotHandleV1 {
       bool                            has_input_stream = false;
       bool                            has_midi_stream  = false;
       std::vector<PDJE_Input_Log>     input_events;
       std::vector<PDJE_MIDI::MIDI_EV> midi_events;
   };
   
   namespace {
   
   PDJE_Input *
   GetInput(PDJE_InputHandleV1 *handle) noexcept
   {
       return handle == nullptr ? nullptr : static_cast<PDJE_Input *>(handle->input);
   }
   
   const PDJE_Input *
   GetInput(const PDJE_InputHandleV1 *handle) noexcept
   {
       return handle == nullptr ? nullptr : static_cast<const PDJE_Input *>(handle->input);
   }
   
   void
   RefreshInputDataLineCache(PDJE_InputHandleV1 *handle) noexcept
   {
       if (handle == nullptr) {
           return;
       }
       auto *input = GetInput(handle);
       if (input == nullptr) {
           handle->input_arena = nullptr;
           handle->midi_datas  = nullptr;
           return;
       }
       const auto line = input->PullOutDataLine();
       handle->input_arena = line.input_arena;
       handle->midi_datas  = line.midi_datas;
   }
   
   template <typename Fn> PDJE_InputResultV1
   GuardInputAbi(const char *context, Fn &&fn) noexcept
   {
       try {
           return fn();
       } catch (const std::exception &e) {
           critlog(context);
           critlog(e.what());
           return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
       } catch (...) {
           critlog(context);
           return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
       }
   }
   
   template <typename T> bool
   StructIsCompatible(const T *value) noexcept
   {
       if (value == nullptr) {
           return false;
       }
       return value->struct_size == 0 || value->struct_size >= sizeof(T);
   }
   
   PDJE_InputStringViewV1
   MakeStringView(const std::string &value) noexcept
   {
       if (value.empty()) {
           return {};
       }
       return PDJE_InputStringViewV1 { value.c_str(), value.size() };
   }
   
   PDJE_InputStringViewV1
   MakeCountedStringView(const char *value, std::size_t value_size) noexcept
   {
       if (value == nullptr || value_size == 0) {
           return {};
       }
       return PDJE_InputStringViewV1 { value, value_size };
   }
   
   PDJE_InputBytesViewV1
   MakeBytesView(const uint8_t *value, std::size_t value_size) noexcept
   {
       if (value == nullptr || value_size == 0) {
           return {};
       }
       return PDJE_InputBytesViewV1 { value, value_size };
   }
   
   PDJE_InputDeviceTypeV1
   ToCDeviceType(const PDJE_Dev_Type type) noexcept
   {
       switch (type) {
       case PDJE_Dev_Type::MOUSE:
           return PDJE_INPUT_DEVICE_MOUSE_V1;
       case PDJE_Dev_Type::KEYBOARD:
           return PDJE_INPUT_DEVICE_KEYBOARD_V1;
       default:
           return PDJE_INPUT_DEVICE_UNKNOWN_V1;
       }
   }
   
   PDJE_InputStateV1
   ToCState(const PDJE_INPUT_STATE state) noexcept
   {
       switch (state) {
       case PDJE_INPUT_STATE::DEVICE_CONFIG_STATE:
           return PDJE_INPUT_STATE_DEVICE_CONFIG_V1;
       case PDJE_INPUT_STATE::INPUT_LOOP_READY:
           return PDJE_INPUT_STATE_LOOP_READY_V1;
       case PDJE_INPUT_STATE::INPUT_LOOP_RUNNING:
           return PDJE_INPUT_STATE_LOOP_RUNNING_V1;
       default:
           return PDJE_INPUT_STATE_DEAD_V1;
       }
   }
   
   bool
   CanEnumerate(const PDJE_InputHandleV1 *input) noexcept
   {
       auto *input_obj = GetInput(const_cast<PDJE_InputHandleV1 *>(input));
       return input_obj != nullptr &&
              const_cast<PDJE_Input &>(*input_obj).GetState() != PDJE_INPUT_STATE::DEAD;
   }
   
   void
   ResetDeviceView(PDJE_InputDeviceViewV1 *out_device) noexcept
   {
       if (out_device == nullptr) {
           return;
       }
       const auto struct_size = out_device->struct_size;
       *out_device            = {};
       out_device->struct_size =
           struct_size != 0 ? struct_size : sizeof(*out_device);
   }
   
   void
   ResetMidiDeviceView(PDJE_MidiDeviceViewV1 *out_device) noexcept
   {
       if (out_device == nullptr) {
           return;
       }
       const auto struct_size = out_device->struct_size;
       *out_device            = {};
       out_device->struct_size =
           struct_size != 0 ? struct_size : sizeof(*out_device);
   }
   
   void
   ResetInputEventView(PDJE_InputEventViewV1 *out_event) noexcept
   {
       if (out_event == nullptr) {
           return;
       }
       const auto struct_size = out_event->struct_size;
       *out_event             = {};
       out_event->struct_size =
           struct_size != 0 ? struct_size : sizeof(*out_event);
   }
   
   void
   ResetMidiEventView(PDJE_MidiEventViewV1 *out_event) noexcept
   {
       if (out_event == nullptr) {
           return;
       }
       const auto struct_size = out_event->struct_size;
       *out_event             = {};
       out_event->struct_size =
           struct_size != 0 ? struct_size : sizeof(*out_event);
   }
   
   void
   ResetSnapshotInfo(PDJE_InputSnapshotInfoV1 *out_info) noexcept
   {
       if (out_info == nullptr) {
           return;
       }
       const auto struct_size = out_info->struct_size;
       *out_info              = {};
       out_info->struct_size =
           struct_size != 0 ? struct_size : sizeof(*out_info);
   }
   
   template <typename HandleT, typename ItemT> PDJE_InputResultV1
   GatherSelectedItems(const HandleT      *list,
                       const std::size_t  *indices,
                       const std::size_t   index_count,
                       std::vector<ItemT> &out_items) noexcept
   {
       out_items.clear();
       if (index_count == 0) {
           return PDJE_INPUT_RESULT_OK_V1;
       }
       if (list == nullptr || indices == nullptr) {
           return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
       }
   
       std::vector<std::size_t> seen;
       seen.reserve(index_count);
       out_items.reserve(index_count);
       for (std::size_t i = 0; i < index_count; ++i) {
           const auto idx = indices[i];
           if (idx >= list->items.size()) {
               return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
           }
           if (std::find(seen.begin(), seen.end(), idx) != seen.end()) {
               continue;
           }
           seen.push_back(idx);
           out_items.push_back(list->items[idx]);
       }
       return PDJE_INPUT_RESULT_OK_V1;
   }
   
   } // namespace
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_create_v1(PDJE_InputHandleV1 **out_input)
   {
       return GuardInputAbi("pdje_input_create_v1 failed", [&]() {
           if (out_input == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *handle = new PDJE_InputHandleV1();
           handle->input = new PDJE_Input();
           RefreshInputDataLineCache(handle);
           *out_input = handle;
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_input_destroy_v1(PDJE_InputHandleV1 *input)
   {
       delete GetInput(input);
       delete input;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_init_v1(PDJE_InputHandleV1 *input,
                      void               *platform_ctx0,
                      void               *platform_ctx1,
                      int                 use_internal_window)
   {
       return GuardInputAbi("pdje_input_init_v1 failed", [&]() {
           if (input == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           if (!PDJE_INPUT_STATE_LOGIC::CanInit(input_obj->GetState())) {
               return PDJE_INPUT_RESULT_INVALID_STATE_V1;
           }
           if (!input_obj->Init(platform_ctx0, platform_ctx1, use_internal_window != 0)) {
               return PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
           }
           RefreshInputDataLineCache(input);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_kill_v1(PDJE_InputHandleV1 *input)
   {
       return GuardInputAbi("pdje_input_kill_v1 failed", [&]() {
           if (input == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           const auto ok = input_obj->Kill();
           RefreshInputDataLineCache(input);
           return ok ? PDJE_INPUT_RESULT_OK_V1 : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_get_state_v1(const PDJE_InputHandleV1 *input,
                           PDJE_InputStateV1        *out_state)
   {
       return GuardInputAbi("pdje_input_get_state_v1 failed", [&]() {
           if (input == nullptr || out_state == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *input_obj = GetInput(const_cast<PDJE_InputHandleV1 *>(input));
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           *out_state = ToCState(const_cast<PDJE_Input &>(*input_obj).GetState());
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_get_backend_name_v1(PDJE_InputHandleV1     *input,
                                  PDJE_InputStringViewV1 *out_backend)
   {
       return GuardInputAbi("pdje_input_get_backend_name_v1 failed", [&]() {
           if (input == nullptr || out_backend == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           *out_backend               = {};
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           input->backend_name_cache = input_obj->GetCurrentInputBackend();
           *out_backend               = MakeStringView(input->backend_name_cache);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_list_devices_v1(PDJE_InputHandleV1           *input,
                              PDJE_InputDeviceListHandleV1 **out_list)
   {
       return GuardInputAbi("pdje_input_list_devices_v1 failed", [&]() {
           if (input == nullptr || out_list == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (!CanEnumerate(input)) {
               return PDJE_INPUT_RESULT_INVALID_STATE_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           auto *list = new PDJE_InputDeviceListHandleV1();
           list->items = input_obj->GetDevs();
           *out_list   = list;
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   size_t PDJE_CALL
   pdje_input_device_list_size_v1(const PDJE_InputDeviceListHandleV1 *list)
   {
       return list != nullptr ? list->items.size() : 0;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_device_list_get_v1(const PDJE_InputDeviceListHandleV1 *list,
                                 size_t                              index,
                                 PDJE_InputDeviceViewV1            *out_device)
   {
       return GuardInputAbi("pdje_input_device_list_get_v1 failed", [&]() {
           if (list == nullptr || !StructIsCompatible(out_device)) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= list->items.size()) {
               return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
           }
           ResetDeviceView(out_device);
           const auto &item             = list->items[index];
           out_device->type             = ToCDeviceType(item.Type);
           out_device->name             = MakeStringView(item.Name);
           out_device->device_specific_id = MakeStringView(item.device_specific_id);
           out_device->struct_size      = sizeof(*out_device);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_input_device_list_destroy_v1(PDJE_InputDeviceListHandleV1 *list)
   {
       delete list;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_list_midi_devices_v1(PDJE_InputHandleV1          *input,
                                   PDJE_MidiDeviceListHandleV1 **out_list)
   {
       return GuardInputAbi("pdje_input_list_midi_devices_v1 failed", [&]() {
           if (input == nullptr || out_list == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (!CanEnumerate(input)) {
               return PDJE_INPUT_RESULT_INVALID_STATE_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           auto *list = new PDJE_MidiDeviceListHandleV1();
           list->items = input_obj->GetMIDIDevs();
           *out_list   = list;
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   size_t PDJE_CALL
   pdje_input_midi_device_list_size_v1(const PDJE_MidiDeviceListHandleV1 *list)
   {
       return list != nullptr ? list->items.size() : 0;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_midi_device_list_get_v1(const PDJE_MidiDeviceListHandleV1 *list,
                                      size_t                             index,
                                      PDJE_MidiDeviceViewV1            *out_device)
   {
       return GuardInputAbi("pdje_input_midi_device_list_get_v1 failed", [&]() {
           if (list == nullptr || !StructIsCompatible(out_device)) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= list->items.size()) {
               return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
           }
           ResetMidiDeviceView(out_device);
           const auto &item          = list->items[index];
           out_device->manufacturer  = MakeStringView(item.manufacturer);
           out_device->device_name   = MakeStringView(item.device_name);
           out_device->port_name     = MakeStringView(item.port_name);
           out_device->display_name  = MakeStringView(item.display_name);
           out_device->client_handle = item.client;
           out_device->port_handle   = item.port;
           out_device->port_type     = static_cast<uint8_t>(item.type);
           out_device->struct_size   = sizeof(*out_device);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_input_midi_device_list_destroy_v1(PDJE_MidiDeviceListHandleV1 *list)
   {
       delete list;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_config_v1(PDJE_InputHandleV1                 *input,
                        const PDJE_InputDeviceListHandleV1 *devices,
                        const std::size_t                  *device_indices,
                        std::size_t                         device_index_count,
                        const PDJE_MidiDeviceListHandleV1  *midi_devices,
                        const std::size_t                  *midi_indices,
                        std::size_t                         midi_index_count)
   {
       return GuardInputAbi("pdje_input_config_v1 failed", [&]() {
           if (input == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           if (!PDJE_INPUT_STATE_LOGIC::CanConfig(input_obj->GetState())) {
               return PDJE_INPUT_RESULT_INVALID_STATE_V1;
           }
   
           std::vector<DeviceData> selected_devices;
           const auto device_result = GatherSelectedItems(
               devices, device_indices, device_index_count, selected_devices);
           if (device_result != PDJE_INPUT_RESULT_OK_V1) {
               return device_result;
           }
   
           std::vector<libremidi::input_port> selected_midi_devices;
           const auto midi_result = GatherSelectedItems(
               midi_devices, midi_indices, midi_index_count, selected_midi_devices);
           if (midi_result != PDJE_INPUT_RESULT_OK_V1) {
               return midi_result;
           }
   
           const auto ok = input_obj->Config(selected_devices, selected_midi_devices);
           RefreshInputDataLineCache(input);
           return ok
                      ? PDJE_INPUT_RESULT_OK_V1
                      : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_run_v1(PDJE_InputHandleV1 *input)
   {
       return GuardInputAbi("pdje_input_run_v1 failed", [&]() {
           if (input == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           if (!PDJE_INPUT_STATE_LOGIC::CanRun(input_obj->GetState())) {
               return PDJE_INPUT_RESULT_INVALID_STATE_V1;
           }
           const auto ok = input_obj->Run();
           RefreshInputDataLineCache(input);
           return ok ? PDJE_INPUT_RESULT_OK_V1 : PDJE_INPUT_RESULT_OPERATION_FAILED_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_poll_snapshot_v1(PDJE_InputHandleV1         *input,
                               PDJE_InputSnapshotHandleV1 **out_snapshot)
   {
       return GuardInputAbi("pdje_input_poll_snapshot_v1 failed", [&]() {
           if (input == nullptr || out_snapshot == nullptr) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           auto *snapshot = new PDJE_InputSnapshotHandleV1();
           auto *input_obj = GetInput(input);
           if (input_obj == nullptr) {
               return PDJE_INPUT_RESULT_INTERNAL_ERROR_V1;
           }
           RefreshInputDataLineCache(input);
           snapshot->has_input_stream = input->input_arena != nullptr;
           snapshot->has_midi_stream  = input->midi_datas != nullptr;
   
           if (input->input_arena != nullptr) {
               auto *arena = static_cast<PDJE_IPC::PDJE_Input_Transfer *>(input->input_arena);
               arena->Receive();
               snapshot->input_events = arena->datas;
           }
           if (input->midi_datas != nullptr) {
               auto *midi_buffer = static_cast<Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> *>(input->midi_datas);
               const auto *midi_events = midi_buffer->Get();
               if (midi_events != nullptr) {
                   snapshot->midi_events = *midi_events;
               }
           }
   
           *out_snapshot = snapshot;
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_describe_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                   PDJE_InputSnapshotInfoV1         *out_info)
   {
       return GuardInputAbi("pdje_input_snapshot_describe_v1 failed", [&]() {
           if (snapshot == nullptr || !StructIsCompatible(out_info)) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           ResetSnapshotInfo(out_info);
           out_info->has_input_stream = snapshot->has_input_stream ? 1 : 0;
           out_info->has_midi_stream  = snapshot->has_midi_stream ? 1 : 0;
           out_info->input_event_count = snapshot->input_events.size();
           out_info->midi_event_count  = snapshot->midi_events.size();
           out_info->struct_size       = sizeof(*out_info);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   size_t PDJE_CALL
   pdje_input_snapshot_input_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot)
   {
       return snapshot != nullptr ? snapshot->input_events.size() : 0;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_input_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                    size_t                            index,
                                    PDJE_InputEventViewV1           *out_event)
   {
       return GuardInputAbi("pdje_input_snapshot_input_get_v1 failed", [&]() {
           if (snapshot == nullptr || !StructIsCompatible(out_event)) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= snapshot->input_events.size()) {
               return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
           }
   
           ResetInputEventView(out_event);
           const auto &item = snapshot->input_events[index];
           out_event->type  = ToCDeviceType(item.type);
           out_event->id    = MakeCountedStringView(
               item.id, std::min<std::size_t>(item.id_len, sizeof(item.id)));
           out_event->name = MakeCountedStringView(
               item.name, std::min<std::size_t>(item.name_len, sizeof(item.name)));
           out_event->microsecond       = item.microSecond;
           out_event->keyboard.key_code =
               static_cast<uint32_t>(item.event.keyboard.k);
           out_event->keyboard.pressed  = item.event.keyboard.pressed ? 1 : 0;
           out_event->mouse.button_type = item.event.mouse.button_type;
           out_event->mouse.wheel_move  = item.event.mouse.wheel_move;
           out_event->mouse.axis_type =
               static_cast<uint32_t>(item.event.mouse.axis_type);
           out_event->mouse.x = item.event.mouse.x;
           out_event->mouse.y = item.event.mouse.y;
           out_event->hid_report = MakeBytesView(
               item.hid_event.hid_buffer,
               std::min<std::size_t>(
                   static_cast<std::size_t>(item.hid_event.hid_byte_size),
                   sizeof(item.hid_event.hid_buffer)));
           out_event->struct_size = sizeof(*out_event);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   size_t PDJE_CALL
   pdje_input_snapshot_midi_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot)
   {
       return snapshot != nullptr ? snapshot->midi_events.size() : 0;
   }
   
   PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_midi_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                   size_t                            index,
                                   PDJE_MidiEventViewV1            *out_event)
   {
       return GuardInputAbi("pdje_input_snapshot_midi_get_v1 failed", [&]() {
           if (snapshot == nullptr || !StructIsCompatible(out_event)) {
               return PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1;
           }
           if (index >= snapshot->midi_events.size()) {
               return PDJE_INPUT_RESULT_OUT_OF_RANGE_V1;
           }
   
           ResetMidiEventView(out_event);
           const auto &item      = snapshot->midi_events[index];
           out_event->type       = item.type;
           out_event->channel    = item.ch;
           out_event->position   = item.pos;
           out_event->value      = item.value;
           out_event->highres_time = item.highres_time;
           out_event->port_name  = MakeCountedStringView(
               item.port_name,
               std::min<std::size_t>(item.port_name_len, sizeof(item.port_name)));
           out_event->struct_size = sizeof(*out_event);
           return PDJE_INPUT_RESULT_OK_V1;
       });
   }
   
   void PDJE_CALL
   pdje_input_snapshot_destroy_v1(PDJE_InputSnapshotHandleV1 *snapshot)
   {
       delete snapshot;
   }
   
   
   
