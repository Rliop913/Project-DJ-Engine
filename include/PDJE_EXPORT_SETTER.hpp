#pragma once

#ifdef PDJE_WINDOWS_DLL
    #ifdef PDJE_BUILDING
        #define PDJE_API __declspec(dllexport)
    #else
        #define PDJE_API __declspec(dllimport)
    #endif
#else
#define PDJE_API
#endif