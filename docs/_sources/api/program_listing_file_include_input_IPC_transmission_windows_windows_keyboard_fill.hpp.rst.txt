
.. _program_listing_file_include_input_IPC_transmission_windows_windows_keyboard_fill.hpp:

Program Listing for File windows_keyboard_fill.hpp
==================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_transmission_windows_windows_keyboard_fill.hpp>` (``include/input/IPC/transmission/windows/windows_keyboard_fill.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <WinUser.h>
   #include <Windows.h>
   
   namespace PDJE_RAWINPUT {
   static inline void
   FillMouseInput(PDJE_Input_Event &tempEv, const RAWINPUT *ri)
   {
       tempEv.mouse.button_type = ri->data.mouse.usButtonFlags;
       tempEv.mouse.wheel_move  = ri->data.mouse.usButtonData;
       if (ri->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
           if (ri->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) {
               tempEv.mouse.axis_type = PDJE_Mouse_Axis_Type::VIRTUAL_DESKTOP_ABS;
           } else {
               tempEv.mouse.axis_type = PDJE_Mouse_Axis_Type::ABS;
           }
       } else {
           tempEv.mouse.axis_type = PDJE_Mouse_Axis_Type::REL;
       }
       tempEv.mouse.x = ri->data.mouse.lLastX;
       tempEv.mouse.y = ri->data.mouse.lLastY;
   }
   
   static inline void
   FillKeyboardInput(PDJE_Input_Event &tempEv, const RAWINPUT *ri)
   {
       bool e0 = (ri->data.keyboard.Flags & RI_KEY_E0) != 0;
       auto k  = &tempEv.keyboard.k;
       switch (ri->data.keyboard.VKey) {
       case VK_F1:
           *k = PDJE_KEY::F_1;
           break;
       case VK_F2:
           *k = PDJE_KEY::F_2;
           break;
       case VK_F3:
           *k = PDJE_KEY::F_3;
           break;
       case VK_F4:
           *k = PDJE_KEY::F_4;
           break;
       case VK_F5:
           *k = PDJE_KEY::F_5;
           break;
       case VK_F6:
           *k = PDJE_KEY::F_6;
           break;
       case VK_F7:
           *k = PDJE_KEY::F_7;
           break;
       case VK_F8:
           *k = PDJE_KEY::F_8;
           break;
       case VK_F9:
           *k = PDJE_KEY::F_9;
           break;
       case VK_F10:
           *k = PDJE_KEY::F_10;
           break;
       case VK_F11:
           *k = PDJE_KEY::F_11;
           break;
       case VK_F12:
           *k = PDJE_KEY::F_12;
           break;
   
       case '0':
           *k = PDJE_KEY::D0;
           break;
       case '1':
           *k = PDJE_KEY::D1;
           break;
       case '2':
           *k = PDJE_KEY::D2;
           break;
       case '3':
           *k = PDJE_KEY::D3;
           break;
       case '4':
           *k = PDJE_KEY::D4;
           break;
       case '5':
           *k = PDJE_KEY::D5;
           break;
       case '6':
           *k = PDJE_KEY::D6;
           break;
       case '7':
           *k = PDJE_KEY::D7;
           break;
       case '8':
           *k = PDJE_KEY::D8;
           break;
       case '9':
           *k = PDJE_KEY::D9;
           break;
   
       case 'A':
           *k = PDJE_KEY::A;
           break;
       case 'B':
           *k = PDJE_KEY::B;
           break;
       case 'C':
           *k = PDJE_KEY::C;
           break;
       case 'D':
           *k = PDJE_KEY::D;
           break;
       case 'E':
           *k = PDJE_KEY::E;
           break;
       case 'F':
           *k = PDJE_KEY::F;
           break;
       case 'G':
           *k = PDJE_KEY::G;
           break;
       case 'H':
           *k = PDJE_KEY::H;
           break;
       case 'I':
           *k = PDJE_KEY::I;
           break;
       case 'J':
           *k = PDJE_KEY::J;
           break;
       case 'K':
           *k = PDJE_KEY::K;
           break;
       case 'L':
           *k = PDJE_KEY::L;
           break;
       case 'M':
           *k = PDJE_KEY::M;
           break;
       case 'N':
           *k = PDJE_KEY::N;
           break;
       case 'O':
           *k = PDJE_KEY::O;
           break;
       case 'P':
           *k = PDJE_KEY::P;
           break;
       case 'Q':
           *k = PDJE_KEY::Q;
           break;
       case 'R':
           *k = PDJE_KEY::R;
           break;
       case 'S':
           *k = PDJE_KEY::S;
           break;
       case 'T':
           *k = PDJE_KEY::T;
           break;
       case 'U':
           *k = PDJE_KEY::U;
           break;
       case 'V':
           *k = PDJE_KEY::V;
           break;
       case 'W':
           *k = PDJE_KEY::W;
           break;
       case 'X':
           *k = PDJE_KEY::X;
           break;
       case 'Y':
           *k = PDJE_KEY::Y;
           break;
       case 'Z':
           *k = PDJE_KEY::Z;
           break;
   
       case VK_OEM_MINUS:
           *k = PDJE_KEY::MINUS;
           break;
       case VK_OEM_PLUS:
           *k = PDJE_KEY::EQUAL;
           break;
       case VK_OEM_4:
           *k = PDJE_KEY::LBRACKET;
           break;
       case VK_OEM_6:
           *k = PDJE_KEY::RBRACKET;
           break;
       case VK_OEM_5:
           *k = PDJE_KEY::BACKSLASH;
           break;
       case VK_OEM_102:
           *k = PDJE_KEY::NONUS_BACKSLASH;
           break;
       case VK_OEM_1:
           *k = PDJE_KEY::SEMICOLON;
           break;
       case VK_OEM_7:
           *k = PDJE_KEY::APOSTROPHE;
           break;
       case VK_OEM_3:
           *k = PDJE_KEY::GRAVE;
           break;
       case VK_OEM_2:
           *k = PDJE_KEY::SLASH;
           break;
       case VK_OEM_COMMA:
           *k = PDJE_KEY::COMMA;
           break;
       case VK_OEM_PERIOD:
           *k = PDJE_KEY::PERIOD;
           break;
   
       case VK_RETURN:
           *k = e0 ? PDJE_KEY::KP_ENTER : PDJE_KEY::ENTER;
           break;
       case VK_ESCAPE:
           *k = PDJE_KEY::ESC;
           break;
       case VK_BACK:
           *k = PDJE_KEY::BACKSPACE;
           break;
       case VK_TAB:
           *k = PDJE_KEY::TAB;
           break;
       case VK_SPACE:
           *k = PDJE_KEY::SPACE;
           break;
       case VK_CAPITAL:
           *k = PDJE_KEY::CAPSLK;
           break;
   
       case VK_LEFT:
           *k = PDJE_KEY::LEFT;
           break;
       case VK_RIGHT:
           *k = PDJE_KEY::RIGHT;
           break;
       case VK_UP:
           *k = PDJE_KEY::UP;
           break;
       case VK_DOWN:
           *k = PDJE_KEY::DOWN;
           break;
   
       case VK_SHIFT:
           *k = (ri->data.keyboard.MakeCode == 0x36) ? PDJE_KEY::RSHIFT
                                                     : PDJE_KEY::LSHIFT;
           break;
       case VK_LSHIFT:
           *k = PDJE_KEY::LSHIFT;
           break;
       case VK_RSHIFT:
           *k = PDJE_KEY::RSHIFT;
           break;
   
       case VK_CONTROL:
           *k = e0 ? PDJE_KEY::RCTRL : PDJE_KEY::LCTRL;
           break;
       case VK_LCONTROL:
           *k = PDJE_KEY::LCTRL;
           break;
       case VK_RCONTROL:
           *k = PDJE_KEY::RCTRL;
           break;
       case VK_MENU:
           *k = e0 ? PDJE_KEY::RALT : PDJE_KEY::LALT;
           break;
       case VK_LMENU:
           *k = PDJE_KEY::LALT;
           break;
       case VK_RMENU:
           *k = PDJE_KEY::RALT;
           break;
   
       case VK_NUMPAD0:
           *k = PDJE_KEY::KP_0;
           break;
       case VK_NUMPAD1:
           *k = PDJE_KEY::KP_1;
           break;
       case VK_NUMPAD2:
           *k = PDJE_KEY::KP_2;
           break;
       case VK_NUMPAD3:
           *k = PDJE_KEY::KP_3;
           break;
       case VK_NUMPAD4:
           *k = PDJE_KEY::KP_4;
           break;
       case VK_NUMPAD5:
           *k = PDJE_KEY::KP_5;
           break;
       case VK_NUMPAD6:
           *k = PDJE_KEY::KP_6;
           break;
       case VK_NUMPAD7:
           *k = PDJE_KEY::KP_7;
           break;
       case VK_NUMPAD8:
           *k = PDJE_KEY::KP_8;
           break;
       case VK_NUMPAD9:
           *k = PDJE_KEY::KP_9;
           break;
   
       case VK_DECIMAL:
           *k = PDJE_KEY::KP_DOT;
           break;
       case VK_ADD:
           *k = PDJE_KEY::KP_PLUS;
           break;
       case VK_SUBTRACT:
           *k = PDJE_KEY::KP_MINUS;
           break;
       case VK_MULTIPLY:
           *k = PDJE_KEY::KP_STAR;
           break;
       case VK_DIVIDE:
           *k = PDJE_KEY::KP_SLASH;
           break;
       case VK_NUMLOCK:
           *k = PDJE_KEY::KP_NUMLOCK;
           break;
   
       case VK_SNAPSHOT:
           *k = PDJE_KEY::SP_PRINT_SCREEN;
           break;
       case VK_SCROLL:
           *k = PDJE_KEY::SP_SCROLL_LOCK;
           break;
   
       case VK_INSERT:
           *k = PDJE_KEY::SP_INSERT;
           break;
       case VK_DELETE:
           *k = PDJE_KEY::SP_DELETE;
           break;
       case VK_HOME:
           *k = PDJE_KEY::SP_HOME;
           break;
       case VK_END:
           *k = PDJE_KEY::SP_END;
           break;
       case VK_PRIOR:
           *k = PDJE_KEY::SP_PAGE_UP;
           break;
       case VK_NEXT:
           *k = PDJE_KEY::SP_PAGE_DOWN;
           break;
       default:
           *k = PDJE_KEY::UNKNOWN;
           break;
       }
   
       tempEv.keyboard.pressed = (ri->data.keyboard.Flags & RI_KEY_BREAK) == 0;
   }
   
   static
   void
   FillHIDInput(uint8_t buffer[],
                const RAWINPUT                         *ri,
                unsigned long                          &byteSize)
   {
   
       
       // hidB.resize(ri->data.hid.dwCount * ri->data.hid.dwSizeHid);
       auto sz = ri->data.hid.dwCount * ri->data.hid.dwSizeHid;
       if(sz > 512){
           return;
       }
       memcpy(buffer, ri->data.hid.bRawData, sz * sizeof(uint8_t));
       byteSize = ri->data.hid.dwSizeHid;
       return;
   }
   }; // namespace PDJE_RAWINPUT
