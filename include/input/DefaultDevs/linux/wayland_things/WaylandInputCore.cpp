#include "WaylandInputCore.hpp"
#include "LinuxMouseButtonMapping.hpp"
#include "PDJE_Input_Log.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "evdev_codemap.hpp"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <linux/input-event-codes.h>
#include <poll.h>
#include <unistd.h>
#include <utility>
#include <wayland-client-protocol.h>

namespace {

template <typename T>
void
CopyFixedString(char (&dst)[256], uint16_t &dst_len, const T &src)
{
    const std::size_t n = std::min<std::size_t>(src.size(), sizeof(dst));
    std::memset(dst, 0, sizeof(dst));
    std::memcpy(dst, src.data(), n);
    dst_len = static_cast<uint16_t>(n);
}

inline uint64_t
ToMicroSeconds(uint32_t wayland_time_ms) noexcept
{
    return static_cast<uint64_t>(wayland_time_ms) * 1000ULL;
}

inline int
WheelDeltaFromFixed(wl_fixed_t value) noexcept
{
    int v = wl_fixed_to_int(value);
    if (v != 0) {
        return v;
    }
    if (value > 0) {
        return 1;
    }
    if (value < 0) {
        return -1;
    }
    return 0;
}

void
ReleaseKeyboardCompat(wl_keyboard *&keyboard) noexcept
{
    if (keyboard == nullptr) {
        return;
    }
    if (wl_keyboard_get_version(keyboard) >= WL_KEYBOARD_RELEASE_SINCE_VERSION) {
        wl_keyboard_release(keyboard);
    } else {
        wl_keyboard_destroy(keyboard);
    }
    keyboard = nullptr;
}

void
ReleasePointerCompat(wl_pointer *&pointer) noexcept
{
    if (pointer == nullptr) {
        return;
    }
    if (wl_pointer_get_version(pointer) >= WL_POINTER_RELEASE_SINCE_VERSION) {
        wl_pointer_release(pointer);
    } else {
        wl_pointer_destroy(pointer);
    }
    pointer = nullptr;
}

void
ReleaseSeatCompat(wl_seat *&seat) noexcept
{
    if (seat == nullptr) {
        return;
    }
    if (wl_seat_get_version(seat) >= WL_SEAT_RELEASE_SINCE_VERSION) {
        wl_seat_release(seat);
    } else {
        wl_seat_destroy(seat);
    }
    seat = nullptr;
}

} // namespace

void
WaylandInputCore::SetError(std::string msg)
{
    last_error = std::move(msg);
}

void
WaylandInputCore::ClearSelectionState()
{
    wants_keyboard   = false;
    wants_pointer    = false;
    keyboard_focused = false;
    pointer_focused  = false;
    keyboard_name.clear();
    pointer_name.clear();
    key_pressed.reset();
}

void
WaylandInputCore::WriteKeyboardEvent(uint32_t time_ms, uint32_t key, bool pressed)
{
    if (out == nullptr) {
        return;
    }

    const PDJE_KEY mapped = PDJE_EVDEV_KEYMAP::keyboard_map(key);
    if (mapped == PDJE_KEY::UNKNOWN) {
        return;
    }

    const auto idx = static_cast<std::size_t>(mapped);
    if (idx < key_pressed.size()) {
        if (key_pressed.test(idx) == pressed) {
            return;
        }
        key_pressed.set(idx, pressed);
    }

    PDJE_Input_Log ilog{};
    ilog.type                    = PDJE_Dev_Type::KEYBOARD;
    ilog.event.keyboard.k        = mapped;
    ilog.event.keyboard.pressed  = pressed;
    ilog.microSecond             = ToMicroSeconds(time_ms);
    const std::string id         = "wayland://keyboard";
    const std::string &name_ref  = keyboard_name.empty()
                                      ? std::string("Wayland Keyboard")
                                      : keyboard_name;

    CopyFixedString(ilog.id, ilog.id_len, id);
    CopyFixedString(ilog.name, ilog.name_len, name_ref);
    out->Write(ilog);
}

void
WaylandInputCore::WritePointerMotion(uint32_t time_ms, int x, int y)
{
    if (out == nullptr) {
        return;
    }

    PDJE_Input_Log ilog{};
    ilog.type                    = PDJE_Dev_Type::MOUSE;
    ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::ABS;
    ilog.event.mouse.button_type = 0;
    ilog.event.mouse.wheel_move  = 0;
    ilog.event.mouse.x           = x;
    ilog.event.mouse.y           = y;
    ilog.microSecond             = ToMicroSeconds(time_ms);
    const std::string id         = "wayland://pointer";
    const std::string &name_ref  = pointer_name.empty()
                                      ? std::string("Wayland Pointer")
                                      : pointer_name;

    CopyFixedString(ilog.id, ilog.id_len, id);
    CopyFixedString(ilog.name, ilog.name_len, name_ref);
    out->Write(ilog);
}

void
WaylandInputCore::WritePointerButton(uint32_t time_ms,
                                     uint32_t button,
                                     bool     pressed)
{
    if (out == nullptr) {
        return;
    }

    PDJE_Input_Log ilog{};
    ilog.type                  = PDJE_Dev_Type::MOUSE;
    ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;
    ilog.event.mouse.wheel_move = 0;
    ilog.event.mouse.x          = 0;
    ilog.event.mouse.y          = 0;
    ilog.microSecond            = ToMicroSeconds(time_ms);

    const auto mapped =
        PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP::TryMapLinuxMouseButton(button,
                                                                      pressed);
    if (!mapped.has_value()) {
        return;
    }
    ilog.event.mouse.button_type = *mapped;

    const std::string id         = "wayland://pointer";
    const std::string &name_ref  = pointer_name.empty()
                                      ? std::string("Wayland Pointer")
                                      : pointer_name;
    CopyFixedString(ilog.id, ilog.id_len, id);
    CopyFixedString(ilog.name, ilog.name_len, name_ref);
    out->Write(ilog);
}

void
WaylandInputCore::WritePointerAxis(uint32_t time_ms, uint32_t axis, int wheel_delta)
{
    if (out == nullptr || wheel_delta == 0) {
        return;
    }

    PDJE_Input_Log ilog{};
    ilog.type                    = PDJE_Dev_Type::MOUSE;
    ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;
    ilog.event.mouse.button_type = 0;
    ilog.event.mouse.wheel_move  = wheel_delta;
    ilog.event.mouse.x           = 0;
    ilog.event.mouse.y           = 0;
    ilog.microSecond             = ToMicroSeconds(time_ms);

    if (axis != WL_POINTER_AXIS_VERTICAL_SCROLL &&
        axis != WL_POINTER_AXIS_HORIZONTAL_SCROLL) {
        return;
    }

    const std::string id         = "wayland://pointer";
    const std::string &name_ref  = pointer_name.empty()
                                      ? std::string("Wayland Pointer")
                                      : pointer_name;
    CopyFixedString(ilog.id, ilog.id_len, id);
    CopyFixedString(ilog.name, ilog.name_len, name_ref);
    out->Write(ilog);
}

void
WaylandInputCore::OnHostRegistryGlobal(void       *data,
                                       wl_registry *registry,
                                       uint32_t     name,
                                       const char  *interface,
                                       uint32_t     version)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || interface == nullptr) {
        return;
    }
    if (std::strcmp(interface, wl_seat_interface.name) != 0) {
        return;
    }
    if (self->seat != nullptr) {
        return;
    }
    const uint32_t bind_version = std::min<uint32_t>(version, 5);
    self->seat = static_cast<wl_seat *>(
        wl_registry_bind(registry, name, &wl_seat_interface, bind_version));
}

void
WaylandInputCore::OnHostRegistryGlobalRemove(void *, wl_registry *, uint32_t)
{
}

void
WaylandInputCore::AttachSeatListeners()
{
    if (seat == nullptr) {
        return;
    }

    static constexpr wl_seat_listener kSeatListener = {
        .capabilities = &WaylandInputCore::OnSeatCapabilities,
        .name         = &WaylandInputCore::OnSeatName,
    };
    wl_seat_add_listener(seat, &kSeatListener, this);
}

void
WaylandInputCore::OnSeatCapabilities(void     *data,
                                     wl_seat  *seat,
                                     uint32_t  capabilities)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || seat == nullptr) {
        return;
    }

    if (self->wants_keyboard) {
        if ((capabilities & WL_SEAT_CAPABILITY_KEYBOARD) != 0) {
            if (self->keyboard == nullptr) {
                self->keyboard = wl_seat_get_keyboard(seat);
                if (self->keyboard != nullptr) {
                    static constexpr wl_keyboard_listener kKeyboardListener = {
                        .keymap     = &WaylandInputCore::OnKeyboardKeymap,
                        .enter      = &WaylandInputCore::OnKeyboardEnter,
                        .leave      = &WaylandInputCore::OnKeyboardLeave,
                        .key        = &WaylandInputCore::OnKeyboardKey,
                        .modifiers  = &WaylandInputCore::OnKeyboardModifiers,
                        .repeat_info = &WaylandInputCore::OnKeyboardRepeatInfo,
                    };
                    wl_keyboard_add_listener(self->keyboard,
                                             &kKeyboardListener,
                                             self);
                }
            }
        } else {
            ReleaseKeyboardCompat(self->keyboard);
            self->keyboard_focused = false;
            self->key_pressed.reset();
        }
    }

    if (self->wants_pointer) {
        if ((capabilities & WL_SEAT_CAPABILITY_POINTER) != 0) {
            if (self->pointer == nullptr) {
                self->pointer = wl_seat_get_pointer(seat);
                if (self->pointer != nullptr) {
                    static constexpr wl_pointer_listener kPointerListener = {
                        .enter = &WaylandInputCore::OnPointerEnter,
                        .leave = &WaylandInputCore::OnPointerLeave,
                        .motion = &WaylandInputCore::OnPointerMotion,
                        .button = &WaylandInputCore::OnPointerButton,
                        .axis = &WaylandInputCore::OnPointerAxis,
                        .frame = &WaylandInputCore::OnPointerFrame,
                        .axis_source = &WaylandInputCore::OnPointerAxisSource,
                        .axis_stop = &WaylandInputCore::OnPointerAxisStop,
                        .axis_discrete = &WaylandInputCore::OnPointerAxisDiscrete,
                        .axis_value120 = &WaylandInputCore::OnPointerAxisValue120,
                        .axis_relative_direction =
                            &WaylandInputCore::OnPointerAxisRelativeDirection,
                    };
                    wl_pointer_add_listener(self->pointer, &kPointerListener, self);
                }
            }
        } else {
            ReleasePointerCompat(self->pointer);
            self->pointer_focused = false;
        }
    }
}

void
WaylandInputCore::OnSeatName(void *, wl_seat *, const char *)
{
}

void
WaylandInputCore::OnKeyboardKeymap(void     *,
                                   wl_keyboard *,
                                   uint32_t,
                                   int32_t  fd,
                                   uint32_t)
{
    if (fd >= 0) {
        close(fd);
    }
}

void
WaylandInputCore::OnKeyboardEnter(void      *data,
                                  wl_keyboard *,
                                  uint32_t,
                                  wl_surface *surface,
                                  wl_array *)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr) {
        return;
    }
    self->keyboard_focused = (surface == self->target_surface);
    self->key_pressed.reset();
}

void
WaylandInputCore::OnKeyboardLeave(void      *data,
                                  wl_keyboard *,
                                  uint32_t,
                                  wl_surface *surface)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr) {
        return;
    }
    if (surface == self->target_surface) {
        self->keyboard_focused = false;
        self->key_pressed.reset();
    }
}

void
WaylandInputCore::OnKeyboardKey(void      *data,
                                wl_keyboard *,
                                uint32_t,
                                uint32_t time,
                                uint32_t key,
                                uint32_t state)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || !self->keyboard_focused) {
        return;
    }
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
        self->WriteKeyboardEvent(time, key, true);
    } else if (state == WL_KEYBOARD_KEY_STATE_RELEASED) {
        self->WriteKeyboardEvent(time, key, false);
    }
}

void
WaylandInputCore::OnKeyboardModifiers(void *,
                                      wl_keyboard *,
                                      uint32_t,
                                      uint32_t,
                                      uint32_t,
                                      uint32_t,
                                      uint32_t)
{
}

void
WaylandInputCore::OnKeyboardRepeatInfo(void *, wl_keyboard *, int32_t, int32_t)
{
}

void
WaylandInputCore::OnPointerEnter(void      *data,
                                 wl_pointer *,
                                 uint32_t,
                                 wl_surface *surface,
                                 wl_fixed_t,
                                 wl_fixed_t)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr) {
        return;
    }
    self->pointer_focused = (surface == self->target_surface);
}

void
WaylandInputCore::OnPointerLeave(void      *data,
                                 wl_pointer *,
                                 uint32_t,
                                 wl_surface *surface)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr) {
        return;
    }
    if (surface == self->target_surface) {
        self->pointer_focused = false;
    }
}

void
WaylandInputCore::OnPointerMotion(void     *data,
                                  wl_pointer *,
                                  uint32_t time,
                                  wl_fixed_t surface_x,
                                  wl_fixed_t surface_y)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || !self->pointer_focused) {
        return;
    }
    self->WritePointerMotion(time,
                             wl_fixed_to_int(surface_x),
                             wl_fixed_to_int(surface_y));
}

void
WaylandInputCore::OnPointerButton(void      *data,
                                  wl_pointer *,
                                  uint32_t,
                                  uint32_t time,
                                  uint32_t button,
                                  uint32_t state)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || !self->pointer_focused) {
        return;
    }
    if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
        self->WritePointerButton(time, button, true);
    } else if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
        self->WritePointerButton(time, button, false);
    }
}

void
WaylandInputCore::OnPointerAxis(void      *data,
                                wl_pointer *,
                                uint32_t time,
                                uint32_t axis,
                                wl_fixed_t value)
{
    auto *self = static_cast<WaylandInputCore *>(data);
    if (self == nullptr || !self->pointer_focused) {
        return;
    }
    self->WritePointerAxis(time, axis, WheelDeltaFromFixed(value));
}

void
WaylandInputCore::OnPointerFrame(void *, wl_pointer *)
{
}

void
WaylandInputCore::OnPointerAxisSource(void *, wl_pointer *, uint32_t)
{
}

void
WaylandInputCore::OnPointerAxisStop(void *, wl_pointer *, uint32_t, uint32_t)
{
}

void
WaylandInputCore::OnPointerAxisDiscrete(void *, wl_pointer *, uint32_t, int32_t)
{
}

void
WaylandInputCore::OnPointerAxisValue120(void *, wl_pointer *, uint32_t, int32_t)
{
}

void
WaylandInputCore::OnPointerAxisRelativeDirection(void *,
                                                 wl_pointer *,
                                                 uint32_t,
                                                 uint32_t)
{
}

bool
WaylandInputCore::ConfigureHostHandles(void *wl_display_ptr, void *wl_surface_ptr)
{
    display        = static_cast<wl_display *>(wl_display_ptr);
    target_surface = static_cast<wl_surface *>(wl_surface_ptr);
    source_mode    = SourceMode::HostHandles;

    if (display == nullptr || target_surface == nullptr) {
        SetError("null host context handle");
        return false;
    }

    host_registry = wl_display_get_registry(display);
    if (host_registry == nullptr) {
        SetError("wl_display_get_registry failed for host handles");
        return false;
    }

    static constexpr wl_registry_listener kRegistryListener = {
        .global        = &WaylandInputCore::OnHostRegistryGlobal,
        .global_remove = &WaylandInputCore::OnHostRegistryGlobalRemove,
    };
    wl_registry_add_listener(host_registry, &kRegistryListener, this);

    if (wl_display_roundtrip(display) < 0) {
        SetError("wl_display_roundtrip failed while discovering host seat");
        return false;
    }
    if (seat == nullptr) {
        SetError("wl_seat not found on host Wayland display");
        return false;
    }

    if (!warned_host_dispatch) {
        warnlog("Wayland host-handle backend dispatches the provided wl_display "
                "from PDJE input thread; host integration must avoid concurrent "
                "Wayland dispatch on the same display.");
        warned_host_dispatch = true;
    }

    AttachSeatListeners();
    if (wl_display_roundtrip(display) < 0) {
        SetError("wl_display_roundtrip failed while initializing seat "
                 "listeners");
        return false;
    }

    if (wants_keyboard && keyboard == nullptr) {
        SetError("host Wayland seat has no keyboard capability");
        return false;
    }
    if (wants_pointer && pointer == nullptr) {
        SetError("host Wayland seat has no pointer capability");
        return false;
    }
    return true;
}

bool
WaylandInputCore::ConfigureInternalWindow()
{
    owned_window.emplace();
    if (!owned_window->Create("PDJE Input Fallback (Wayland)", 640, 360)) {
        SetError(owned_window->LastError());
        owned_window.reset();
        return false;
    }

    display        = owned_window->Display();
    target_surface = owned_window->Surface();
    seat           = owned_window->Seat();
    source_mode    = SourceMode::InternalWindow;

    if (display == nullptr || target_surface == nullptr) {
        SetError("internal Wayland window did not provide display/surface "
                 "handles");
        return false;
    }
    if (seat == nullptr && (wants_keyboard || wants_pointer)) {
        SetError("internal Wayland window has no wl_seat");
        return false;
    }

    if (owned_window && owned_window->SeatCapabilitiesKnown()) {
        // The owned window already holds the seat listener to capture the
        // initial capability event during window creation. Re-apply the cached
        // capabilities here instead of attaching a second listener.
        OnSeatCapabilities(this, seat, owned_window->SeatCapabilities());
    } else {
        SetError("internal Wayland seat capabilities were not captured during "
                 "window creation");
        return false;
    }
    if (wl_display_roundtrip(display) < 0) {
        SetError("wl_display_roundtrip failed while initializing internal "
                 "window seat listeners");
        return false;
    }

    if (wants_keyboard && keyboard == nullptr) {
        SetError("internal Wayland seat has no keyboard capability (or "
                 "capability event was not observed)");
        return false;
    }
    if (wants_pointer && pointer == nullptr) {
        SetError("internal Wayland seat has no pointer capability (or "
                 "capability event was not observed)");
        return false;
    }

    return true;
}

bool
WaylandInputCore::Configure(void                          *wl_display_ptr,
                            void                          *wl_surface_ptr,
                            const std::vector<DeviceData> &devs,
                            bool                           allow_internal_window)
{
    Reset();
    last_error.clear();
    ClearSelectionState();

    for (const auto &dev : devs) {
        switch (dev.Type) {
        case PDJE_Dev_Type::KEYBOARD:
            wants_keyboard = true;
            if (keyboard_name.empty() && !dev.Name.empty()) {
                keyboard_name = dev.Name;
            }
            break;
        case PDJE_Dev_Type::MOUSE:
            wants_pointer = true;
            if (pointer_name.empty() && !dev.Name.empty()) {
                pointer_name = dev.Name;
            }
            break;
        default:
            break;
        }
    }

    if (!wants_keyboard && !wants_pointer) {
        SetError("no supported Wayland input devices requested");
        return false;
    }

    if (wants_keyboard && keyboard_name.empty()) {
        keyboard_name = "Wayland Keyboard";
    }
    if (wants_pointer && pointer_name.empty()) {
        pointer_name = "Wayland Pointer";
    }

    if (wl_display_ptr != nullptr && wl_surface_ptr != nullptr) {
        return ConfigureHostHandles(wl_display_ptr, wl_surface_ptr);
    }

    if (allow_internal_window) {
        return ConfigureInternalWindow();
    }

    SetError("null host context handle");
    return false;
}

void
WaylandInputCore::Trig()
{
    stop_requested = false;
    if (display == nullptr) {
        warnlog("Wayland input loop started without a configured display.");
        return;
    }

    const int display_fd = wl_display_get_fd(display);
    if (display_fd < 0) {
        warnlog("wl_display_get_fd failed in Wayland input loop.");
        return;
    }

    while (!stop_requested.load()) {
        if (source_mode == SourceMode::InternalWindow && owned_window &&
            owned_window->Closed()) {
            warnlog("PDJE internal Wayland window was closed; stopping Wayland "
                    "input loop.");
            break;
        }

        const int pending_rc = wl_display_dispatch_pending(display);
        if (pending_rc < 0) {
            warnlog("wl_display_dispatch_pending failed in Wayland input loop.");
            break;
        }

        if (wl_display_flush(display) < 0 && errno != EAGAIN) {
            warnlog("wl_display_flush failed in Wayland input loop.");
            break;
        }

        pollfd pfd{};
        pfd.fd     = display_fd;
        pfd.events = POLLIN;

        const int pr = poll(&pfd, 1, 20);
        if (pr < 0) {
            if (errno == EINTR) {
                continue;
            }
            warnlog("poll failed in Wayland input loop.");
            break;
        }
        if (pr == 0) {
            continue;
        }
        if ((pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0) {
            warnlog("Wayland display fd reported an error/hangup.");
            break;
        }
        if ((pfd.revents & POLLIN) != 0) {
            if (wl_display_dispatch(display) < 0) {
                warnlog("wl_display_dispatch failed in Wayland input loop.");
                break;
            }
        }
    }
}

void
WaylandInputCore::Stop()
{
    stop_requested = true;
}

void
WaylandInputCore::Reset()
{
    Stop();

    ReleaseKeyboardCompat(keyboard);
    ReleasePointerCompat(pointer);

    if (source_mode == SourceMode::HostHandles) {
        ReleaseSeatCompat(seat);
        if (host_registry != nullptr) {
            wl_registry_destroy(host_registry);
            host_registry = nullptr;
        }
    } else {
        seat = nullptr; // owned by WaylandOwnedWindow
    }

    if (owned_window) {
        owned_window->Destroy();
        owned_window.reset();
    }

    display         = nullptr;
    target_surface  = nullptr;
    source_mode     = SourceMode::None;
    keyboard_focused = false;
    pointer_focused  = false;
    key_pressed.reset();
}
