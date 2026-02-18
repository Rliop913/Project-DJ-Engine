#include "InputCore.hpp"
#include "evdev_codemap.hpp"
#include <algorithm>
#include <cstring>

void
InputCore::kbRead(const input_event &evtrig, const int FD)
{
    if (evtrig.type != EV_KEY) {
        return;
    }

    PDJE_Input_Log ilog{};
    ilog.type             = PDJE_Dev_Type::KEYBOARD;
    ilog.event.keyboard.k = PDJE_EVDEV_KEYMAP::keyboard_map(evtrig.code);
    if (ilog.event.keyboard.k == PDJE_KEY::UNKNOWN) {
        return;
    }

    bool writable         = true;

    if (evtrig.value == 0) {
        ilog.event.keyboard.pressed = false;
    } else if (evtrig.value == 1) {
        ilog.event.keyboard.pressed = true;
    } else {
        writable = false;
    }
    if (writable) {
        if (id_pressed[FD].test(ilog.event.keyboard.k) ==
            ilog.event.keyboard.pressed) {
            writable = false;
        } else {
            id_pressed[FD].set(ilog.event.keyboard.k,
                               ilog.event.keyboard.pressed);
        }
    }
    if (writable) {
        auto              idstr = std::to_string(FD);
        const std::size_t id_len =
            std::min(idstr.size(), sizeof(ilog.id));
        std::memcpy(ilog.id, idstr.data(), id_len);
        ilog.id_len = static_cast<uint16_t>(id_len);

        auto name_it = id_to_name.find(FD);
        if (name_it == id_to_name.end()) {
            return;
        }
        const std::string &name = name_it->second;
        const std::size_t  name_len =
            std::min(name.size(), sizeof(ilog.name));

        ilog.microSecond = clock.ConvertToMicroSecond(evtrig.time);
        std::memcpy(ilog.name, name.data(), name_len);
        ilog.name_len = static_cast<uint16_t>(name_len);
        out->Write(ilog);
    }
}
