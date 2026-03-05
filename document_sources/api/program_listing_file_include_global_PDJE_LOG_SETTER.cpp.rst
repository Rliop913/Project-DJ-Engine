
.. _program_listing_file_include_global_PDJE_LOG_SETTER.cpp:

Program Listing for File PDJE_LOG_SETTER.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_LOG_SETTER.cpp>` (``include\global\PDJE_LOG_SETTER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_LOG_SETTER.hpp"
   
   #include <spdlog/sinks/basic_file_sink.h>
   
   #include <cstdio>
   #include <filesystem>
   #include <memory>
   #include <mutex>
   #include <string>
   #include <utility>
   #include <vector>
   
   namespace {
   
   enum class PDJE_LogBackendKindInternal {
       kInternalSpdlog = 0,
       kHostCallback   = 1,
   };
   
   struct PDJE_LoggingState {
       std::mutex                      mutex;
       bool                            initialized          = false;
       PDJE_LogBackendKindInternal     backend_kind         =
           PDJE_LogBackendKindInternal::kInternalSpdlog;
       PDJE_LogHostSinkV1              host_sink            = {};
       int                             min_level            = PDJE_LOG_LEVEL_INFO_V1;
       bool                            autoinit_enabled     = true;
       bool                            internal_spdlog_owner = false;
       std::shared_ptr<spdlog::logger> logger;
       std::string                     file_path            = "logs/pdjeLog.txt";
   };
   
   constexpr const char *PDJE_LOGGER_NAME = "global_logger";
   
   PDJE_LoggingState &
   GetLoggingState()
   {
       static PDJE_LoggingState state;
       return state;
   }
   
   int
   DefaultMinLevel()
   {
   #ifndef NDEBUG
       return PDJE_LOG_LEVEL_DEBUG_V1;
   #else
       return PDJE_LOG_LEVEL_ERROR_V1;
   #endif
   }
   
   bool
   StrictExplicitInitEnabled()
   {
   #ifdef PDJE_LOG_STRICT_EXPLICIT_INIT
       return true;
   #else
       return false;
   #endif
   }
   
   int
   NormalizeLevel(const int level)
   {
       if (level < PDJE_LOG_LEVEL_TRACE_V1) {
           return PDJE_LOG_LEVEL_TRACE_V1;
       }
       if (level > PDJE_LOG_LEVEL_OFF_V1) {
           return PDJE_LOG_LEVEL_OFF_V1;
       }
       return level;
   }
   
   bool
   ShouldEmit(const int level, const int min_level)
   {
       return NormalizeLevel(level) >= NormalizeLevel(min_level) &&
              NormalizeLevel(level) < PDJE_LOG_LEVEL_OFF_V1;
   }
   
   spdlog::level::level_enum
   ToSpdlogLevel(const int level)
   {
       switch (NormalizeLevel(level)) {
       case PDJE_LOG_LEVEL_TRACE_V1:
           return spdlog::level::trace;
       case PDJE_LOG_LEVEL_DEBUG_V1:
           return spdlog::level::debug;
       case PDJE_LOG_LEVEL_INFO_V1:
           return spdlog::level::info;
       case PDJE_LOG_LEVEL_WARN_V1:
           return spdlog::level::warn;
       case PDJE_LOG_LEVEL_ERROR_V1:
           return spdlog::level::err;
       case PDJE_LOG_LEVEL_CRITICAL_V1:
           return spdlog::level::critical;
       case PDJE_LOG_LEVEL_OFF_V1:
       default:
           return spdlog::level::off;
       }
   }
   
   bool
   ConfigRequestsHostCallback(const PDJE_LogConfigV1 *cfg)
   {
       if (cfg == nullptr) {
           return false;
       }
       return cfg->backend == PDJE_LOG_BACKEND_HOST_CALLBACK_V1;
   }
   
   bool
   ConfigIsValid(const PDJE_LogConfigV1 *cfg)
   {
       if (cfg == nullptr) {
           return true;
       }
   
       if (cfg->struct_size != 0 && cfg->struct_size < sizeof(PDJE_LogConfigV1)) {
           return false;
       }
   
       if (cfg->backend != PDJE_LOG_BACKEND_INTERNAL_SPDLOG_V1 &&
           cfg->backend != PDJE_LOG_BACKEND_HOST_CALLBACK_V1) {
           return false;
       }
   
       if (ConfigRequestsHostCallback(cfg)) {
           if (cfg->host_sink.write == nullptr) {
               return false;
           }
           if (cfg->host_sink.struct_size != 0 &&
               cfg->host_sink.struct_size < sizeof(PDJE_LogHostSinkV1)) {
               return false;
           }
       }
   
       return true;
   }
   
   PDJE_LogInitResultV1
   InitInternalSpdlogLocked(PDJE_LoggingState      &state,
                            const PDJE_LogConfigV1 *cfg) noexcept
   {
       try {
           std::string log_path = "logs/pdjeLog.txt";
           if (cfg != nullptr && cfg->file_path != nullptr && cfg->file_path[0] != '\0') {
               log_path = cfg->file_path;
           }
   
           std::filesystem::path path(log_path);
           const auto            parent = path.parent_path();
           if (!parent.empty()) {
               std::filesystem::create_directories(parent);
           }
   
           spdlog::drop(PDJE_LOGGER_NAME);
           auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
               path.string(), false);
           std::vector<spdlog::sink_ptr> sinks{ file_sink };
   
           auto logger = std::make_shared<spdlog::logger>(
               PDJE_LOGGER_NAME, sinks.begin(), sinks.end());
           logger->set_level(ToSpdlogLevel(state.min_level));
           logger->flush_on(spdlog::level::err);
           logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
   
           // Compatibility: keep the default logger path available for legacy code
           // that still calls spdlog directly.
           spdlog::set_default_logger(logger);
   
           state.logger                = std::move(logger);
           state.file_path             = path.string();
           state.backend_kind          = PDJE_LogBackendKindInternal::kInternalSpdlog;
           state.internal_spdlog_owner = true;
           state.initialized           = true;
           return PDJE_LOG_INIT_OK_V1;
       } catch (...) {
           return PDJE_LOG_INIT_FAILED_V1;
       }
   }
   
   PDJE_LogInitResultV1
   InitHostCallbackLocked(PDJE_LoggingState      &state,
                          const PDJE_LogConfigV1 *cfg) noexcept
   {
       if (cfg == nullptr || cfg->host_sink.write == nullptr) {
           return PDJE_LOG_INIT_INVALID_ARGUMENT_V1;
       }
   
       state.host_sink               = cfg->host_sink;
       state.logger.reset();
       state.backend_kind            = PDJE_LogBackendKindInternal::kHostCallback;
       state.internal_spdlog_owner   = false;
       state.initialized             = true;
       return PDJE_LOG_INIT_OK_V1;
   }
   
   PDJE_LogInitResultV1
   InitLocked(PDJE_LoggingState &state, const PDJE_LogConfigV1 *cfg) noexcept
   {
       state.min_level = DefaultMinLevel();
       if (cfg != nullptr) {
           state.min_level = NormalizeLevel(cfg->min_level);
       }
       if (cfg != nullptr &&
           (cfg->flags & PDJE_LOG_CFG_FLAG_DISABLE_AUTOINIT_V1) != 0u) {
           state.autoinit_enabled = false;
       } else {
           state.autoinit_enabled = !StrictExplicitInitEnabled();
       }
   
       if (ConfigRequestsHostCallback(cfg)) {
           return InitHostCallbackLocked(state, cfg);
       }
   
       return InitInternalSpdlogLocked(state, cfg);
   }
   
   void
   ResetStateLocked(PDJE_LoggingState &state)
   {
       state.initialized = false;
       state.backend_kind = PDJE_LogBackendKindInternal::kInternalSpdlog;
       state.host_sink = {};
       state.min_level = DefaultMinLevel();
       state.autoinit_enabled = !StrictExplicitInitEnabled();
       state.internal_spdlog_owner = false;
       state.logger.reset();
       state.file_path = "logs/pdjeLog.txt";
   }
   
   struct PDJE_LogDispatchSnapshot {
       bool                            initialized    = false;
       PDJE_LogBackendKindInternal     backend_kind   =
           PDJE_LogBackendKindInternal::kInternalSpdlog;
       int                             min_level      = PDJE_LOG_LEVEL_INFO_V1;
       bool                            autoinit       = true;
       PDJE_LogHostSinkV1              host_sink      = {};
       std::shared_ptr<spdlog::logger> logger;
   };
   
   PDJE_LogDispatchSnapshot
   CaptureSnapshot()
   {
       auto &state = GetLoggingState();
       std::lock_guard<std::mutex> guard(state.mutex);
   
       PDJE_LogDispatchSnapshot snapshot;
       snapshot.initialized = state.initialized;
       snapshot.backend_kind = state.backend_kind;
       if (state.initialized) {
           snapshot.min_level = state.min_level;
           snapshot.autoinit = state.autoinit_enabled;
       } else {
           snapshot.min_level = DefaultMinLevel();
           snapshot.autoinit = !StrictExplicitInitEnabled();
       }
       snapshot.host_sink = state.host_sink;
       snapshot.logger = state.logger;
       return snapshot;
   }
   
   void
   EmitStrictInitWarningOnce() noexcept
   {
       static std::mutex warn_mutex;
       static bool       warned = false;
   
       std::lock_guard<std::mutex> guard(warn_mutex);
       if (warned) {
           return;
       }
       warned = true;
       std::fprintf(stderr,
                    "[PDJE] logging dropped: runtime not initialized and strict "
                    "explicit init is enabled\n");
   }
   
   void
   DispatchLog(const PDJE_LogDispatchSnapshot &snapshot,
               const int                      level,
               const char                    *message,
               const size_t                   message_len) noexcept
   {
       if (!snapshot.initialized) {
           return;
       }
   
       if (!ShouldEmit(level, snapshot.min_level)) {
           return;
       }
   
       const char  *safe_message = message != nullptr ? message : "";
       const size_t safe_len =
           (safe_message == message) ? message_len : static_cast<size_t>(0);
   
       try {
           if (snapshot.backend_kind == PDJE_LogBackendKindInternal::kHostCallback) {
               if (snapshot.host_sink.write != nullptr) {
                   snapshot.host_sink.write(
                       NormalizeLevel(level),
                       safe_message,
                       safe_len,
                       snapshot.host_sink.user_data);
               }
               return;
           }
   
           if (snapshot.logger != nullptr) {
               snapshot.logger->log(
                   ToSpdlogLevel(level), spdlog::string_view_t(safe_message, safe_len));
           }
       } catch (...) {
           // Never throw across log dispatch boundaries.
       }
   }
   
   } // namespace
   
   extern "C" PDJE_API int PDJE_CALL
   pdje_logging_init_v1(const PDJE_LogConfigV1 *cfg)
   {
   #ifdef LOG_OFF
       (void)cfg;
       return PDJE_LOG_INIT_OK_V1;
   #else
       try {
           if (!ConfigIsValid(cfg)) {
               return PDJE_LOG_INIT_INVALID_ARGUMENT_V1;
           }
   
           auto &state = GetLoggingState();
           std::lock_guard<std::mutex> guard(state.mutex);
           if (state.initialized) {
               return PDJE_LOG_INIT_ALREADY_INITIALIZED_V1;
           }
           return InitLocked(state, cfg);
       } catch (...) {
           return PDJE_LOG_INIT_FAILED_V1;
       }
   #endif
   }
   
   extern "C" PDJE_API int PDJE_CALL
   pdje_logging_shutdown_v1(void)
   {
   #ifdef LOG_OFF
       return PDJE_LOG_SHUTDOWN_OK_V1;
   #else
       try {
           bool had_internal_owner = false;
           {
               auto &state = GetLoggingState();
               std::lock_guard<std::mutex> guard(state.mutex);
               if (!state.initialized) {
                   return PDJE_LOG_SHUTDOWN_NOT_INITIALIZED_V1;
               }
               had_internal_owner = state.internal_spdlog_owner;
               ResetStateLocked(state);
           }
   
           if (had_internal_owner) {
               try {
                   spdlog::drop(PDJE_LOGGER_NAME);
               } catch (...) {
               }
           }
           return PDJE_LOG_SHUTDOWN_OK_V1;
       } catch (...) {
           return PDJE_LOG_SHUTDOWN_FAILED_V1;
       }
   #endif
   }
   
   extern "C" PDJE_API int PDJE_CALL
   pdje_logging_is_initialized_v1(void)
   {
   #ifdef LOG_OFF
       return 0;
   #else
       auto &state = GetLoggingState();
       std::lock_guard<std::mutex> guard(state.mutex);
       return state.initialized ? 1 : 0;
   #endif
   }
   
   extern "C" PDJE_API void PDJE_CALL
   pdje_log_write_v1(int level, const char *message, size_t message_len)
   {
   #ifdef LOG_OFF
       (void)level;
       (void)message;
       (void)message_len;
       return;
   #else
       auto snapshot = CaptureSnapshot();
       if (!snapshot.initialized) {
           if (snapshot.autoinit) {
               (void)pdje_logging_init_v1(nullptr);
               snapshot = CaptureSnapshot();
           } else {
               EmitStrictInitWarningOnce();
               return;
           }
       }
   
       DispatchLog(snapshot, level, message, message_len);
   #endif
   }
   
   void
   startlog()
   {
       (void)pdje_logging_init_v1(nullptr);
   }
   
   void
   shutdownlog()
   {
       (void)pdje_logging_shutdown_v1();
   }
