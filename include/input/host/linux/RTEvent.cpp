#include "RTEvent.hpp"
#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstring>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <linux/input-event-codes.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
int
RTEvent::Add(const fs::path &target, PDJE_Dev_Type type, std::string name)
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
    libevdev_set_clock_id(events[FD], CLOCK_MONOTONIC_RAW);
    id_to_type[FD] = type;
    id_to_name[FD] = name;
    return true;
}

RTEvent::~RTEvent()
{
    Reset();
}
void
RTEvent::Reset()
{
    for (auto &target : events) {
        libevdev_free(target.second);
        close(target.first);
    }
}
#include <iostream>
void
RTEvent::Trig()
{
    int epfd = epoll_create1(EPOLL_CLOEXEC);

    for (const auto &dev : events) {
        epoll_event ev;
        ev.events  = EPOLLIN | EPOLLET;
        ev.data.fd = dev.first;
        epoll_ctl(epfd, EPOLL_CTL_ADD, dev.first, &ev);
    }
    // use rtkit here.
    // sched_param sp{};
    // sp.sched_priority = 70;
    // if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp) != 0) {
    //     std::cerr << "pthread_setschedparam failed: " << stderr << std::endl;
    //     return;
    // }
    epoll_event out_events[64];
    while (loop_switch) {
        int n = epoll_wait(epfd, out_events, 64, 500);

        for (int i = 0; i < n; ++i) {
            DrainEvents(
                epfd, out_events[i].data.fd, events[out_events[i].data.fd]);
        }
    }
    return;
}
#include "evdev_codemap.hpp"

void
RTEvent::DrainEvents(const int epFD, int FD, libevdev *evdev)
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
RTEvent::mouseRead(const input_event &evtrig, const int FD)
{
    PDJE_Input_Log ilog;
    ilog.type  = PDJE_Dev_Type::MOUSE;
    auto idstr = std::to_string(FD);
    std::memcpy(ilog.id, idstr.data(), idstr.size());
    ilog.id_len      = idstr.size();
    ilog.microSecond = clock.Get_MicroSecond();
    std::memcpy(ilog.name, id_to_name[FD].data(), id_to_name[FD].size());
    ilog.name_len = id_to_name[FD].size();
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
        ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::IGNORE;

        ilog.event.mouse.wheel_move = 0;
        ilog.event.mouse.x          = 0;
        ilog.event.mouse.y          = 0;
        const bool down             = (evtrig.value != 0);
        switch (evtrig.code) {
        case BTN_LEFT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_L_BTN_DOWN : PDJE_MOUSE_L_BTN_UP;
        case BTN_RIGHT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_R_BTN_DOWN : PDJE_MOUSE_R_BTN_UP;
        case BTN_MIDDLE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_M_BTN_DOWN : PDJE_MOUSE_M_BTN_UP;
        case BTN_SIDE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_SIDE_BTN_DOWN
                     : PDJE_MOUSE_SIDE_BTN_UP; // XBUTTON1
        case BTN_EXTRA:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_EX_BTN_DOWN
                     : PDJE_MOUSE_EX_BTN_UP; // XBUTTON2
        default:
            ilog.event.mouse.button_type = 0;
        }
    } break;
    }
    out->Write(ilog);
}
void
RTEvent::kbRead(const input_event &evtrig, const int FD)
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
        ilog.id_len      = idstr.size();
        ilog.microSecond = clock.Get_MicroSecond();
        std::memcpy(ilog.name, id_to_name[FD].data(), id_to_name[FD].size());
        ilog.name_len = id_to_name[FD].size();
        out->Write(ilog);
    }
}

void
RTEvent::use_event(const input_event &evtrig, const int FD)
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
