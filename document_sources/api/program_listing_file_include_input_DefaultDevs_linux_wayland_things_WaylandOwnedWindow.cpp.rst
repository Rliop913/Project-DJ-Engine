
.. _program_listing_file_include_input_DefaultDevs_linux_wayland_things_WaylandOwnedWindow.cpp:

Program Listing for File WaylandOwnedWindow.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_wayland_things_WaylandOwnedWindow.cpp>` (``include/input/DefaultDevs/linux/wayland_things/WaylandOwnedWindow.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "WaylandOwnedWindow.hpp"
   #include "wayland_protocols/xdg-shell-client-protocol.h"
   
   #include <algorithm>
   #include <cerrno>
   #include <cstdlib>
   #include <cstring>
   #include <fcntl.h>
   #include <string>
   #include <utility>
   #include <sys/mman.h>
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <unistd.h>
   #include <wayland-client.h>
   
   namespace {
   
   int
   CreateAnonymousShmFile(std::size_t size)
   {
       const char *runtime_dir = std::getenv("XDG_RUNTIME_DIR");
       std::string base        = (runtime_dir && runtime_dir[0] != '\0')
                                     ? runtime_dir
                                     : "/tmp";
       std::string tmpl = base + "/pdje-wayland-shm-XXXXXX";
       std::string path = tmpl;
       int         fd   = mkstemp(path.data());
       if (fd < 0) {
           return -1;
       }
       unlink(path.c_str());
       if (ftruncate(fd, static_cast<off_t>(size)) != 0) {
           close(fd);
           return -1;
       }
       return fd;
   }
   
   template <typename T>
   void
   ProxyDestroy(T *&proxy) noexcept
   {
       if (proxy != nullptr) {
           wl_proxy_destroy(reinterpret_cast<wl_proxy *>(proxy));
           proxy = nullptr;
       }
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
   WaylandOwnedWindow::SetError(std::string msg)
   {
       last_error_ = std::move(msg);
   }
   
   WaylandOwnedWindow::~WaylandOwnedWindow()
   {
       Destroy();
   }
   
   void
   WaylandOwnedWindow::OnRegistryGlobal(void       *data,
                                        wl_registry *registry,
                                        uint32_t     name,
                                        const char  *interface,
                                        uint32_t     version)
   {
       auto *self = static_cast<WaylandOwnedWindow *>(data);
       if (self == nullptr || interface == nullptr) {
           return;
       }
   
       if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
           if (self->compositor_ == nullptr) {
               const uint32_t bind_version = std::min<uint32_t>(version, 4);
               self->compositor_ = static_cast<wl_compositor *>(
                   wl_registry_bind(registry,
                                    name,
                                    &wl_compositor_interface,
                                    bind_version));
           }
           return;
       }
   
       if (std::strcmp(interface, wl_shm_interface.name) == 0) {
           if (self->shm_ == nullptr) {
               self->shm_ = static_cast<wl_shm *>(
                   wl_registry_bind(registry, name, &wl_shm_interface, 1));
           }
           return;
       }
   
       if (std::strcmp(interface, wl_seat_interface.name) == 0) {
           if (self->seat_ == nullptr) {
               const uint32_t bind_version = std::min<uint32_t>(version, 5);
               self->seat_ = static_cast<wl_seat *>(
                   wl_registry_bind(registry, name, &wl_seat_interface, bind_version));
               if (self->seat_ != nullptr) {
                   static constexpr wl_seat_listener kSeatListener = {
                       .capabilities = &WaylandOwnedWindow::OnSeatCapabilities,
                       .name         = &WaylandOwnedWindow::OnSeatName,
                   };
                   wl_seat_add_listener(self->seat_, &kSeatListener, self);
               }
           }
           return;
       }
   
       if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
           if (self->wm_base_ == nullptr) {
               self->wm_base_ = static_cast<xdg_wm_base *>(
                   wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
           }
       }
   }
   
   void
   WaylandOwnedWindow::OnRegistryGlobalRemove(void *, wl_registry *, uint32_t)
   {
   }
   
   void
   WaylandOwnedWindow::OnWmBasePing(void *, xdg_wm_base *wm_base, uint32_t serial)
   {
       if (wm_base != nullptr) {
           xdg_wm_base_pong(wm_base, serial);
       }
   }
   
   void
   WaylandOwnedWindow::OnXdgSurfaceConfigure(void *data,
                                             xdg_surface *surface,
                                             uint32_t     serial)
   {
       auto *self = static_cast<WaylandOwnedWindow *>(data);
       if (surface != nullptr) {
           xdg_surface_ack_configure(surface, serial);
       }
       if (self != nullptr) {
           self->configured_ = true;
       }
   }
   
   void
   WaylandOwnedWindow::OnToplevelConfigure(void *,
                                           xdg_toplevel *,
                                           int32_t,
                                           int32_t,
                                           wl_array *)
   {
   }
   
   void
   WaylandOwnedWindow::OnToplevelClose(void *data, xdg_toplevel *)
   {
       auto *self = static_cast<WaylandOwnedWindow *>(data);
       if (self != nullptr) {
           self->closed_.store(true);
       }
   }
   
   void
   WaylandOwnedWindow::OnBufferRelease(void *, wl_buffer *)
   {
   }
   
   void
   WaylandOwnedWindow::OnSeatCapabilities(void     *data,
                                          wl_seat  *,
                                          uint32_t  capabilities)
   {
       auto *self = static_cast<WaylandOwnedWindow *>(data);
       if (self == nullptr) {
           return;
       }
       self->seat_caps_       = capabilities;
       self->seat_caps_known_ = true;
   }
   
   void
   WaylandOwnedWindow::OnSeatName(void *, wl_seat *, const char *)
   {
   }
   
   bool
   WaylandOwnedWindow::CreateShmBuffer()
   {
       if (shm_ == nullptr || surface_ == nullptr) {
           SetError("wl_shm or wl_surface unavailable while creating internal "
                    "Wayland window buffer");
           return false;
       }
   
       DestroyShmBuffer();
   
       const int bytes_per_pixel = 4;
       const int stride          = width_ * bytes_per_pixel;
       if (width_ <= 0 || height_ <= 0 || stride <= 0) {
           SetError("invalid internal Wayland window dimensions");
           return false;
       }
   
       buffer_size_ = static_cast<std::size_t>(stride) *
                      static_cast<std::size_t>(height_);
       shm_fd_ = CreateAnonymousShmFile(buffer_size_);
       if (shm_fd_ < 0) {
           SetError("failed to create shm file for internal Wayland window");
           return false;
       }
   
       buffer_map_ =
           mmap(nullptr, buffer_size_, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0);
       if (buffer_map_ == MAP_FAILED) {
           buffer_map_ = nullptr;
           close(shm_fd_);
           shm_fd_ = -1;
           SetError("mmap failed for internal Wayland window buffer");
           return false;
       }
   
       shm_pool_ = wl_shm_create_pool(
           shm_, shm_fd_, static_cast<int32_t>(buffer_size_));
       if (shm_pool_ == nullptr) {
           DestroyShmBuffer();
           SetError("wl_shm_create_pool failed");
           return false;
       }
   
       buffer_ = wl_shm_pool_create_buffer(shm_pool_,
                                           0,
                                           width_,
                                           height_,
                                           stride,
                                           WL_SHM_FORMAT_XRGB8888);
       if (buffer_ == nullptr) {
           DestroyShmBuffer();
           SetError("wl_shm_pool_create_buffer failed");
           return false;
       }
   
       static constexpr wl_buffer_listener kBufferListener = {
           .release = &WaylandOwnedWindow::OnBufferRelease,
       };
       wl_buffer_add_listener(buffer_, &kBufferListener, this);
   
       auto *pixels = static_cast<uint32_t *>(buffer_map_);
       const std::size_t pixel_count =
           static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);
       for (std::size_t i = 0; i < pixel_count; ++i) {
           pixels[i] = 0xFF2A3E52u; // opaque XRGB-like solid color
       }
       return true;
   }
   
   void
   WaylandOwnedWindow::DestroyShmBuffer() noexcept
   {
       ProxyDestroy(buffer_);
       ProxyDestroy(shm_pool_);
   
       if (buffer_map_ != nullptr) {
           munmap(buffer_map_, buffer_size_);
           buffer_map_ = nullptr;
       }
       if (shm_fd_ >= 0) {
           close(shm_fd_);
           shm_fd_ = -1;
       }
       buffer_size_ = 0;
   }
   
   void
   WaylandOwnedWindow::CleanupWaylandObjects() noexcept
   {
       if (toplevel_ != nullptr) {
           xdg_toplevel_destroy(toplevel_);
           toplevel_ = nullptr;
       }
       if (xdg_surface_ != nullptr) {
           xdg_surface_destroy(xdg_surface_);
           xdg_surface_ = nullptr;
       }
       if (surface_ != nullptr) {
           wl_surface_destroy(surface_);
           surface_ = nullptr;
       }
   
       if (wm_base_ != nullptr) {
           xdg_wm_base_destroy(wm_base_);
           wm_base_ = nullptr;
       }
   
       ReleaseSeatCompat(seat_);
       ProxyDestroy(shm_);
       ProxyDestroy(compositor_);
       ProxyDestroy(registry_);
   }
   
   bool
   WaylandOwnedWindow::Create(const char *title, int width, int height)
   {
       Destroy();
       last_error_.clear();
       closed_.store(false);
   
       width_      = std::max(width, 64);
       height_     = std::max(height, 64);
       configured_ = false;
   
       display_ = wl_display_connect(nullptr);
       if (display_ == nullptr) {
           SetError("wl_display_connect failed for internal Wayland window");
           return false;
       }
   
       registry_ = wl_display_get_registry(display_);
       if (registry_ == nullptr) {
           SetError("wl_display_get_registry failed");
           Destroy();
           return false;
       }
   
       static constexpr wl_registry_listener kRegistryListener = {
           .global        = &WaylandOwnedWindow::OnRegistryGlobal,
           .global_remove = &WaylandOwnedWindow::OnRegistryGlobalRemove,
       };
       wl_registry_add_listener(registry_, &kRegistryListener, this);
   
       if (wl_display_roundtrip(display_) < 0) {
           SetError("wl_display_roundtrip failed while discovering globals");
           Destroy();
           return false;
       }
   
       if (wm_base_ != nullptr) {
           static constexpr xdg_wm_base_listener kWmBaseListener = {
               .ping = &WaylandOwnedWindow::OnWmBasePing,
           };
           xdg_wm_base_add_listener(wm_base_, &kWmBaseListener, this);
       }
   
       if (compositor_ == nullptr || shm_ == nullptr || wm_base_ == nullptr) {
           SetError("required Wayland globals missing (compositor/shm/xdg_wm_base)");
           Destroy();
           return false;
       }
   
       surface_ = wl_compositor_create_surface(compositor_);
       if (surface_ == nullptr) {
           SetError("wl_compositor_create_surface failed");
           Destroy();
           return false;
       }
   
       xdg_surface_ = xdg_wm_base_get_xdg_surface(wm_base_, surface_);
       if (xdg_surface_ == nullptr) {
           SetError("xdg_wm_base_get_xdg_surface failed");
           Destroy();
           return false;
       }
   
       static constexpr xdg_surface_listener kXdgSurfaceListener = {
           .configure = &WaylandOwnedWindow::OnXdgSurfaceConfigure,
       };
       xdg_surface_add_listener(xdg_surface_, &kXdgSurfaceListener, this);
   
       toplevel_ = xdg_surface_get_toplevel(xdg_surface_);
       if (toplevel_ == nullptr) {
           SetError("xdg_surface_get_toplevel failed");
           Destroy();
           return false;
       }
   
       static constexpr xdg_toplevel_listener kToplevelListener = {
           .configure         = &WaylandOwnedWindow::OnToplevelConfigure,
           .close             = &WaylandOwnedWindow::OnToplevelClose,
           .configure_bounds  = nullptr,
           .wm_capabilities   = nullptr,
       };
       xdg_toplevel_add_listener(toplevel_, &kToplevelListener, this);
       if (title != nullptr && title[0] != '\0') {
           xdg_toplevel_set_title(toplevel_, title);
       } else {
           xdg_toplevel_set_title(toplevel_, "PDJE Input Fallback (Wayland)");
       }
       xdg_toplevel_set_app_id(toplevel_, "pdje-input-fallback");
   
       wl_surface_commit(surface_);
   
       if (wl_display_roundtrip(display_) < 0) {
           SetError("wl_display_roundtrip failed while awaiting xdg configure");
           Destroy();
           return false;
       }
       if (!configured_) {
           SetError("internal Wayland window was not configured by compositor");
           Destroy();
           return false;
       }
   
       if (!CreateShmBuffer()) {
           Destroy();
           return false;
       }
   
       wl_surface_attach(surface_, buffer_, 0, 0);
       wl_surface_damage_buffer(surface_, 0, 0, width_, height_);
       wl_surface_commit(surface_);
   
       if (wl_display_roundtrip(display_) < 0) {
           SetError("wl_display_roundtrip failed after attaching internal window "
                    "buffer");
           Destroy();
           return false;
       }
   
       return true;
   }
   
   void
   WaylandOwnedWindow::Destroy() noexcept
   {
       closed_.store(false);
       DestroyShmBuffer();
       CleanupWaylandObjects();
   
       if (display_ != nullptr) {
           wl_display_disconnect(display_);
           display_ = nullptr;
       }
   
       configured_ = false;
       seat_caps_known_ = false;
       seat_caps_       = 0;
       width_      = 640;
       height_     = 360;
   }
