/**
 * @file PDJE_EXPORT_SETTER.hpp
 * @brief Defines the PDJE_API macro for DLL import/export.
 */
#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#define PDJE_CALL __cdecl
#else
#define PDJE_CALL
#endif

#ifdef PDJE_WINDOWS_DLL
#ifdef PDJE_BUILDING
/**
 * @def PDJE_API
 * @brief Exports symbols from the DLL when building the project.
 */
#define PDJE_API __declspec(dllexport)
#else
/**
 * @def PDJE_API
 * @brief Imports symbols from the DLL when using the project.
 */
#define PDJE_API __declspec(dllimport)
#endif
#else
/**
 * @def PDJE_API
 * @brief Exports symbols on GCC/Clang and is empty otherwise.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PDJE_API __attribute__((visibility("default")))
#else
#define PDJE_API
#endif
#endif
