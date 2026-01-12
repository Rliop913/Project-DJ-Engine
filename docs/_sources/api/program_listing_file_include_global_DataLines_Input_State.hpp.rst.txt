
.. _program_listing_file_include_global_DataLines_Input_State.hpp:

Program Listing for File Input_State.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_Input_State.hpp>` (``include\global\DataLines\Input_State.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <future>
   #include <memory_resource>
   #include <optional>
   #include <string>
   #include <vector>
   enum PDJE_INPUT_STATE {
       DEVICE_CONFIG_STATE = 0,
       INPUT_LOOP_READY,
       INPUT_LOOP_RUNNING,
       DEAD
   };
   
   enum PDJE_MIDI_EVENTS {
   
   };
   
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
   
   enum PDJE_KEY {
       F_1 = 0,
       F_2,
       F_3,
       F_4,
       F_5,
       F_6,
       F_7,
       F_8,
       F_9,
       F_10,
       F_11,
       F_12,
       D1,
       D2,
       D3,
       D4,
       D5,
       D6,
       D7,
       D8,
       D9,
       D0,
       Q,
       W,
       E,
       R,
       T,
       Y,
       U,
       I,
       O,
       P,
       A,
       S,
       D,
       F,
       G,
       H,
       J,
       K,
       L,
       Z,
       X,
       C,
       V,
       B,
       N,
       M,
   
       KP_1,
       KP_2,
       KP_3,
       KP_4,
       KP_5,
       KP_6,
       KP_7,
       KP_8,
       KP_9,
       KP_0,
       KP_DOT,
       KP_ENTER,
       KP_PLUS,
       KP_NUMLOCK,
       KP_SLASH,
       KP_STAR,
       KP_MINUS,
   
       ENTER,
       ESC,
       BACKSPACE,
       TAB,
       SPACE,
       CAPSLK,
   
       LEFT,
       RIGHT,
       UP,
       DOWN,
   
       LCTRL,
       RCTRL,
       LALT,
       RALT,
       LSHIFT,
       RSHIFT,
   
       MINUS,
       EQUAL,
       LBRACKET,
       RBRACKET,
       BACKSLASH,
       SLASH,
       SEMICOLON,
       APOSTROPHE,
       GRAVE,
       COMMA,
       PERIOD,
       NONUS_BACKSLASH,
   
       SP_PRINT_SCREEN,
       SP_SCROLL_LOCK,
       SP_INSERT,
       SP_HOME,
       SP_END,
       SP_DELETE,
       SP_PAGE_UP,
       SP_PAGE_DOWN,
       UNKNOWN
   };
   
   enum PDJE_Mouse_Axis_Type {
       REL                 = 0,
       ABS                 = 1,
       VIRTUAL_DESKTOP_ABS = 2 // maybe windows only
   };
   
   using BITMASK = uint16_t;
   struct PDJE_Mouse_Event {
       BITMASK              button_type;
       int                  wheel_move;
       PDJE_Mouse_Axis_Type axis_type;
       int                  x;
       int                  y;
   };
   
   struct PDJE_Keyboard_Event {
       PDJE_KEY k;
       bool     pressed;
   };
   
   struct PDJE_HID_Event {
       uint8_t       hid_buffer[512];
       unsigned long hid_byte_size = 0;
   };
   
   struct Midi_Input_Data {
       PDJE_MIDI_EVENTS event_type;
       uint8_t          channel;
       uint8_t          note;
       uint8_t          velocity;
   };
   
   union PDJE_Input_Event {
       PDJE_Mouse_Event    mouse;
       PDJE_Keyboard_Event keyboard;
   };
