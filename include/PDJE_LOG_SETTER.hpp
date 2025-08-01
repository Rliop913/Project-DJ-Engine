/**
 * @file PDJE_LOG_SETTER.hpp
 * @brief Configures the logging framework for the application.
 */
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>
#include <type_traits>
#include <string_view>

/**
 * @brief Initializes the logging system.
 *
 * This function sets up a global logger with both a file sink and a console sink.
 * The log level is set to `debug` in debug builds and `err` in release builds.
 */
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
    /**
     * @def infolog(...)
     * @brief Logs an informational message.
     */
    #define infolog(...)        SPDLOG_INFO(__VA_ARGS__)
    #else
    #define infolog(...)
    #endif

    #ifdef ENABLE_WARN_LOG
    /**
     * @def warnlog(...)
     * @brief Logs a warning message.
     */
    #define warnlog(...)        SPDLOG_WARN(__VA_ARGS__)
    #else
    #define warnlog(...)
    #endif

/**
 * @def critlog(...)
 * @brief Logs a critical message.
 */
#define critlog(...)    SPDLOG_CRITICAL(__VA_ARGS__)
#else
#define infolog(...)
#define warnlog(...)
#define critlog(...)
#endif
