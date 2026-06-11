#include <doctest/doctest.h>

#include "CPDJE_Input.h"
#include "PDJE_CAbi_Input_Private.hpp"
#include "PDJE_Input_Log.hpp"

#include <cstdint>
#include <cstring>
#include <string>

namespace {

void
SetText(char (&target)[256], uint16_t &target_len, const char *value)
{
    const auto len = std::strlen(value);
    REQUIRE(len <= sizeof(target));
    std::memcpy(target, value, len);
    target_len = static_cast<uint16_t>(len);
}

std::string
ToString(const PDJE_InputStringViewV1 &value)
{
    if (value.data == nullptr || value.size == 0) {
        return {};
    }
    return std::string(value.data, value.size);
}

void
CheckMousePayloadZero(const PDJE_InputMouseEventV1 &mouse)
{
    CHECK(mouse.button_type == 0);
    CHECK(mouse.wheel_move == 0);
    CHECK(mouse.axis_type == 0);
    CHECK(mouse.x == 0);
    CHECK(mouse.y == 0);
}

void
CheckKeyboardPayloadZero(const PDJE_InputKeyboardEventV1 &keyboard)
{
    CHECK(keyboard.key_code == 0);
    CHECK(keyboard.pressed == 0);
}

} // namespace

TEST_CASE("CPDJE input C ABI snapshot event view reads only active union arm")
{
    SUBCASE("keyboard event leaves mouse payload zeroed")
    {
        PDJE_Input_Log log {};
        log.type                   = PDJE_Dev_Type::KEYBOARD;
        log.event.keyboard.k       = PDJE_KEY::A;
        log.event.keyboard.pressed = true;
        log.microSecond            = 1234;
        SetText(log.id, log.id_len, "keyboard-id");
        SetText(log.name, log.name_len, "keyboard-name");

        PDJE_InputEventViewV1 view {};
        view.struct_size = sizeof(view);
        PDJE_CABI::FillInputEventViewFromLog(log, &view);

        CHECK(view.struct_size == sizeof(view));
        CHECK(view.type == PDJE_INPUT_DEVICE_KEYBOARD_V1);
        CHECK(ToString(view.id) == "keyboard-id");
        CHECK(ToString(view.name) == "keyboard-name");
        CHECK(view.microsecond == 1234);
        CHECK(view.keyboard.key_code == static_cast<uint32_t>(PDJE_KEY::A));
        CHECK(view.keyboard.pressed == 1);
        CheckMousePayloadZero(view.mouse);
    }

    SUBCASE("mouse event leaves keyboard payload zeroed")
    {
        PDJE_Input_Log log {};
        log.type                    = PDJE_Dev_Type::MOUSE;
        log.event.mouse.button_type = PDJE_MOUSE_L_BTN_DOWN;
        log.event.mouse.wheel_move  = -2;
        log.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::REL;
        log.event.mouse.x           = 10;
        log.event.mouse.y           = -20;
        log.microSecond             = 5678;
        SetText(log.id, log.id_len, "mouse-id");
        SetText(log.name, log.name_len, "mouse-name");

        PDJE_InputEventViewV1 view {};
        view.struct_size = sizeof(view);
        PDJE_CABI::FillInputEventViewFromLog(log, &view);

        CHECK(view.struct_size == sizeof(view));
        CHECK(view.type == PDJE_INPUT_DEVICE_MOUSE_V1);
        CHECK(ToString(view.id) == "mouse-id");
        CHECK(ToString(view.name) == "mouse-name");
        CHECK(view.microsecond == 5678);
        CHECK(view.mouse.button_type == PDJE_MOUSE_L_BTN_DOWN);
        CHECK(view.mouse.wheel_move == -2);
        CHECK(view.mouse.axis_type ==
              static_cast<uint32_t>(PDJE_Mouse_Axis_Type::REL));
        CHECK(view.mouse.x == 10);
        CHECK(view.mouse.y == -20);
        CheckKeyboardPayloadZero(view.keyboard);
    }

    SUBCASE("unknown event leaves both payloads zeroed")
    {
        PDJE_Input_Log log {};
        log.type        = PDJE_Dev_Type::UNKNOWN;
        log.microSecond = 9999;
        SetText(log.id, log.id_len, "unknown-id");
        SetText(log.name, log.name_len, "unknown-name");

        PDJE_InputEventViewV1 view {};
        view.struct_size = sizeof(view);
        PDJE_CABI::FillInputEventViewFromLog(log, &view);

        CHECK(view.struct_size == sizeof(view));
        CHECK(view.type == PDJE_INPUT_DEVICE_UNKNOWN_V1);
        CHECK(ToString(view.id) == "unknown-id");
        CHECK(ToString(view.name) == "unknown-name");
        CHECK(view.microsecond == 9999);
        CheckKeyboardPayloadZero(view.keyboard);
        CheckMousePayloadZero(view.mouse);
    }
}
