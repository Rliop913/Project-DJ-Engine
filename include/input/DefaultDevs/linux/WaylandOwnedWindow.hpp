#pragma once

#include <cstddef>
#include <cstdint>
#include <atomic>
#include <string>

struct wl_display;
struct wl_registry;
struct wl_compositor;
struct wl_surface;
struct wl_shm;
struct wl_shm_pool;
struct wl_buffer;
struct wl_seat;
struct wl_array;
struct xdg_wm_base;
struct xdg_surface;
struct xdg_toplevel;

class WaylandOwnedWindow {
  private:
    wl_display    *display_     = nullptr;
    wl_registry   *registry_    = nullptr;
    wl_compositor *compositor_  = nullptr;
    wl_shm        *shm_         = nullptr;
    wl_seat       *seat_        = nullptr;
    xdg_wm_base   *wm_base_     = nullptr;
    wl_surface    *surface_     = nullptr;
    xdg_surface   *xdg_surface_ = nullptr;
    xdg_toplevel  *toplevel_    = nullptr;

    wl_shm_pool *shm_pool_ = nullptr;
    wl_buffer   *buffer_   = nullptr;
    void        *buffer_map_ = nullptr;
    std::size_t  buffer_size_ = 0;
    int          shm_fd_      = -1;
    int          width_       = 640;
    int          height_      = 360;
    bool         configured_  = false;
    std::atomic<bool> closed_{ false };
    std::string  last_error_;

    void
    SetError(std::string msg);
    bool
    CreateShmBuffer();
    void
    DestroyShmBuffer() noexcept;
    void
    CleanupWaylandObjects() noexcept;

    static void
    OnRegistryGlobal(void *data,
                     wl_registry *registry,
                     uint32_t     name,
                     const char  *interface,
                     uint32_t     version);
    static void
    OnRegistryGlobalRemove(void *, wl_registry *, uint32_t);
    static void
    OnWmBasePing(void *, xdg_wm_base *wm_base, uint32_t serial);
    static void
    OnXdgSurfaceConfigure(void *data, xdg_surface *surface, uint32_t serial);
    static void
    OnToplevelConfigure(void *, xdg_toplevel *, int32_t, int32_t, struct wl_array *);
    static void
    OnToplevelClose(void *data, xdg_toplevel *);
    static void
    OnBufferRelease(void *, wl_buffer *);

  public:
    WaylandOwnedWindow() = default;
    ~WaylandOwnedWindow();

    WaylandOwnedWindow(const WaylandOwnedWindow &)            = delete;
    WaylandOwnedWindow &operator=(const WaylandOwnedWindow &) = delete;
    WaylandOwnedWindow(WaylandOwnedWindow &&)                 = delete;
    WaylandOwnedWindow &operator=(WaylandOwnedWindow &&)      = delete;

    bool
    Create(const char *title, int width, int height);
    void
    Destroy() noexcept;

    wl_display *
    Display() const noexcept
    {
        return display_;
    }
    wl_surface *
    Surface() const noexcept
    {
        return surface_;
    }
    wl_seat *
    Seat() const noexcept
    {
        return seat_;
    }
    bool
    Closed() const noexcept
    {
        return closed_.load();
    }
    const std::string &
    LastError() const noexcept
    {
        return last_error_;
    }
};
