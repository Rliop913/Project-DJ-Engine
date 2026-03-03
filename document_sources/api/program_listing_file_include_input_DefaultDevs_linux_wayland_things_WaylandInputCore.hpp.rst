
.. _program_listing_file_include_input_DefaultDevs_linux_wayland_things_WaylandInputCore.hpp:

Program Listing for File WaylandInputCore.hpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_wayland_things_WaylandInputCore.hpp>` (``include/input/DefaultDevs/linux/wayland_things/WaylandInputCore.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_Transfer.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "WaylandOwnedWindow.hpp"
   #include <cstdint>
   #include <atomic>
   #include <bitset>
   #include <optional>
   #include <string>
   #include <vector>
   #include <wayland-client.h>
   
   struct wl_display;
   struct wl_surface;
   struct wl_registry;
   struct wl_seat;
   struct wl_keyboard;
   struct wl_pointer;
   struct wl_array;
   
   class WaylandInputCore {
     private:
       enum class SourceMode { None, HostHandles, InternalWindow };
   
       PDJE_IPC::PDJE_Input_Transfer *out = nullptr;
   
       std::optional<WaylandOwnedWindow> owned_window;
   
       SourceMode source_mode = SourceMode::None;
   
       wl_display  *display        = nullptr;
       wl_surface  *target_surface = nullptr;
       wl_registry *host_registry  = nullptr;
       wl_seat     *seat           = nullptr;
       wl_keyboard *keyboard       = nullptr;
       wl_pointer  *pointer        = nullptr;
   
       bool wants_keyboard = false;
       bool wants_pointer  = false;
       bool keyboard_focused = false;
       bool pointer_focused  = false;
       std::bitset<102> key_pressed{};
   
       std::string keyboard_name;
       std::string pointer_name;
       std::string last_error;
       std::atomic<bool> stop_requested{ false };
       bool warned_host_dispatch = false;
   
       void
       SetError(std::string msg);
       void
       ClearSelectionState();
       void
       WriteKeyboardEvent(uint32_t time_ms, uint32_t key, bool pressed);
       void
       WritePointerMotion(uint32_t time_ms, int x, int y);
       void
       WritePointerButton(uint32_t time_ms, uint32_t button, bool pressed);
       void
       WritePointerAxis(uint32_t time_ms, uint32_t axis, int wheel_delta);
       void
       AttachSeatListeners();
       bool
       ConfigureHostHandles(void *wl_display_ptr, void *wl_surface_ptr);
       bool
       ConfigureInternalWindow();
   
       static void
       OnHostRegistryGlobal(void *data,
                            wl_registry *registry,
                            uint32_t     name,
                            const char  *interface,
                            uint32_t     version);
       static void
       OnHostRegistryGlobalRemove(void *, wl_registry *, uint32_t);
   
       static void
       OnSeatCapabilities(void *data,
                          wl_seat *seat,
                          uint32_t capabilities);
       static void
       OnSeatName(void *, wl_seat *, const char *);
   
       static void
       OnKeyboardKeymap(void *data,
                        wl_keyboard *,
                        uint32_t format,
                        int32_t  fd,
                        uint32_t size);
       static void
       OnKeyboardEnter(void *data,
                       wl_keyboard *,
                       uint32_t serial,
                       wl_surface *surface,
                       struct wl_array *keys);
       static void
       OnKeyboardLeave(void *data,
                       wl_keyboard *,
                       uint32_t serial,
                       wl_surface *surface);
       static void
       OnKeyboardKey(void *data,
                     wl_keyboard *,
                     uint32_t serial,
                     uint32_t time,
                     uint32_t key,
                     uint32_t state);
       static void
       OnKeyboardModifiers(void *,
                           wl_keyboard *,
                           uint32_t,
                           uint32_t,
                           uint32_t,
                           uint32_t,
                           uint32_t);
       static void
       OnKeyboardRepeatInfo(void *, wl_keyboard *, int32_t, int32_t);
   
       static void
       OnPointerEnter(void *data,
                      wl_pointer *,
                      uint32_t serial,
                      wl_surface *surface,
                      wl_fixed_t surface_x,
                      wl_fixed_t surface_y);
       static void
       OnPointerLeave(void *data,
                      wl_pointer *,
                      uint32_t serial,
                      wl_surface *surface);
       static void
       OnPointerMotion(void *data,
                       wl_pointer *,
                       uint32_t time,
                       wl_fixed_t surface_x,
                       wl_fixed_t surface_y);
       static void
       OnPointerButton(void *data,
                       wl_pointer *,
                       uint32_t serial,
                       uint32_t time,
                       uint32_t button,
                       uint32_t state);
       static void
       OnPointerAxis(void *data,
                     wl_pointer *,
                     uint32_t time,
                     uint32_t axis,
                     wl_fixed_t value);
       static void
       OnPointerFrame(void *, wl_pointer *);
       static void
       OnPointerAxisSource(void *, wl_pointer *, uint32_t);
       static void
       OnPointerAxisStop(void *, wl_pointer *, uint32_t, uint32_t);
       static void
       OnPointerAxisDiscrete(void *data, wl_pointer *, uint32_t axis, int32_t discrete);
       static void
       OnPointerAxisValue120(void *data, wl_pointer *, uint32_t axis, int32_t value120);
       static void
       OnPointerAxisRelativeDirection(void *, wl_pointer *, uint32_t, uint32_t);
   
     public:
       void
       set_Input_Transfer(PDJE_IPC::PDJE_Input_Transfer *input_trsf)
       {
           out = input_trsf;
       }
   
       bool
       Configure(void                          *wl_display_ptr,
                 void                          *wl_surface_ptr,
                 const std::vector<DeviceData> &devs,
                 bool                           allow_internal_window);
   
       void
       Trig();
   
       void
       Stop();
   
       void
       Reset();
   
       bool
       UsingInternalWindow() const noexcept
       {
           return source_mode == SourceMode::InternalWindow;
       }
       bool
       HasConfiguredDevice() const noexcept
       {
           return wants_keyboard || wants_pointer;
       }
       const std::string &
       LastError() const noexcept
       {
           return last_error;
       }
   };
