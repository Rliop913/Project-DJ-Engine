
.. _program_listing_file_include_input_DefaultDevs_linux_wayland_things_WaylandRuntimeLoader.hpp:

Program Listing for File WaylandRuntimeLoader.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_wayland_things_WaylandRuntimeLoader.hpp>` (``include/input/DefaultDevs/linux/wayland_things/WaylandRuntimeLoader.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
   
   struct WaylandDynLibOps {
       void *(*dlopen_fn)(const char *, int)  = nullptr;
       void *(*dlsym_fn)(void *, const char *) = nullptr;
       int (*dlclose_fn)(void *)              = nullptr;
       const char *(*dlerror_fn)()            = nullptr;
   };
   
   class WaylandRuntimeLoader {
     private:
       void                *wayland_client_handle = nullptr;
       void                *xkbcommon_handle      = nullptr;
       WaylandRuntimeStatus status{};
       WaylandDynLibOps     dynlib_ops{};
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
       WaylandRuntimeLoader() noexcept;
   #ifdef PDJE_UNIT_TESTING
       explicit WaylandRuntimeLoader(WaylandDynLibOps ops) noexcept;
   #endif
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
