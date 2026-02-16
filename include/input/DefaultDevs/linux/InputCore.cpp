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
InputCore::use_event(const input_event &evtrig, const int FD)
{
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
