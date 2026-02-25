/**
 * @file PDJE_LOG_SETTER.hpp
 * @brief Configures the logging framework for the application.
 */
#pragma once

#include "PDJE_EXPORT_SETTER.hpp"

#include <filesystem>
#include <mutex>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string_view>
#include <type_traits>

/**
 * @brief Initializes the logging system.
 *
 * This function sets up a global logger with both a file sink and a console
 * sink. The log level is set to `debug` in debug builds and `err` in release
 * builds.
 *
 * Host applications should call this explicitly during startup before creating
 * exported runtime objects when loading multiple DSOs.
 */
PDJE_API void
startlog();

#ifndef LOG_OFF
#ifdef ENABLE_INFO_LOG
/**
 * @def infolog(...)
 * @brief Logs an informational message.
 */
#define infolog(...) SPDLOG_INFO(__VA_ARGS__)
#else
#define infolog(...)
#endif

#ifdef ENABLE_WARN_LOG
/**
 * @def warnlog(...)
 * @brief Logs a warning message.
 */
#define warnlog(...) SPDLOG_WARN(__VA_ARGS__)
#else
#define warnlog(...)
#endif

/**
 * @def critlog(...)
 * @brief Logs a critical message.
 */
#define critlog(...) SPDLOG_CRITICAL(__VA_ARGS__)
#else
#define infolog(...)
#define warnlog(...)
#define critlog(...)
#endif
