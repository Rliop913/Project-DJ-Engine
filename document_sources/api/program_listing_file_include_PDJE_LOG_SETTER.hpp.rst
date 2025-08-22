
.. _program_listing_file_include_PDJE_LOG_SETTER.hpp:

Program Listing for File PDJE_LOG_SETTER.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_PDJE_LOG_SETTER.hpp>` (``include\PDJE_LOG_SETTER.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include <spdlog/spdlog.h>
   #include <spdlog/sinks/basic_file_sink.h>
   #include <spdlog/sinks/stdout_color_sinks.h>
   #include <filesystem>
   #include <type_traits>
   #include <string_view>
   
   inline
   void startlog(){
       #ifndef LOG_OFF
       std::filesystem::create_directories("logs");
       auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/pdjeLog.txt", true);
       auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
   
       std::vector<spdlog::sink_ptr> sinks {consoleSink, fileSink};
   
       auto logger = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
       #ifndef NDEBUG
       logger->set_level(spdlog::level::debug);
       #else
       logger->set_level(spdlog::level::err);
       #endif
       logger->flush_on(spdlog::level::err);
       logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
       spdlog::set_default_logger(logger);
   
       
       #endif
   }
   
   #ifndef LOG_OFF
       #ifdef ENABLE_INFO_LOG
       #define infolog(...)        SPDLOG_INFO(__VA_ARGS__)
       #else
       #define infolog(...)
       #endif
   
       #ifdef ENABLE_WARN_LOG
       #define warnlog(...)        SPDLOG_WARN(__VA_ARGS__)
       #else
       #define warnlog(...)
       #endif
   
   #define critlog(...)    SPDLOG_CRITICAL(__VA_ARGS__)
   #else
   #define infolog(...)
   #define warnlog(...)
   #define critlog(...)
   #endif
