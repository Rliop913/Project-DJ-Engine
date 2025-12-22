#include "RTEvent.hpp"
#include "RTSocket.hpp"
#include <libevdev/libevdev.h>

int
RTEvent::Add(const fs::path &target)
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
    sched_param sp{};
    sp.sched_priority = 70;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp) != 0) {
        std::cerr << "pthread_setschedparam failed: " << stderr << std::endl;
        return;
    }
    int         trigCount = 40;
    epoll_event out_events[64];
    while (true) {
        int n = epoll_wait(epfd, out_events, 64, 500);
        std::cout << "trigged" << n << std::endl;

        for (int i = 0; i < n; ++i) {
            DrainEvents(
                epfd, out_events[i].data.fd, events[out_events[i].data.fd]);
        }
        trigCount--;
        if (trigCount < 1) {
            return;
        }
    }
    return;
}

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
        case LIBEVDEV_READ_STATUS_SUCCESS:
            use_event(now_ev);
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
    if (evtrig.type == EV_KEY && evtrig.code == KEY_A && evtrig.value == 1) {
        std::cout << "pressed";
    }
    return;
}
