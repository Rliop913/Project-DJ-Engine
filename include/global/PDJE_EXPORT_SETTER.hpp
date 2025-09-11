/**
 * @file PDJE_EXPORT_SETTER.hpp
 * @brief Defines the PDJE_API macro for DLL import/export.
 */
#pragma once

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
 * @brief Defined as empty for non-Windows platforms.
 */
#define PDJE_API
#endif