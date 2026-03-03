
.. _program_listing_file_include_global_PDJE_LOG_SETTER.hpp:

Program Listing for File PDJE_LOG_SETTER.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_LOG_SETTER.hpp>` (``include/global/PDJE_LOG_SETTER.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_LOG_RUNTIME_API.h"
   
   #include <spdlog/fmt/fmt.h>
   #include <spdlog/spdlog.h>
   #include <exception>
   #include <string_view>
   #include <utility>
   
   PDJE_API void
   startlog();
   
   PDJE_API void
   shutdownlog();
   
   #ifndef LOG_OFF
   namespace PDJE_LOG_DETAIL {
   
   inline void
   log_preformatted(const int level, std::string_view message) noexcept
   {
       pdje_log_write_v1(level, message.data(), message.size());
   }
   
   template <typename T> inline void
   log_value(const int level, T &&value) noexcept
   {
       try {
           auto formatted = spdlog::fmt_lib::format("{}", std::forward<T>(value));
           pdje_log_write_v1(level, formatted.data(), formatted.size());
       } catch (const std::exception &e) {
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1,
                            "pdje logging format error in log_value");
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1, e.what());
       } catch (...) {
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1,
                            "pdje logging unknown format error in log_value");
       }
   }
   
   template <typename... Args>
   inline void
   log_format(const int level,
              spdlog::format_string_t<Args...> fmt,
              Args &&...args) noexcept
       requires(sizeof...(Args) > 0)
   {
       try {
           auto formatted = spdlog::fmt_lib::format(fmt, std::forward<Args>(args)...);
           pdje_log_write_v1(level, formatted.data(), formatted.size());
       } catch (const std::exception &e) {
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1,
                            "pdje logging format error in log_format");
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1, e.what());
       } catch (...) {
           log_preformatted(PDJE_LOG_LEVEL_ERROR_V1,
                            "pdje logging unknown format error in log_format");
       }
   }
   
   template <typename T> inline void
   info(T &&value) noexcept
   {
       log_value(PDJE_LOG_LEVEL_INFO_V1, std::forward<T>(value));
   }
   
   template <typename... Args>
   inline void
   info(spdlog::format_string_t<Args...> fmt, Args &&...args) noexcept
       requires(sizeof...(Args) > 0)
   {
       log_format(PDJE_LOG_LEVEL_INFO_V1, fmt, std::forward<Args>(args)...);
   }
   
   template <typename T> inline void
   warn(T &&value) noexcept
   {
       log_value(PDJE_LOG_LEVEL_WARN_V1, std::forward<T>(value));
   }
   
   template <typename... Args>
   inline void
   warn(spdlog::format_string_t<Args...> fmt, Args &&...args) noexcept
       requires(sizeof...(Args) > 0)
   {
       log_format(PDJE_LOG_LEVEL_WARN_V1, fmt, std::forward<Args>(args)...);
   }
   
   template <typename T> inline void
   critical(T &&value) noexcept
   {
       log_value(PDJE_LOG_LEVEL_CRITICAL_V1, std::forward<T>(value));
   }
   
   template <typename... Args>
   inline void
   critical(spdlog::format_string_t<Args...> fmt, Args &&...args) noexcept
       requires(sizeof...(Args) > 0)
   {
       log_format(PDJE_LOG_LEVEL_CRITICAL_V1, fmt, std::forward<Args>(args)...);
   }
   
   } // namespace PDJE_LOG_DETAIL
   
   #ifdef ENABLE_INFO_LOG
   #define infolog(...) PDJE_LOG_DETAIL::info(__VA_ARGS__)
   #else
   #define infolog(...)
   #endif
   
   #ifdef ENABLE_WARN_LOG
   #define warnlog(...) PDJE_LOG_DETAIL::warn(__VA_ARGS__)
   #else
   #define warnlog(...)
   #endif
   
   #define critlog(...) PDJE_LOG_DETAIL::critical(__VA_ARGS__)
   #else
   #define infolog(...)
   #define warnlog(...)
   #define critlog(...)
   #endif
