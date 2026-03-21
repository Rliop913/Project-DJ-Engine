
.. _program_listing_file_include_input_CPDJE_Input.h:

Program Listing for File CPDJE_Input.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_CPDJE_Input.h>` (``include/input/CPDJE_Input.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   #include <stddef.h>
   #include <stdint.h>
   
   #define PDJE_MOUSE_L_BTN_DOWN 0x0001
   #define PDJE_MOUSE_L_BTN_UP 0x0002
   #define PDJE_MOUSE_R_BTN_DOWN 0x0004
   #define PDJE_MOUSE_R_BTN_UP 0x0008
   #define PDJE_MOUSE_M_BTN_DOWN 0x0010
   #define PDJE_MOUSE_M_BTN_UP 0x0020
   #define PDJE_MOUSE_SIDE_BTN_DOWN 0x0040
   #define PDJE_MOUSE_SIDE_BTN_UP 0x0080
   #define PDJE_MOUSE_EX_BTN_DOWN 0x0100
   #define PDJE_MOUSE_EX_BTN_UP 0x0200
   #define PDJE_MOUSE_YWHEEL 0x0400
   #define PDJE_MOUSE_XWHEEL 0x0800
   
   #ifdef __cplusplus
   extern "C" {
   #endif
   
   typedef struct PDJE_InputHandleV1 PDJE_InputHandleV1;
   typedef struct PDJE_InputDeviceListHandleV1 PDJE_InputDeviceListHandleV1;
   typedef struct PDJE_MidiDeviceListHandleV1 PDJE_MidiDeviceListHandleV1;
   typedef struct PDJE_InputSnapshotHandleV1 PDJE_InputSnapshotHandleV1;
   
   typedef enum PDJE_InputResultV1 {
       PDJE_INPUT_RESULT_OK_V1               = 0,
       PDJE_INPUT_RESULT_INVALID_ARGUMENT_V1 = 1,
       PDJE_INPUT_RESULT_OUT_OF_RANGE_V1     = 2,
       PDJE_INPUT_RESULT_INVALID_STATE_V1    = 3,
       PDJE_INPUT_RESULT_OPERATION_FAILED_V1 = 4,
       PDJE_INPUT_RESULT_INTERNAL_ERROR_V1   = 5
   } PDJE_InputResultV1;
   
   typedef enum PDJE_InputStateV1 {
       PDJE_INPUT_STATE_DEVICE_CONFIG_V1 = 0,
       PDJE_INPUT_STATE_LOOP_READY_V1    = 1,
       PDJE_INPUT_STATE_LOOP_RUNNING_V1  = 2,
       PDJE_INPUT_STATE_DEAD_V1          = 3
   } PDJE_InputStateV1;
   
   typedef enum PDJE_InputDeviceTypeV1 {
       PDJE_INPUT_DEVICE_MOUSE_V1    = 0,
       PDJE_INPUT_DEVICE_KEYBOARD_V1 = 1,
       PDJE_INPUT_DEVICE_UNKNOWN_V1  = 2
   } PDJE_InputDeviceTypeV1;
   
   typedef struct PDJE_InputStringViewV1 {
       const char *data;
       size_t      size;
   } PDJE_InputStringViewV1;
   
   typedef struct PDJE_InputBytesViewV1 {
       const uint8_t *data;
       size_t         size;
   } PDJE_InputBytesViewV1;
   
   typedef struct PDJE_InputDeviceViewV1 {
       uint32_t               struct_size;
       PDJE_InputDeviceTypeV1 type;
       PDJE_InputStringViewV1 name;
       PDJE_InputStringViewV1 device_specific_id;
   } PDJE_InputDeviceViewV1;
   
   typedef struct PDJE_MidiDeviceViewV1 {
       uint32_t               struct_size;
       PDJE_InputStringViewV1 manufacturer;
       PDJE_InputStringViewV1 device_name;
       PDJE_InputStringViewV1 port_name;
       PDJE_InputStringViewV1 display_name;
       uint64_t               client_handle;
       uint64_t               port_handle;
       uint8_t                port_type;
   } PDJE_MidiDeviceViewV1;
   
   typedef struct PDJE_InputKeyboardEventV1 {
       uint32_t key_code;
       int      pressed;
   } PDJE_InputKeyboardEventV1;
   
   typedef struct PDJE_InputMouseEventV1 {
       uint16_t button_type;
       int32_t  wheel_move;
       uint32_t axis_type;
       int32_t  x;
       int32_t  y;
   } PDJE_InputMouseEventV1;
   
   typedef struct PDJE_InputEventViewV1 {
       uint32_t                  struct_size;
       PDJE_InputDeviceTypeV1    type;
       PDJE_InputStringViewV1    id;
       PDJE_InputStringViewV1    name;
       uint64_t                  microsecond;
       PDJE_InputKeyboardEventV1 keyboard;
       PDJE_InputMouseEventV1    mouse;
       PDJE_InputBytesViewV1     hid_report;
   } PDJE_InputEventViewV1;
   
   typedef struct PDJE_MidiEventViewV1 {
       uint32_t               struct_size;
       uint8_t                type;
       uint8_t                channel;
       uint8_t                position;
       uint16_t               value;
       uint64_t               highres_time;
       PDJE_InputStringViewV1 port_name;
   } PDJE_MidiEventViewV1;
   
   typedef struct PDJE_InputSnapshotInfoV1 {
       uint32_t struct_size;
       int      has_input_stream;
       int      has_midi_stream;
       size_t   input_event_count;
       size_t   midi_event_count;
   } PDJE_InputSnapshotInfoV1;
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_create_v1(PDJE_InputHandleV1 **out_input);
   
   PDJE_API void PDJE_CALL
   pdje_input_destroy_v1(PDJE_InputHandleV1 *input);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_init_v1(PDJE_InputHandleV1 *input,
                      void               *platform_ctx0,
                      void               *platform_ctx1,
                      int                 use_internal_window);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_kill_v1(PDJE_InputHandleV1 *input);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_get_state_v1(const PDJE_InputHandleV1 *input,
                           PDJE_InputStateV1        *out_state);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_get_backend_name_v1(PDJE_InputHandleV1     *input,
                                  PDJE_InputStringViewV1 *out_backend);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_list_devices_v1(PDJE_InputHandleV1           *input,
                              PDJE_InputDeviceListHandleV1 **out_list);
   
   PDJE_API size_t PDJE_CALL
   pdje_input_device_list_size_v1(const PDJE_InputDeviceListHandleV1 *list);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_device_list_get_v1(const PDJE_InputDeviceListHandleV1 *list,
                                 size_t                              index,
                                 PDJE_InputDeviceViewV1            *out_device);
   
   PDJE_API void PDJE_CALL
   pdje_input_device_list_destroy_v1(PDJE_InputDeviceListHandleV1 *list);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_list_midi_devices_v1(PDJE_InputHandleV1          *input,
                                   PDJE_MidiDeviceListHandleV1 **out_list);
   
   PDJE_API size_t PDJE_CALL
   pdje_input_midi_device_list_size_v1(const PDJE_MidiDeviceListHandleV1 *list);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_midi_device_list_get_v1(const PDJE_MidiDeviceListHandleV1 *list,
                                      size_t                             index,
                                      PDJE_MidiDeviceViewV1            *out_device);
   
   PDJE_API void PDJE_CALL
   pdje_input_midi_device_list_destroy_v1(PDJE_MidiDeviceListHandleV1 *list);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_config_v1(PDJE_InputHandleV1                 *input,
                        const PDJE_InputDeviceListHandleV1 *devices,
                        const size_t                       *device_indices,
                        size_t                              device_index_count,
                        const PDJE_MidiDeviceListHandleV1  *midi_devices,
                        const size_t                       *midi_indices,
                        size_t                              midi_index_count);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_run_v1(PDJE_InputHandleV1 *input);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_poll_snapshot_v1(PDJE_InputHandleV1         *input,
                               PDJE_InputSnapshotHandleV1 **out_snapshot);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_describe_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                   PDJE_InputSnapshotInfoV1         *out_info);
   
   PDJE_API size_t PDJE_CALL
   pdje_input_snapshot_input_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_input_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                    size_t                            index,
                                    PDJE_InputEventViewV1           *out_event);
   
   PDJE_API size_t PDJE_CALL
   pdje_input_snapshot_midi_size_v1(const PDJE_InputSnapshotHandleV1 *snapshot);
   
   PDJE_API PDJE_InputResultV1 PDJE_CALL
   pdje_input_snapshot_midi_get_v1(const PDJE_InputSnapshotHandleV1 *snapshot,
                                   size_t                            index,
                                   PDJE_MidiEventViewV1            *out_event);
   
   PDJE_API void PDJE_CALL
   pdje_input_snapshot_destroy_v1(PDJE_InputSnapshotHandleV1 *snapshot);
   
   #ifdef __cplusplus
   }
   #endif
