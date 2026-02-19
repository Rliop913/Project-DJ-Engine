#include "WaylandRuntimeLoader.hpp"
#include <cstdio>
#include <cstring>
#include <dlfcn.h>

namespace PDJE_DEFAULT_DEVICES {
namespace {

inline void
SetError(char (&dst)[256], const char *msg) noexcept
{
    if (msg == nullptr || msg[0] == '\0') {
        dst[0] = '\0';
        return;
    }
    std::snprintf(dst, sizeof(dst), "%s", msg);
}

inline void
SetMissingSymbolError(char (&dst)[256], const char *symbol) noexcept
{
    std::snprintf(dst, sizeof(dst), "missing symbol: %s", symbol);
}

inline bool
IsLibraryMissingError(const char *msg) noexcept
{
    if (msg == nullptr) {
        return false;
    }
    return std::strstr(msg, "No such file") != nullptr ||
           std::strstr(msg, "cannot open shared object file") != nullptr;
}

void *
TryLoadLibrary(const char *const *names,
               const std::size_t  count,
               LibLoadState       &state,
               char (&error)[256]) noexcept
{
    state = LibLoadState::Unchecked;
    error[0] = '\0';

    const char *last_error = nullptr;
    for (std::size_t i = 0; i < count; ++i) {
        dlerror();
        void *h = dlopen(names[i], RTLD_NOW | RTLD_LOCAL);
        if (h != nullptr) {
            state = LibLoadState::Loaded;
            return h;
        }
        last_error = dlerror();
        if (last_error != nullptr) {
            SetError(error, last_error);
        }
    }

    state = IsLibraryMissingError(last_error) ? LibLoadState::Missing
                                              : LibLoadState::LoadError;
    if (error[0] == '\0') {
        SetError(error, "dlopen failed without a detailed error message.");
    }
    return nullptr;
}

bool
ResolveRequiredSymbols(void       *handle,
                       char (&error)[256],
                       const char *const *symbols,
                       const std::size_t  count) noexcept
{
    for (std::size_t i = 0; i < count; ++i) {
        dlerror();
        void       *sym = dlsym(handle, symbols[i]);
        const char *err = dlerror();
        if (sym == nullptr || err != nullptr) {
            if (err != nullptr && err[0] != '\0') {
                SetError(error, err);
            } else {
                SetMissingSymbolError(error, symbols[i]);
            }
            return false;
        }
    }
    return true;
}

} // namespace

WaylandRuntimeLoader::~WaylandRuntimeLoader()
{
    Unload();
}

void
WaylandRuntimeLoader::ClearStatusUnlocked() noexcept
{
    status.ready          = false;
    status.wayland_client = LibLoadState::Unchecked;
    status.xkbcommon      = LibLoadState::Unchecked;
    status.wayland_error[0] = '\0';
    status.xkb_error[0]     = '\0';
}

void
WaylandRuntimeLoader::UnloadUnlocked() noexcept
{
    if (wayland_client_handle != nullptr) {
        dlclose(wayland_client_handle);
        wayland_client_handle = nullptr;
    }
    if (xkbcommon_handle != nullptr) {
        dlclose(xkbcommon_handle);
        xkbcommon_handle = nullptr;
    }
}

bool
WaylandRuntimeLoader::ResolveWaylandSymbolsUnlocked() noexcept
{
    static constexpr const char *kWaylandSymbols[] = {
        "wl_display_connect",
        "wl_display_disconnect",
        "wl_display_dispatch",
        "wl_display_get_fd"
    };
    return ResolveRequiredSymbols(wayland_client_handle,
                                  status.wayland_error,
                                  kWaylandSymbols,
                                  sizeof(kWaylandSymbols) /
                                      sizeof(kWaylandSymbols[0]));
}

bool
WaylandRuntimeLoader::ResolveXKBCommonSymbolsUnlocked() noexcept
{
    static constexpr const char *kXKBSymbols[] = {
        "xkb_context_new",
        "xkb_context_unref",
        "xkb_keymap_new_from_names",
        "xkb_keymap_unref",
        "xkb_state_new",
        "xkb_state_unref"
    };
    return ResolveRequiredSymbols(xkbcommon_handle,
                                  status.xkb_error,
                                  kXKBSymbols,
                                  sizeof(kXKBSymbols) /
                                      sizeof(kXKBSymbols[0]));
}

bool
WaylandRuntimeLoader::EnsureLoaded() noexcept
{
    std::lock_guard<std::mutex> guard(lock);

    if (status.ready) {
        return true;
    }

    UnloadUnlocked();
    ClearStatusUnlocked();

    static constexpr const char *kWaylandLibraryNames[] = {
        "libwayland-client.so.0",
        "libwayland-client.so"
    };
    static constexpr const char *kXKBLibraryNames[] = {
        "libxkbcommon.so.0",
        "libxkbcommon.so"
    };

    wayland_client_handle = TryLoadLibrary(kWaylandLibraryNames,
                                           sizeof(kWaylandLibraryNames) /
                                               sizeof(kWaylandLibraryNames[0]),
                                           status.wayland_client,
                                           status.wayland_error);
    xkbcommon_handle = TryLoadLibrary(kXKBLibraryNames,
                                      sizeof(kXKBLibraryNames) /
                                          sizeof(kXKBLibraryNames[0]),
                                      status.xkbcommon,
                                      status.xkb_error);

    if (wayland_client_handle != nullptr &&
        !ResolveWaylandSymbolsUnlocked()) {
        status.wayland_client = LibLoadState::SymbolMissing;
        dlclose(wayland_client_handle);
        wayland_client_handle = nullptr;
    }
    if (xkbcommon_handle != nullptr &&
        !ResolveXKBCommonSymbolsUnlocked()) {
        status.xkbcommon = LibLoadState::SymbolMissing;
        dlclose(xkbcommon_handle);
        xkbcommon_handle = nullptr;
    }

    status.ready = status.wayland_client == LibLoadState::Loaded &&
                   status.xkbcommon == LibLoadState::Loaded;
    return status.ready;
}

void
WaylandRuntimeLoader::Unload() noexcept
{
    std::lock_guard<std::mutex> guard(lock);
    UnloadUnlocked();
    status.ready = false;
}

} // namespace PDJE_DEFAULT_DEVICES
