#pragma once
#include <NanoLog.hpp>
#include <filesystem>
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

template<>
inline 
void critlog<std::filesystem::path>(const std::filesystem::path& msg){
    #ifndef NANOLOG_OFF
    
    LOG_CRIT << msg.generic_string();
    #endif
}
// #ifdef NANOLOG_OFF\
// \
// #elif defined(NDEBUG)\
//     N\
// #else\
//     N\
// #endif