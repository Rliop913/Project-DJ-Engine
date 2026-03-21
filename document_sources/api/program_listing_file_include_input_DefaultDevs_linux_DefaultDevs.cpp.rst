
.. _program_listing_file_include_input_DefaultDevs_linux_DefaultDevs.cpp:

Program Listing for File DefaultDevs.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_DefaultDevs.cpp>` (``include/input/DefaultDevs/linux/DefaultDevs.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "DefaultDevs.hpp"
   #include "LinuxInputContracts.hpp"
   #include <exception>
   #include <fcntl.h>
   #include <unistd.h>
   
   namespace PDJE_DEFAULT_DEVICES {
   namespace {
   } // namespace
   
   DefaultDevs::DefaultDevs() : input_buffer(1024)
   {
   }
   
   DefaultDevs::~DefaultDevs()
   {
       TerminateLoop();
   }
   
   bool
   DefaultDevs::IsWaylandSyntheticId(const std::string &id) noexcept
   {
       return LINUX_INPUT_CONTRACTS::IsWaylandSyntheticId(id);
   }
   
   bool
   DefaultDevs::HasValidWaylandHostContext() const noexcept
   {
       return platform_ctx0_ != nullptr && platform_ctx1_ != nullptr;
   }
   
   std::string
   DefaultDevs::GetCurrentBackendString() const
   {
       switch (active_backend) {
       case ActiveBackendKind::Evdev:
           return "evdev";
       case ActiveBackendKind::Wayland:
           return "wayland";
       default:
           return "none";
       }
   }
   
   void
   DefaultDevs::Ready()
   {
       if (!wayland_loader.EnsureLoaded()) {
           const auto &status = wayland_loader.Status();
           if (status.wayland_client == LibLoadState::Missing) {
               warnlog("Wayland runtime unavailable: missing libwayland-client");
               if (status.wayland_error[0] != '\0') {
                   warnlog(status.wayland_error);
               }
           } else if (status.wayland_client != LibLoadState::Loaded) {
               warnlog(
                   "Wayland runtime unavailable: failed to load libwayland-client");
               if (status.wayland_error[0] != '\0') {
                   warnlog(status.wayland_error);
               }
           }
   
           if (status.xkbcommon == LibLoadState::Missing) {
               warnlog("Wayland runtime unavailable: missing libxkbcommon");
               if (status.xkb_error[0] != '\0') {
                   warnlog(status.xkb_error);
               }
           } else if (status.xkbcommon != LibLoadState::Loaded) {
               warnlog("Wayland runtime unavailable: failed to load libxkbcommon");
               if (status.xkb_error[0] != '\0') {
                   warnlog(status.xkb_error);
               }
           }
       }
   
       if (!evdev_core) {
           evdev_core.emplace();
           evdev_core->set_Input_Transfer(&input_buffer);
       }
   }
   
   bool
   DefaultDevs::ConfigureWayland(const std::vector<DeviceData> &devs)
   {
       const bool has_host_handles = HasValidWaylandHostContext();
       if (!has_host_handles && !use_internal_window_) {
           warnlog("evdev failed and wayland fallback unavailable: null host "
                   "context handle");
           return false;
       }
       if (!has_host_handles && use_internal_window_) {
           warnlog("evdev failed; trying wayland fallback via internal window");
       }
   
       if (!wayland_loader.EnsureLoaded()) {
           warnlog("evdev failed and wayland fallback unavailable: wayland "
                   "runtime not loaded");
           return false;
       }
   
       if (!wayland_core) {
           wayland_core.emplace();
       }
       wayland_core->set_Input_Transfer(&input_buffer);
       if (!wayland_core->Configure(platform_ctx0_,
                                    platform_ctx1_,
                                    devs,
                                    use_internal_window_)) {
           warnlog("failed to configure wayland fallback backend on linux.");
           if (!wayland_core->LastError().empty()) {
               warnlog(wayland_core->LastError());
           }
           return false;
       }
   
       active_backend = ActiveBackendKind::Wayland;
       wayland_source_mode = wayland_core->UsingInternalWindow()
                                 ? WaylandSourceMode::InternalWindow
                                 : WaylandSourceMode::HostHandles;
       return true;
   }
   
   bool
   DefaultDevs::Config(const std::vector<DeviceData> &devs)
   {
       try {
           if (!evdev_core) {
               Ready();
           }
   
           active_backend = ActiveBackendKind::None;
           wayland_source_mode = WaylandSourceMode::None;
   
           std::vector<DeviceData> evdev_targets;
           std::vector<DeviceData> wayland_targets;
           bool                    saw_evdev   = false;
           bool                    saw_wayland = false;
   
           for (const auto &i : devs) {
               if (i.Name.empty()) {
                   continue;
               }
   
               auto searched = stored_dev.find(i.Name);
               if (searched != stored_dev.end()) {
                   if (searched->second.backend_kind == StoredBackendKind::Wayland) {
                       saw_wayland = true;
                       wayland_targets.push_back(i);
                   } else {
                       saw_evdev = true;
                       evdev_targets.push_back(i);
                   }
                   continue;
               }
   
               if (IsWaylandSyntheticId(i.device_specific_id)) {
                   saw_wayland = true;
                   wayland_targets.push_back(i);
               }
           }
   
           if (saw_evdev && saw_wayland) {
               warnlog("linux input config rejected mixed evdev/wayland backend "
                       "selection.");
               return false;
           }
   
           if (saw_wayland) {
               return ConfigureWayland(wayland_targets);
           }
   
           if (!saw_evdev) {
               return false;
           }
   
           if (!evdev_core) {
               evdev_core.emplace();
           }
           evdev_core->Reset();
           evdev_core->set_Input_Transfer(&input_buffer);
   
           std::size_t added_count       = 0;
           std::size_t open_failed_count = 0;
           for (const auto &i : evdev_targets) {
               auto searched = stored_dev.find(i.Name);
               if (searched == stored_dev.end()) {
                   continue;
               }
   
               const auto add_result = evdev_core->Add(searched->second.dev_path,
                                                       searched->second.dev_type,
                                                       i.Name);
               if (add_result.ok) {
                   ++added_count;
                   continue;
               }
               if (add_result.open_failed) {
                   ++open_failed_count;
               }
               warnlog("failed to add input device on linux:");
               warnlog(i.Name);
           }
   
           if (added_count > 0) {
               active_backend = ActiveBackendKind::Evdev;
               wayland_source_mode = WaylandSourceMode::None;
               if (wayland_core) {
                   wayland_core->Reset();
               }
               return true;
           }
   
           if (open_failed_count == 0) {
               warnlog("linux evdev config failed, and no /dev open failures were "
                       "detected. skipping wayland fallback.");
               return false;
           }
   
           return ConfigureWayland(evdev_targets);
       } catch (const std::exception &e) {
           critlog("failed on Device Configure on linux. What:");
           critlog(e.what());
           return false;
       }
   }
   
   std::vector<DeviceData>
   DefaultDevs::GetDevices()
   {
       DEV_LIST lsdev;
       fs::path device_root("/dev/input/");
       stored_dev.clear();
   
       try {
           for (const auto &dev : fs::directory_iterator(device_root)) {
               if (!dev.is_character_file()) {
                   continue;
               }
   
               const std::string dev_path = dev.path().string();
               if (dev_path.find("event") == std::string::npos) {
                   continue;
               }
   
               int FD = open(dev_path.c_str(), O_RDONLY | O_NONBLOCK);
   
               if (FD < 0) {
                   continue;
               }
               libevdev  *info = nullptr;
               DeviceData dd;
   
               if (libevdev_new_from_fd(FD, &info) == 0) {
                   const char *dev_name = libevdev_get_name(info);
                   if (dev_name) {
   
                       dd.Name               = std::string(dev_name);
                       dd.device_specific_id = dev.path().string();
                       stored_dev[dd.Name].dev_path      = dev.path();
                       stored_dev[dd.Name].source_id     = dd.device_specific_id;
                       stored_dev[dd.Name].backend_kind  = StoredBackendKind::Evdev;
   
                       if (libevdev_has_event_type(info, EV_KEY) &&
                           libevdev_has_event_code(info, EV_KEY, KEY_A) &&
                           libevdev_has_event_code(info, EV_KEY, KEY_SPACE) &&
                           libevdev_has_event_code(info, EV_KEY, KEY_ENTER)) {
                           dd.Type                      = PDJE_Dev_Type::KEYBOARD;
                           stored_dev[dd.Name].dev_type = PDJE_Dev_Type::KEYBOARD;
   
                       } else if (libevdev_has_event_type(info, EV_REL) &&
                                  libevdev_has_event_code(info, EV_REL, REL_X) &&
                                  libevdev_has_event_code(info, EV_REL, REL_Y)) {
                           dd.Type                      = PDJE_Dev_Type::MOUSE;
                           stored_dev[dd.Name].dev_type = PDJE_Dev_Type::MOUSE;
   
                       } else if (libevdev_has_event_type(info, EV_ABS) &&
                                  (libevdev_has_event_code(
                                       info, EV_KEY, BTN_GAMEPAD) ||
                                   libevdev_has_event_code(
                                       info, EV_KEY, BTN_JOYSTICK))) {
                           stored_dev[dd.Name].dev_type = PDJE_Dev_Type::UNKNOWN;
                           dd.Type                      = PDJE_Dev_Type::UNKNOWN;
   
                       } else if (libevdev_has_event_type(info, EV_ABS) &&
                                  libevdev_has_event_code(
                                      info, EV_KEY, BTN_TOUCH)) {
                           dd.Type                      = PDJE_Dev_Type::MOUSE;
                           stored_dev[dd.Name].dev_type = PDJE_Dev_Type::MOUSE;
   
                       } else {
                           dd.Type                      = PDJE_Dev_Type::UNKNOWN;
                           stored_dev[dd.Name].dev_type = PDJE_Dev_Type::UNKNOWN;
                       }
                       lsdev.push_back(dd);
                   }
                   libevdev_free(info);
                   close(FD);
               } else {
                   close(FD);
                   continue;
               }
           }
       } catch (const std::exception &e) {
           warnlog("linux evdev device scan failed; trying wayland fallback "
                   "discovery.");
           warnlog(e.what());
       }
   
       if (!lsdev.empty()) {
           return lsdev;
       }
   
       const bool has_host_handles = HasValidWaylandHostContext();
       if (!has_host_handles && !use_internal_window_) {
           return lsdev;
       }
       if (!wayland_loader.EnsureLoaded()) {
           return lsdev;
       }
   
       DeviceData kb;
       kb.Type               = PDJE_Dev_Type::KEYBOARD;
       kb.Name               = LINUX_INPUT_CONTRACTS::GetWaylandSyntheticName(
           kb.Type, !has_host_handles);
       kb.device_specific_id = LINUX_INPUT_CONTRACTS::kWaylandKeyboardId;
       stored_dev[kb.Name].backend_kind = StoredBackendKind::Wayland;
       stored_dev[kb.Name].source_id     = kb.device_specific_id;
       stored_dev[kb.Name].dev_type      = kb.Type;
       lsdev.push_back(kb);
   
       DeviceData mouse;
       mouse.Type               = PDJE_Dev_Type::MOUSE;
       mouse.Name               = LINUX_INPUT_CONTRACTS::GetWaylandSyntheticName(
           mouse.Type, !has_host_handles);
       mouse.device_specific_id = LINUX_INPUT_CONTRACTS::kWaylandPointerId;
       stored_dev[mouse.Name].backend_kind = StoredBackendKind::Wayland;
       stored_dev[mouse.Name].source_id     = mouse.device_specific_id;
       stored_dev[mouse.Name].dev_type      = mouse.Type;
       lsdev.push_back(mouse);
   
       return lsdev;
   }
   
   void
   DefaultDevs::RunLoop()
   {
       if (input_thread) {
           return;
       }
   
       Ready();
   
       switch (active_backend) {
       case ActiveBackendKind::Evdev:
           if (!evdev_core) {
               critlog("linux input backend selected evdev but core is missing.");
               return;
           }
           input_thread.emplace([this]() { evdev_core->Trig(); });
           return;
       case ActiveBackendKind::Wayland:
           if (!wayland_core) {
               critlog("linux input backend selected wayland but core is missing.");
               return;
           }
           input_thread.emplace([this]() { wayland_core->Trig(); });
           return;
       case ActiveBackendKind::None:
       default:
           warnlog("RunLoop called before linux input backend was configured.");
           return;
       }
   }
   
   void
   DefaultDevs::TerminateLoop()
   {
       try {
           if (!input_thread) {
               return;
           }
   
           if (active_backend == ActiveBackendKind::Evdev && evdev_core) {
               evdev_core->Stop();
           } else if (active_backend == ActiveBackendKind::Wayland &&
                      wayland_core) {
               wayland_core->Stop();
           }
   
           if (input_thread->joinable()) {
               input_thread->join();
           }
           input_thread.reset();
   
           if (active_backend == ActiveBackendKind::Evdev) {
               evdev_core.reset();
           } else if (active_backend == ActiveBackendKind::Wayland) {
               if (wayland_core) {
                   wayland_core->Reset();
               }
               wayland_core.reset();
           }
           wayland_source_mode = WaylandSourceMode::None;
       } catch (const std::exception &e) {
           critlog("input_thread join failed on linux. What: ");
           critlog(e.what());
           return;
       }
   }
   
   }; // namespace PDJE_DEFAULT_DEVICES
