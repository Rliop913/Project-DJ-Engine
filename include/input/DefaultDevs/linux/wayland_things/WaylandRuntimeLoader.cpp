#include "WaylandRuntimeLoader.hpp"
#include <cstdio>
#include <cstring>
#include <dlfcn.h>

namespace PDJE_DEFAULT_DEVICES {
namespace {

void *
SystemDlopen(const char *path, int flags)
{
    return dlopen(path, flags);
}

void *
SystemDlsym(void *handle, const char *symbol)
{
    return dlsym(handle, symbol);
}

int
SystemDlclose(void *handle)
{
    return dlclose(handle);
}

const char *
SystemDlerror()
{
    return dlerror();
}

WaylandDynLibOps
MakeSystemDynLibOps() noexcept
{
    WaylandDynLibOps ops;
    ops.dlopen_fn  = &SystemDlopen;
    ops.dlsym_fn   = &SystemDlsym;
    ops.dlclose_fn = &SystemDlclose;
    ops.dlerror_fn = &SystemDlerror;
    return ops;
}

bool
HasValidDynLibOps(const WaylandDynLibOps &ops) noexcept
{
    return ops.dlopen_fn != nullptr && ops.dlsym_fn != nullptr &&
           ops.dlclose_fn != nullptr && ops.dlerror_fn != nullptr;
}

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
TryLoadLibrary(const WaylandDynLibOps &ops,
               const char *const      *names,
               const std::size_t       count,
               const int               dlopen_flags,
               LibLoadState           &state,
               char (&error)[256]) noexcept
{
    state = LibLoadState::Unchecked;
    error[0] = '\0';

    const char *last_error = nullptr;
    for (std::size_t i = 0; i < count; ++i) {
        ops.dlerror_fn();
        void *h = ops.dlopen_fn(names[i], dlopen_flags);
        if (h != nullptr) {
            state = LibLoadState::Loaded;
            return h;
        }
        last_error = ops.dlerror_fn();
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
ResolveRequiredSymbols(const WaylandDynLibOps &ops,
                       void                   *handle,
                       char (&error)[256],
                       const char *const      *symbols,
                       const std::size_t       count) noexcept
{
    for (std::size_t i = 0; i < count; ++i) {
        ops.dlerror_fn();
        void       *sym = ops.dlsym_fn(handle, symbols[i]);
        const char *err = ops.dlerror_fn();
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

WaylandRuntimeLoader::WaylandRuntimeLoader() noexcept
    : dynlib_ops(MakeSystemDynLibOps())
{}

#ifdef PDJE_UNIT_TESTING
WaylandRuntimeLoader::WaylandRuntimeLoader(WaylandDynLibOps ops) noexcept
    : dynlib_ops(HasValidDynLibOps(ops) ? ops : MakeSystemDynLibOps())
{}
#endif

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
        dynlib_ops.dlclose_fn(wayland_client_handle);
        wayland_client_handle = nullptr;
    }
    if (xkbcommon_handle != nullptr) {
        dynlib_ops.dlclose_fn(xkbcommon_handle);
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
    return ResolveRequiredSymbols(dynlib_ops,
                                  wayland_client_handle,
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
    return ResolveRequiredSymbols(dynlib_ops,
                                  xkbcommon_handle,
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
    static constexpr int kDlopenFlags = RTLD_NOW | RTLD_LOCAL;

    wayland_client_handle = TryLoadLibrary(dynlib_ops,
                                           kWaylandLibraryNames,
                                           sizeof(kWaylandLibraryNames) /
                                               sizeof(kWaylandLibraryNames[0]),
                                           kDlopenFlags,
                                           status.wayland_client,
                                           status.wayland_error);
    xkbcommon_handle = TryLoadLibrary(dynlib_ops,
                                      kXKBLibraryNames,
                                      sizeof(kXKBLibraryNames) /
                                          sizeof(kXKBLibraryNames[0]),
                                      kDlopenFlags,
                                      status.xkbcommon,
                                      status.xkb_error);

    if (wayland_client_handle != nullptr &&
        !ResolveWaylandSymbolsUnlocked()) {
        status.wayland_client = LibLoadState::SymbolMissing;
        dynlib_ops.dlclose_fn(wayland_client_handle);
        wayland_client_handle = nullptr;
    }
    if (xkbcommon_handle != nullptr &&
        !ResolveXKBCommonSymbolsUnlocked()) {
        status.xkbcommon = LibLoadState::SymbolMissing;
        dynlib_ops.dlclose_fn(xkbcommon_handle);
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
