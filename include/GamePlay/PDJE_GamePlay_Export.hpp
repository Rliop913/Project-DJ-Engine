#pragma once

// Gameplay exports its own facade while importing the native module APIs.
#if defined(PDJE_GAMEPLAY_SHARED)
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(PDJE_GAMEPLAY_BUILDING)
#define PDJE_GAMEPLAY_API __declspec(dllexport)
#else
#define PDJE_GAMEPLAY_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define PDJE_GAMEPLAY_API __attribute__((visibility("default")))
#else
#define PDJE_GAMEPLAY_API
#endif
#else
#define PDJE_GAMEPLAY_API
#endif
