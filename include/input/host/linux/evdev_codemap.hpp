#pragma once

#include "Input_State.hpp"
#include <array>
#include <libevdev/libevdev.h>

namespace PDJE_EVDEV_KEYMAP {

static constexpr std::array<uint16_t, 102> kPdjeToEvdev = [] {
    std::array<uint16_t, 102> a{};
    for (auto &x : a)
        x = KEY_RESERVED;

    a[F_1]  = KEY_F1;
    a[F_2]  = KEY_F2;
    a[F_3]  = KEY_F3;
    a[F_4]  = KEY_F4;
    a[F_5]  = KEY_F5;
    a[F_6]  = KEY_F6;
    a[F_7]  = KEY_F7;
    a[F_8]  = KEY_F8;
    a[F_9]  = KEY_F9;
    a[F_10] = KEY_F10;
    a[F_11] = KEY_F11;
    a[F_12] = KEY_F12;

    a[D1] = KEY_1;
    a[D2] = KEY_2;
    a[D3] = KEY_3;
    a[D4] = KEY_4;
    a[D5] = KEY_5;
    a[D6] = KEY_6;
    a[D7] = KEY_7;
    a[D8] = KEY_8;
    a[D9] = KEY_9;
    a[D0] = KEY_0;

    a[Q] = KEY_Q;
    a[W] = KEY_W;
    a[E] = KEY_E;
    a[R] = KEY_R;
    a[T] = KEY_T;
    a[Y] = KEY_Y;
    a[U] = KEY_U;
    a[I] = KEY_I;
    a[O] = KEY_O;
    a[P] = KEY_P;
    a[A] = KEY_A;
    a[S] = KEY_S;
    a[D] = KEY_D;
    a[F] = KEY_F;
    a[G] = KEY_G;
    a[H] = KEY_H;
    a[J] = KEY_J;
    a[K] = KEY_K;
    a[L] = KEY_L;
    a[Z] = KEY_Z;
    a[X] = KEY_X;
    a[C] = KEY_C;
    a[V] = KEY_V;
    a[B] = KEY_B;
    a[N] = KEY_N;
    a[M] = KEY_M;

    a[KP_0]       = KEY_KP0;
    a[KP_1]       = KEY_KP1;
    a[KP_2]       = KEY_KP2;
    a[KP_3]       = KEY_KP3;
    a[KP_4]       = KEY_KP4;
    a[KP_5]       = KEY_KP5;
    a[KP_6]       = KEY_KP6;
    a[KP_7]       = KEY_KP7;
    a[KP_8]       = KEY_KP8;
    a[KP_9]       = KEY_KP9;
    a[KP_DOT]     = KEY_KPDOT;
    a[KP_ENTER]   = KEY_KPENTER;
    a[KP_PLUS]    = KEY_KPPLUS;
    a[KP_NUMLOCK] = KEY_NUMLOCK;
    a[KP_SLASH]   = KEY_KPSLASH;
    a[KP_STAR]    = KEY_KPASTERISK;
    a[KP_MINUS]   = KEY_KPMINUS;

    a[ENTER]     = KEY_ENTER;
    a[ESC]       = KEY_ESC;
    a[BACKSPACE] = KEY_BACKSPACE;
    a[TAB]       = KEY_TAB;
    a[SPACE]     = KEY_SPACE;
    a[CAPSLK]    = KEY_CAPSLOCK;

    a[LEFT]  = KEY_LEFT;
    a[RIGHT] = KEY_RIGHT;
    a[UP]    = KEY_UP;
    a[DOWN]  = KEY_DOWN;

    a[LCTRL]  = KEY_LEFTCTRL;
    a[RCTRL]  = KEY_RIGHTCTRL;
    a[LALT]   = KEY_LEFTALT;
    a[RALT]   = KEY_RIGHTALT;
    a[LSHIFT] = KEY_LEFTSHIFT;
    a[RSHIFT] = KEY_RIGHTSHIFT;

    a[MINUS]      = KEY_MINUS;
    a[EQUAL]      = KEY_EQUAL;
    a[LBRACKET]   = KEY_LEFTBRACE;
    a[RBRACKET]   = KEY_RIGHTBRACE;
    a[BACKSLASH]  = KEY_BACKSLASH;
    a[SLASH]      = KEY_SLASH;
    a[SEMICOLON]  = KEY_SEMICOLON;
    a[APOSTROPHE] = KEY_APOSTROPHE;
    a[GRAVE]      = KEY_GRAVE;
    a[COMMA]      = KEY_COMMA;
    a[PERIOD]     = KEY_DOT;

    a[NONUS_BACKSLASH] = KEY_102ND;

    a[SP_PRINT_SCREEN] = KEY_SYSRQ;
    a[SP_SCROLL_LOCK]  = KEY_SCROLLLOCK;
    a[SP_INSERT]       = KEY_INSERT;
    a[SP_HOME]         = KEY_HOME;
    a[SP_END]          = KEY_END;
    a[SP_DELETE]       = KEY_DELETE;
    a[SP_PAGE_UP]      = KEY_PAGEUP;
    a[SP_PAGE_DOWN]    = KEY_PAGEDOWN;

    a[UNKNOWN] = KEY_RESERVED;

    return a;
}();

static constexpr std::array<PDJE_KEY, KEY_MAX + 1> kEvdevToPdje = [] {
    std::array<PDJE_KEY, KEY_MAX + 1> a{};
    for (auto &x : a)
        x = UNKNOWN;

    for (uint16_t pk = 0; pk < 102; ++pk) {
        auto ev = kPdjeToEvdev[pk];
        if (ev != KEY_RESERVED && ev <= KEY_MAX)
            a[ev] = static_cast<PDJE_KEY>(pk);
    }
    return a;
}();

static inline PDJE_KEY
keyboard_map(__u16 code)
{
    if (code > KEY_MAX)
        return UNKNOWN;
    return kEvdevToPdje[code];
}

} // namespace PDJE_EVDEV_KEYMAP