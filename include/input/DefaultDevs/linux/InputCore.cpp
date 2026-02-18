#include "InputCore.hpp"
#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "rtkitcodes.hpp"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <exception>
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

    libevdev *dev = nullptr;
    if (libevdev_new_from_fd(FD, &dev) < 0) {
        close(FD);
        return false;
    }

    if (libevdev_set_clock_id(dev, CLOCK_MONOTONIC) < 0) {
        warnlog("failed to set CLOCK_MONOTONIC for an input device on linux.");
    }
    events[FD]     = dev;
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
        if (target.second) {
            libevdev_free(target.second);
        }
        close(target.first);
    }
    events.clear();
    id_to_name.clear();
    id_to_type.clear();
    id_pressed.clear();
}
void
InputCore::Stop()
{
    if (switches.stop_event_fd >= 0) {
        uint64_t one = 1;
        while (true) {
            const ssize_t wr =
                write(switches.stop_event_fd, &one, sizeof(one));
            if (wr == static_cast<ssize_t>(sizeof(one))) {
                return;
            }
            if (wr < 0 && errno == EINTR) {
                continue;
            }
            if (wr < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                return;
            }
            critlog("failed to signal input loop stop event on linux.");
            return;
        }
    }
}

void
InputCore::Trig()
{
    switches.loop_switch   = true;
    int epfd               = epoll_create1(EPOLL_CLOEXEC);
    if (epfd < 0) {
        critlog("failed to create epoll fd on linux.");
        return;
    }
    switches.stop_event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (switches.stop_event_fd < 0) {
        critlog("failed to init stop event fd on linux.");
        close(epfd);
        return;
    }

    { // add stop event fd
        epoll_event ev{};
        ev.events  = EPOLLIN;
        ev.data.fd = switches.stop_event_fd;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, switches.stop_event_fd, &ev) < 0) {
            critlog("failed to register stop event fd on linux.");
            close(switches.stop_event_fd);
            switches.stop_event_fd = -1;
            close(epfd);
            return;
        }
    }

    for (auto it = events.begin(); it != events.end();) { // add device event fd
        epoll_event ev{};
        ev.events  = EPOLLIN | EPOLLET;
        ev.data.fd = it->first;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, it->first, &ev) < 0) {
            warnlog("failed to register an input device fd to epoll. removing "
                    "this device.");
            const int failed_fd = it->first;
            ++it;
            RemoveDeviceFD(epfd, failed_fd);
            continue;
        }
        ++it;
    }

    try {
        rtkit::make_current_thread_realtime(70);
    } catch (const std::exception &e) {
        warnlog("rtkit realtime promotion failed. continuing with normal "
                "scheduler.");
        warnlog(e.what());
    }

    epoll_event out_events[64];
    int         cachedfd = 0;
    while (switches.loop_switch) {
        int n = epoll_wait(epfd, out_events, 64, -1);
        if (n < 0) {
            if (errno == EINTR) { // just signal
                continue;
            }
            critlog("epoll_wait failed in linux input loop.");
            break; // error
        }

        for (int i = 0; i < n; ++i) {
            cachedfd = out_events[i].data.fd;
            if (cachedfd == switches.stop_event_fd) {
                uint64_t v;
                while (true) {
                    const ssize_t rd = read(switches.stop_event_fd, &v, sizeof(v));
                    if (rd == static_cast<ssize_t>(sizeof(v))) {
                        continue;
                    }
                    if (rd < 0 && errno == EINTR) {
                        continue;
                    }
                    break;
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
    if (switches.stop_event_fd >= 0) {
        close(switches.stop_event_fd);
        switches.stop_event_fd = -1;
    }
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
            RemoveDeviceFD(epFD, FD);
            return;
        }
    }
}

void
InputCore::use_event(const input_event &evtrig, const int FD)
{
    auto type_it = id_to_type.find(FD);
    if (type_it == id_to_type.end()) {
        return;
    }

    switch (type_it->second) {
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

void
InputCore::RemoveDeviceFD(int epFD, int FD)
{
    if (epFD >= 0 && epoll_ctl(epFD, EPOLL_CTL_DEL, FD, nullptr) < 0) {
        if (errno != ENOENT && errno != EBADF) {
            warnlog("failed to remove fd from epoll while cleaning up input "
                    "device.");
        }
    }

    auto dev_it = events.find(FD);
    if (dev_it != events.end()) {
        if (dev_it->second) {
            libevdev_free(dev_it->second);
        }
        close(FD);
        events.erase(dev_it);
    }
    id_to_type.erase(FD);
    id_to_name.erase(FD);
    id_pressed.erase(FD);
}
