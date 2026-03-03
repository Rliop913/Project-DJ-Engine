
.. _program_listing_file_include_input_DefaultDevs_linux_DefaultDevs.hpp:

Program Listing for File DefaultDevs.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_DefaultDevs.hpp>` (``include/input/DefaultDevs/linux/DefaultDevs.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "WaylandRuntimeLoader.hpp"
   #include "WaylandInputCore.hpp"
   
   #include "InputCore.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <exception>
   #include <filesystem>
   #include <nlohmann/json.hpp>
   #include <optional>
   #include <thread>
   #include <unordered_map>
   #include <vector>
   namespace PDJE_DEFAULT_DEVICES {
   using namespace PDJE_IPC;
   using nj     = nlohmann::json;
   namespace fs = std::filesystem;
   class DefaultDevs {
     private:
       enum class StoredBackendKind { Evdev, Wayland };
       enum class ActiveBackendKind { None, Evdev, Wayland };
       enum class WaylandSourceMode { None, HostHandles, InternalWindow };
   
       PDJE_IPC::PDJE_Input_Transfer input_buffer;
       struct device_metadata {
           StoredBackendKind backend_kind = StoredBackendKind::Evdev;
           fs::path          dev_path{};
           std::string       source_id;
           PDJE_Dev_Type     dev_type = PDJE_Dev_Type::UNKNOWN;
       };
   
       // key: device name (cross-platform compatibility key)
       std::unordered_map<std::string, device_metadata> stored_dev;
   
       std::optional<InputCore>       evdev_core;
       std::optional<WaylandInputCore> wayland_core;
       std::optional<std::thread> input_thread;
       WaylandRuntimeLoader       wayland_loader;
       void                      *platform_ctx0_ = nullptr;
       void                      *platform_ctx1_ = nullptr;
       bool                       use_internal_window_ = false;
       ActiveBackendKind          active_backend = ActiveBackendKind::None;
       WaylandSourceMode          wayland_source_mode = WaylandSourceMode::None;
   
       static bool
       IsWaylandSyntheticId(const std::string &id) noexcept;
       bool
       HasValidWaylandHostContext() const noexcept;
       bool
       ConfigureWayland(const std::vector<DeviceData> &devs);
   
     public:
       void
       SetPlatformContexts(void *platform_ctx0,
                           void *platform_ctx1,
                           bool  use_internal_window) noexcept
       {
           platform_ctx0_ = platform_ctx0;
           platform_ctx1_ = platform_ctx1;
           use_internal_window_ = use_internal_window;
       }
       std::string
       GetCurrentBackendString() const;
       bool
       Kill()
       {
           return true; // compatibility no-op (windows parity)
       }
       std::vector<DeviceData>
       GetDevices();
   
       PDJE_IPC::PDJE_Input_Transfer *
       GetInputBufferPTR()
       {
   
           return &(input_buffer);
       }
       void
       Ready();
   
       void
       RunLoop();
       void
       TerminateLoop();
   
       bool
       Config(const std::vector<DeviceData> &devs);
       DefaultDevs();
       ~DefaultDevs();
   };
   
   }; // namespace PDJE_DEFAULT_DEVICES
