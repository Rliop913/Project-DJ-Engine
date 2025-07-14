#pragma once
#include <NanoLog.hpp>
template<typename T>
inline 
void infolog(const T& msg){
    #ifndef NDEBUG
    LOG_INFO << msg;
    #endif
}
template<typename T>
inline 
void warnlog(const T& msg){
    #ifndef NDEBUG
    LOG_WARN << msg;
    #endif
}

template<typename T>
inline 
void critlog(const T& msg){
    #ifndef NANOLOG_OFF
    LOG_CRIT << msg;
    #endif
}

// #ifdef NANOLOG_OFF\
// \
// #elif defined(NDEBUG)\
//     N\
// #else\
//     N\
// #endif