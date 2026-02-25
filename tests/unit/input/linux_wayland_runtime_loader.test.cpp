#if defined(__linux__)

#include "WaylandRuntimeLoader.hpp"
#include <doctest/doctest.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

class FakeDynLibRuntime {
  private:
    struct HandleKeyHash {
        std::size_t
        operator()(const void *p) const noexcept
        {
            return std::hash<std::uintptr_t>{}(
                reinterpret_cast<std::uintptr_t>(p));
        }
    };

    std::unordered_map<std::string, void *> handles_by_name_;
    std::unordered_map<std::string, std::string> dlopen_error_by_name_;
    std::unordered_map<void *, std::unordered_set<std::string>, HandleKeyHash>
        symbols_by_handle_;
    std::unordered_map<void *, std::unordered_map<std::string, std::string>,
                       HandleKeyHash>
        dlsym_error_by_handle_;
    std::unordered_map<void *, std::unordered_set<std::string>, HandleKeyHash>
        dlsym_missing_no_error_by_handle_;

    std::string pending_error_;
    std::string returned_error_;
    int         next_symbol_id_ = 1000;

    static FakeDynLibRuntime *active_;

    const char *
    DlerrorImpl()
    {
        if (pending_error_.empty()) {
            returned_error_.clear();
            return nullptr;
        }
        returned_error_ = pending_error_;
        pending_error_.clear();
        return returned_error_.c_str();
    }

    void *
    DlopenImpl(const char *name, int /*flags*/)
    {
        ++dlopen_calls;
        dlopen_names.emplace_back(name == nullptr ? "" : name);
        if (name == nullptr) {
            pending_error_ = "invalid path";
            return nullptr;
        }
        const auto it = handles_by_name_.find(name);
        if (it != handles_by_name_.end()) {
            pending_error_.clear();
            return it->second;
        }
        const auto err_it = dlopen_error_by_name_.find(name);
        if (err_it != dlopen_error_by_name_.end()) {
            pending_error_ = err_it->second;
        } else {
            pending_error_ = "fake dlopen failure";
        }
        return nullptr;
    }

    void *
    DlsymImpl(void *handle, const char *symbol)
    {
        ++dlsym_calls;
        dlsym_symbols.emplace_back(symbol == nullptr ? "" : symbol);
        if (handle == nullptr || symbol == nullptr) {
            pending_error_ = "invalid dlsym args";
            return nullptr;
        }

        const auto miss_it = dlsym_missing_no_error_by_handle_.find(handle);
        if (miss_it != dlsym_missing_no_error_by_handle_.end() &&
            miss_it->second.find(symbol) != miss_it->second.end()) {
            pending_error_.clear();
            return nullptr;
        }

        const auto err_map_it = dlsym_error_by_handle_.find(handle);
        if (err_map_it != dlsym_error_by_handle_.end()) {
            const auto err_it = err_map_it->second.find(symbol);
            if (err_it != err_map_it->second.end()) {
                pending_error_ = err_it->second;
                return nullptr;
            }
        }

        const auto syms_it = symbols_by_handle_.find(handle);
        if (syms_it != symbols_by_handle_.end() &&
            syms_it->second.find(symbol) != syms_it->second.end()) {
            pending_error_.clear();
            return reinterpret_cast<void *>(
                static_cast<std::uintptr_t>(++next_symbol_id_));
        }

        pending_error_ = "undefined fake symbol";
        return nullptr;
    }

    int
    DlcloseImpl(void *handle)
    {
        ++dlclose_calls;
        dlclose_handles.push_back(handle);
        return 0;
    }

    static void *
    WrapDlopen(const char *name, int flags)
    {
        return active_ != nullptr ? active_->DlopenImpl(name, flags) : nullptr;
    }

    static void *
    WrapDlsym(void *handle, const char *symbol)
    {
        return active_ != nullptr ? active_->DlsymImpl(handle, symbol) : nullptr;
    }

    static int
    WrapDlclose(void *handle)
    {
        return active_ != nullptr ? active_->DlcloseImpl(handle) : 0;
    }

    static const char *
    WrapDlerror()
    {
        return active_ != nullptr ? active_->DlerrorImpl() : nullptr;
    }

  public:
    int dlopen_calls  = 0;
    int dlsym_calls   = 0;
    int dlclose_calls = 0;
    std::vector<std::string> dlopen_names;
    std::vector<std::string> dlsym_symbols;
    std::vector<void *>      dlclose_handles;

    FakeDynLibRuntime()
    {
        active_ = this;
    }
    ~FakeDynLibRuntime()
    {
        active_ = nullptr;
    }

    static void *
    Ptr(std::uintptr_t v)
    {
        return reinterpret_cast<void *>(v);
    }

    PDJE_DEFAULT_DEVICES::WaylandDynLibOps
    MakeOps() const noexcept
    {
        PDJE_DEFAULT_DEVICES::WaylandDynLibOps ops;
        ops.dlopen_fn  = &WrapDlopen;
        ops.dlsym_fn   = &WrapDlsym;
        ops.dlclose_fn = &WrapDlclose;
        ops.dlerror_fn = &WrapDlerror;
        return ops;
    }

    void
    SetLibraryHandle(const std::string &name, void *handle)
    {
        handles_by_name_[name] = handle;
    }

    void
    SetLibraryOpenError(const std::string &name, const std::string &error)
    {
        dlopen_error_by_name_[name] = error;
    }

    void
    AllowSymbol(void *handle, const std::string &symbol)
    {
        symbols_by_handle_[handle].insert(symbol);
    }

    void
    AllowManySymbols(void                         *handle,
                     const std::vector<std::string> &symbols)
    {
        for (const auto &s : symbols) {
            AllowSymbol(handle, s);
        }
    }

    void
    SetDlsymError(void *handle, const std::string &symbol, const std::string &error)
    {
        dlsym_error_by_handle_[handle][symbol] = error;
    }

    void
    SetDlsymMissingNoError(void *handle, const std::string &symbol)
    {
        dlsym_missing_no_error_by_handle_[handle].insert(symbol);
    }
};

FakeDynLibRuntime *FakeDynLibRuntime::active_ = nullptr;

std::vector<std::string>
WaylandRequiredSymbols()
{
    return {
        "wl_display_connect",
        "wl_display_disconnect",
        "wl_display_dispatch",
        "wl_display_get_fd"
    };
}

std::vector<std::string>
XkbRequiredSymbols()
{
    return {
        "xkb_context_new",
        "xkb_context_unref",
        "xkb_keymap_new_from_names",
        "xkb_keymap_unref",
        "xkb_state_new",
        "xkb_state_unref"
    };
}

void
ConfigureSuccessRuntime(FakeDynLibRuntime &fake,
                        void              *wayland_handle,
                        void              *xkb_handle)
{
    fake.SetLibraryHandle("libwayland-client.so.0", wayland_handle);
    fake.SetLibraryHandle("libxkbcommon.so.0", xkb_handle);
    fake.AllowManySymbols(wayland_handle, WaylandRequiredSymbols());
    fake.AllowManySymbols(xkb_handle, XkbRequiredSymbols());
}

} // namespace

TEST_CASE("input/linux: wayland runtime loader success path is ready")
{
    FakeDynLibRuntime fake;
    ConfigureSuccessRuntime(fake,
                            FakeDynLibRuntime::Ptr(0x11),
                            FakeDynLibRuntime::Ptr(0x22));

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    CHECK(loader.EnsureLoaded());
    CHECK(loader.IsLoaded());

    const auto &st = loader.Status();
    CHECK(st.ready);
    CHECK(st.wayland_client == PDJE_DEFAULT_DEVICES::LibLoadState::Loaded);
    CHECK(st.xkbcommon == PDJE_DEFAULT_DEVICES::LibLoadState::Loaded);
}

TEST_CASE("input/linux: wayland runtime loader classifies missing library")
{
    FakeDynLibRuntime fake;
    fake.SetLibraryOpenError("libwayland-client.so.0",
                             "libwayland-client.so.0: cannot open shared object file");
    fake.SetLibraryOpenError("libwayland-client.so",
                             "libwayland-client.so: cannot open shared object file");

    fake.SetLibraryHandle("libxkbcommon.so.0", FakeDynLibRuntime::Ptr(0x22));
    fake.AllowManySymbols(FakeDynLibRuntime::Ptr(0x22), XkbRequiredSymbols());

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    CHECK_FALSE(loader.EnsureLoaded());
    const auto &st = loader.Status();
    CHECK_FALSE(st.ready);
    CHECK(st.wayland_client == PDJE_DEFAULT_DEVICES::LibLoadState::Missing);
    CHECK(st.xkbcommon == PDJE_DEFAULT_DEVICES::LibLoadState::Loaded);
}

TEST_CASE("input/linux: wayland runtime loader classifies generic load error")
{
    FakeDynLibRuntime fake;
    fake.SetLibraryOpenError("libwayland-client.so.0", "permission denied");
    fake.SetLibraryOpenError("libwayland-client.so", "permission denied");

    fake.SetLibraryHandle("libxkbcommon.so.0", FakeDynLibRuntime::Ptr(0x22));
    fake.AllowManySymbols(FakeDynLibRuntime::Ptr(0x22), XkbRequiredSymbols());

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    CHECK_FALSE(loader.EnsureLoaded());
    const auto &st = loader.Status();
    CHECK(st.wayland_client == PDJE_DEFAULT_DEVICES::LibLoadState::LoadError);
    CHECK_FALSE(st.ready);
}

TEST_CASE("input/linux: wayland runtime loader classifies missing symbol")
{
    FakeDynLibRuntime fake;
    const void       *wayland_handle = FakeDynLibRuntime::Ptr(0x11);
    const void       *xkb_handle     = FakeDynLibRuntime::Ptr(0x22);

    ConfigureSuccessRuntime(
        fake, const_cast<void *>(wayland_handle), const_cast<void *>(xkb_handle));
    fake.SetDlsymMissingNoError(const_cast<void *>(wayland_handle),
                                "wl_display_get_fd");

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    CHECK_FALSE(loader.EnsureLoaded());
    const auto &st = loader.Status();
    CHECK(st.wayland_client ==
          PDJE_DEFAULT_DEVICES::LibLoadState::SymbolMissing);
    CHECK_FALSE(st.ready);
    CHECK(std::strstr(st.wayland_error, "wl_display_get_fd") != nullptr);
}

TEST_CASE("input/linux: wayland runtime loader partial success is not ready")
{
    FakeDynLibRuntime fake;
    fake.SetLibraryHandle("libwayland-client.so.0", FakeDynLibRuntime::Ptr(0x11));
    fake.AllowManySymbols(FakeDynLibRuntime::Ptr(0x11), WaylandRequiredSymbols());

    fake.SetLibraryOpenError("libxkbcommon.so.0",
                             "libxkbcommon.so.0: cannot open shared object file");
    fake.SetLibraryOpenError("libxkbcommon.so",
                             "libxkbcommon.so: cannot open shared object file");

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    CHECK_FALSE(loader.EnsureLoaded());
    const auto &st = loader.Status();
    CHECK(st.wayland_client == PDJE_DEFAULT_DEVICES::LibLoadState::Loaded);
    CHECK(st.xkbcommon == PDJE_DEFAULT_DEVICES::LibLoadState::Missing);
    CHECK_FALSE(st.ready);
}

TEST_CASE("input/linux: wayland runtime loader ensureloaded is idempotent after success")
{
    FakeDynLibRuntime fake;
    ConfigureSuccessRuntime(fake,
                            FakeDynLibRuntime::Ptr(0x11),
                            FakeDynLibRuntime::Ptr(0x22));

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    REQUIRE(loader.EnsureLoaded());

    const int dlopen_before = fake.dlopen_calls;
    const int dlsym_before  = fake.dlsym_calls;

    CHECK(loader.EnsureLoaded());
    CHECK(fake.dlopen_calls == dlopen_before);
    CHECK(fake.dlsym_calls == dlsym_before);
}

TEST_CASE("input/linux: wayland runtime loader unload closes handles and can reload")
{
    FakeDynLibRuntime fake;
    ConfigureSuccessRuntime(fake,
                            FakeDynLibRuntime::Ptr(0x11),
                            FakeDynLibRuntime::Ptr(0x22));

    PDJE_DEFAULT_DEVICES::WaylandRuntimeLoader loader(fake.MakeOps());
    REQUIRE(loader.EnsureLoaded());
    const int dlopen_before_unload = fake.dlopen_calls;

    loader.Unload();
    CHECK_FALSE(loader.IsLoaded());
    CHECK(fake.dlclose_calls >= 2);

    REQUIRE(loader.EnsureLoaded());
    CHECK(loader.IsLoaded());
    CHECK(fake.dlopen_calls > dlopen_before_unload);
}

#endif // __linux__
