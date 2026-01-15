#include "RTEvent.hpp"
#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstring>
#include <fcntl.h>
#include <libevdev/libevdev.h>
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
    // sched_param sp{};
    // sp.sched_priority = 70;
    // if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp) != 0) {
    //     std::cerr << "pthread_setschedparam failed: " << stderr << std::endl;
    //     return;
    // }
    epoll_event out_events[64];
    while (true) {
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
            // std::cout << "Read SUCCESS" << std::endl;
            PDJE_Input_Log ilog;
            ilog.type = id_to_type[FD];
            ilog.event.keyboard.k =
                PDJE_EVDEV_KEYMAP::keyboard_map(now_ev.code);
            bool writable = true;

            if (now_ev.value == 0) {
                ilog.event.keyboard.pressed = false;
            } else if (now_ev.value == 1) {
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
                std::memcpy(
                    ilog.name, id_to_name[FD].data(), id_to_name[FD].size());
                ilog.name_len = id_to_name[FD].size();
                out->Write(ilog);
                // std::cout << "Wrote" << std::endl;
            }
            // use_event(now_ev);
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
RTEvent::use_event(const input_event &evtrig)
{
    PDJE_Input_Log ilog;

    // ilog.out->Write(ilog);
    if (evtrig.type == EV_KEY && evtrig.code == KEY_A && evtrig.value == 1) {
        std::cout << "pressed";
    }
    return;
}
