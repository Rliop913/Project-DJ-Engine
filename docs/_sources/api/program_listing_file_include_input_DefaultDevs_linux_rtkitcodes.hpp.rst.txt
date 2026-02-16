
.. _program_listing_file_include_input_DefaultDevs_linux_rtkitcodes.hpp:

Program Listing for File rtkitcodes.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_rtkitcodes.hpp>` (``include/input/DefaultDevs/linux/rtkitcodes.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <cstdint>
   #include <stdexcept>
   #include <string>
   
   #include <sys/syscall.h>
   #include <unistd.h>
   
   #include <systemd/sd-bus.h>
   
   namespace rtkit {
   
   //AI generated -> how to use acl
   // 1) 네, 글로브로 한 번에 줄 수 있어
   
   // 쉘이 event*를 실제 파일 목록으로 확장해줘서 아래처럼 동작해.
   
   // sudo setfacl -m u:$USER:r /dev/input/event*
   
   
   // 그리고 해제도 동일하게:
   
   // sudo setfacl -x u:$USER /dev/input/event*
   
   
   // AI GENERATED... todo - fix & modulize these code
   
   static inline uint64_t
   get_tid()
   {
       return static_cast<uint64_t>(syscall(SYS_gettid));
   }
   
   static inline void
   throw_bus_err(const char *what, int r, sd_bus_error *err)
   {
       std::string msg = what;
       msg += " (";
       msg += std::to_string(r);
       msg += ")";
       if (err && sd_bus_error_is_set(err)) {
           msg += ": ";
           msg += err->message ? err->message : "(no message)";
       }
       throw std::runtime_error(msg);
   }
   
   // Query helpers (optional)
   static inline uint32_t
   get_max_realtime_priority(sd_bus *bus)
   {
       sd_bus_error    err   = SD_BUS_ERROR_NULL;
       sd_bus_message *reply = nullptr;
   
       int r = sd_bus_call_method(bus,
                                  "org.freedesktop.RealtimeKit1",
                                  "/org/freedesktop/RealtimeKit1",
                                  "org.freedesktop.RealtimeKit1",
                                  "GetMaxRealtimePriority",
                                  &err,
                                  &reply,
                                  "");
       if (r < 0) {
           sd_bus_error_free(&err);
           throw_bus_err("GetMaxRealtimePriority failed", r, &err);
       }
   
       uint32_t prio = 0;
       r             = sd_bus_message_read(reply, "u", &prio);
       sd_bus_message_unref(reply);
       if (r < 0)
           throw std::runtime_error(
               "Failed to parse GetMaxRealtimePriority reply");
       return prio;
   }
   
   static inline int32_t
   get_min_nice_level(sd_bus *bus)
   {
       sd_bus_error    err   = SD_BUS_ERROR_NULL;
       sd_bus_message *reply = nullptr;
   
       int r = sd_bus_call_method(bus,
                                  "org.freedesktop.RealtimeKit1",
                                  "/org/freedesktop/RealtimeKit1",
                                  "org.freedesktop.RealtimeKit1",
                                  "GetMinNiceLevel",
                                  &err,
                                  &reply,
                                  "");
       if (r < 0) {
           sd_bus_error_free(&err);
           throw_bus_err("GetMinNiceLevel failed", r, &err);
       }
   
       int32_t nicev = 0;
       r             = sd_bus_message_read(reply, "i", &nicev);
       sd_bus_message_unref(reply);
       if (r < 0)
           throw std::runtime_error("Failed to parse GetMinNiceLevel reply");
       return nicev;
   }
   
   // Make current thread SCHED_FIFO realtime with given priority (1..99, but rtkit
   // caps it)
   static inline void
   make_current_thread_realtime(uint32_t priority)
   {
       sd_bus         *bus   = nullptr;
       sd_bus_error    err   = SD_BUS_ERROR_NULL;
       sd_bus_message *reply = nullptr;
   
       int r = sd_bus_open_system(&bus);
       if (r < 0)
           throw std::runtime_error("sd_bus_open_system failed: " +
                                    std::to_string(r));
   
       const uint64_t tid = get_tid();
   
       // (optional) clamp to rtkit max
       uint32_t max_prio = 0;
       try {
           max_prio = get_max_realtime_priority(bus);
           if (priority > max_prio)
               priority = max_prio;
           if (priority == 0)
               priority = 1;
       } catch (...) {
           // If query fails, still try the call with provided priority
       }
   
       r = sd_bus_call_method(
           bus,
           "org.freedesktop.RealtimeKit1",
           "/org/freedesktop/RealtimeKit1",
           "org.freedesktop.RealtimeKit1",
           "MakeThreadRealtime",
           &err,
           &reply,
           "tu", // t = uint64 (thread id), u = uint32 (priority)
           tid,
           priority);
   
       if (r < 0) {
           sd_bus_error_free(&err);
           sd_bus_unref(bus);
           throw_bus_err("MakeThreadRealtime failed", r, &err);
       }
   
       sd_bus_message_unref(reply);
       sd_bus_unref(bus);
   }
   
   // Make current thread "high priority" by lowering nice (negative is higher
   // priority). rtkit enforces a minimum nice level; values below that will be
   // denied.
   static inline void
   make_current_thread_high_priority(int32_t nice_level)
   {
       sd_bus         *bus   = nullptr;
       sd_bus_error    err   = SD_BUS_ERROR_NULL;
       sd_bus_message *reply = nullptr;
   
       int r = sd_bus_open_system(&bus);
       if (r < 0)
           throw std::runtime_error("sd_bus_open_system failed: " +
                                    std::to_string(r));
   
       const uint64_t tid = get_tid();
   
       // (optional) clamp to rtkit min nice
       try {
           int32_t min_nice = get_min_nice_level(bus); // e.g. -11
           if (nice_level < min_nice)
               nice_level = min_nice;
       } catch (...) {
           // ignore
       }
   
       r = sd_bus_call_method(
           bus,
           "org.freedesktop.RealtimeKit1",
           "/org/freedesktop/RealtimeKit1",
           "org.freedesktop.RealtimeKit1",
           "MakeThreadHighPriority",
           &err,
           &reply,
           "ti", // t = uint64 (thread id), i = int32 (nice level)
           tid,
           nice_level);
   
       if (r < 0) {
           sd_bus_error_free(&err);
           sd_bus_unref(bus);
           throw_bus_err("MakeThreadHighPriority failed", r, &err);
       }
   
       sd_bus_message_unref(reply);
       sd_bus_unref(bus);
   }
   
   } // namespace rtkit
