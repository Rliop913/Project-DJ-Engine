#include "InputCore.hpp"
#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <linux/input-event-codes.h>
#include <string>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
bool
InputCore::Add(const fs::path &target, PDJE_Dev_Type type, std::string name)
{
    int FD = open(target.c_str(), O_RDONLY | O_NONBLOCK);
    if (FD < 0) {
        return false;
    }

    if (libevdev_new_from_fd(FD, &events[FD]) < 0) {
        close(FD);
        events.erase(FD);
        return false;
    }
    libevdev_set_clock_id(events[FD], CLOCK_MONOTONIC);
    id_to_type[FD] = type;
    id_to_name[FD] = name;
    return true;
}

InputCore::~InputCore()
{
    Reset();
}
void
InputCore::Reset()
{
    Stop();
    for (auto &target : events) {
        libevdev_free(target.second);
        close(target.first);
    }
    id_to_name.clear();
    id_to_type.clear();
    id_pressed.clear();
}
void
InputCore::Stop()
{
    if (switches.stop_event_fd >= 0) {
        uint64_t one = 1;
        write(switches.stop_event_fd, &one, sizeof(one));
    }
}

#include <iostream>
void
InputCore::Trig()
{
    int epfd               = epoll_create1(EPOLL_CLOEXEC);
    switches.stop_event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (switches.stop_event_fd < 0) {
        close(epfd);
        critlog("failed to init stop event fd on linux.");
        return;
    }

    { // add stop event fd
        epoll_event ev{};
        ev.events  = EPOLLIN;
        ev.data.fd = switches.stop_event_fd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, switches.stop_event_fd, &ev);
    }

    for (const auto &dev : events) { // add device event fd
        epoll_event ev{};
        ev.events  = EPOLLIN | EPOLLET;
        ev.data.fd = dev.first;
        epoll_ctl(epfd, EPOLL_CTL_ADD, dev.first, &ev);
    }

    // use rtkit here.
    // sched_param sp{};
    // sp.sched_priority = 70;
    // if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp) != 0) {
    //     std::cerr << "pthread_setschedparam failed: " << stderr <<
    //     std::endl; return;
    // }
    epoll_event out_events[64];
    int         cachedfd = 0;
    while (switches.loop_switch) {
        int n = epoll_wait(epfd, out_events, 64, -1);
        if (n < 0) {
            if (errno == EINTR) { // just signal
                continue;
            }
            break; // error
        }

        for (int i = 0; i < n; ++i) {
            cachedfd = out_events[i].data.fd;
            if (cachedfd == switches.stop_event_fd) {
                uint64_t v;
                while (read(switches.stop_event_fd, &v, sizeof(v)) ==
                       sizeof(v)) {
                }
                switches.loop_switch = false;
                break;
            }
            auto it = events.find(cachedfd);
            if (it != events.end()) {
                DrainEvents(epfd, cachedfd, it->second);
            }
        }
    }
    close(epfd);
    close(switches.stop_event_fd);
    switches.stop_event_fd = -1;
    return;
}
#include "evdev_codemap.hpp"

void
InputCore::DrainEvents(const int epFD, int FD, libevdev *evdev)
{
    input_event now_ev;
    auto        flag   = LIBEVDEV_READ_FLAG_NORMAL;
    bool        inSync = false;
    int         state  = 0;
    while (true) {
        flag  = inSync ? LIBEVDEV_READ_FLAG_SYNC : LIBEVDEV_READ_FLAG_NORMAL;
        state = libevdev_next_event(evdev, flag, &now_ev);

        switch (state) {
        case LIBEVDEV_READ_STATUS_SUCCESS: {

            use_event(now_ev, FD);
        }
            continue;

        case LIBEVDEV_READ_STATUS_SYNC:
            inSync = true;
            continue;

        case -EAGAIN:
            if (inSync) {
                inSync = false;
                continue;
            } else {
                return;
            }
        case -ENOMEM: {
            timespec sleepTime{ .tv_nsec = 300000 };
            nanosleep(&sleepTime, nullptr);
            continue;
        }

        case -ENODEV:
            [[fallthrough]];
        default:
            epoll_ctl(epFD, EPOLL_CTL_DEL, FD, nullptr);
            return;
        }
    }
}

void
InputCore::mouseRead(const input_event &evtrig, const int FD)
{
    PDJE_Input_Log ilog;
    ilog.type   = PDJE_Dev_Type::MOUSE;
    auto idstr  = std::to_string(FD);
    ilog.id_len = (idstr.size() > 255 ? 255 : idstr.size());
    std::memcpy(ilog.id, idstr.data(), ilog.id_len);
    ilog.microSecond        = clock.ConvertToMicroSecond(evtrig.time);
    const std::string &name = id_to_name[FD];
    ilog.name_len           = (name.size() > 255 ? 255 : name.size());
    std::memcpy(ilog.name, name.data(), ilog.name_len);
    switch (evtrig.type) {
    case EV_REL:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::REL;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        if (evtrig.code == REL_X) {
            ilog.event.mouse.x = evtrig.value;
            ilog.event.mouse.y = 0;
        } else {
            ilog.event.mouse.x = 0;
            ilog.event.mouse.y = evtrig.value;
        }
        break;
    case EV_ABS:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::ABS;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        if (evtrig.code == ABS_X) {
            ilog.event.mouse.x = evtrig.value;
            ilog.event.mouse.y = 0;
        } else {
            ilog.event.mouse.x = 0;
            ilog.event.mouse.y = evtrig.value;
        }
        break;
    case EV_KEY: {
        ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;

        ilog.event.mouse.wheel_move = 0;
        ilog.event.mouse.x          = 0;
        ilog.event.mouse.y          = 0;
        const bool down             = (evtrig.value != 0);
        switch (evtrig.code) {
        case BTN_LEFT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_L_BTN_DOWN : PDJE_MOUSE_L_BTN_UP;
            break;
        case BTN_RIGHT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_R_BTN_DOWN : PDJE_MOUSE_R_BTN_UP;
            break;
        case BTN_MIDDLE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_M_BTN_DOWN : PDJE_MOUSE_M_BTN_UP;
            break;
        case BTN_SIDE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_SIDE_BTN_DOWN
                     : PDJE_MOUSE_SIDE_BTN_UP; // XBUTTON1
            break;
        case BTN_EXTRA:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_EX_BTN_DOWN
                     : PDJE_MOUSE_EX_BTN_UP; // XBUTTON2
            break;
        default:
            ilog.event.mouse.button_type = 0;
            break;
        }
    } break;
    }
    out->Write(ilog);
}
void
InputCore::kbRead(const input_event &evtrig, const int FD)
{
    PDJE_Input_Log ilog;
    ilog.type             = PDJE_Dev_Type::KEYBOARD;
    ilog.event.keyboard.k = PDJE_EVDEV_KEYMAP::keyboard_map(evtrig.code);
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
        auto idstr = std::to_string(FD);
        std::memcpy(ilog.id, idstr.data(), idstr.size());
        ilog.id_len             = idstr.size();
        const std::string &name = id_to_name[FD];

        ilog.microSecond = clock.ConvertToMicroSecond(evtrig.time);
        std::memcpy(ilog.name, name.data(), name.size());
        ilog.name_len = name.size();
        out->Write(ilog);
    }
}

void
InputCore::use_event(const input_event &evtrig, const int FD)
{
    PDJE_Input_Log ilog;

    switch (id_to_type[FD]) {
    case PDJE_Dev_Type::KEYBOARD:
        kbRead(evtrig, FD);
        break;
    case PDJE_Dev_Type::MOUSE:
        mouseRead(evtrig, FD);
        break;
    default:
        break;
    }
}
