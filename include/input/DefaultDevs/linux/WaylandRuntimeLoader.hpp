#pragma once

#include <mutex>

namespace PDJE_DEFAULT_DEVICES {

enum class LibLoadState {
    Loaded,
    Missing,
    LoadError,
    SymbolMissing,
    Unchecked
};

struct WaylandRuntimeStatus {
    bool         ready          = false;
    LibLoadState wayland_client = LibLoadState::Unchecked;
    LibLoadState xkbcommon      = LibLoadState::Unchecked;
    char         wayland_error[256]{};
    char         xkb_error[256]{};
};

class WaylandRuntimeLoader {
  private:
    void                *wayland_client_handle = nullptr;
    void                *xkbcommon_handle      = nullptr;
    WaylandRuntimeStatus status{};
    mutable std::mutex   lock;

    void
    UnloadUnlocked() noexcept;
    void
    ClearStatusUnlocked() noexcept;
    bool
    ResolveWaylandSymbolsUnlocked() noexcept;
    bool
    ResolveXKBCommonSymbolsUnlocked() noexcept;

  public:
    WaylandRuntimeLoader() = default;
    ~WaylandRuntimeLoader();

    WaylandRuntimeLoader(const WaylandRuntimeLoader &)            = delete;
    WaylandRuntimeLoader &operator=(const WaylandRuntimeLoader &) = delete;

    bool
    EnsureLoaded() noexcept;
    bool
    IsLoaded() const noexcept
    {
        return status.ready;
    }
    const WaylandRuntimeStatus &
    Status() const noexcept
    {
        return status;
    }
    void
    Unload() noexcept;
};

} // namespace PDJE_DEFAULT_DEVICES
