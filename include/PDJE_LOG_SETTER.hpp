#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#endif
#include "quill/Logger.h"
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogFunctions.h"
#include "quill/Logger.h"
#include "quill/sinks/FileSink.h"
#include "quill/sinks/ConsoleSink.h"
#include <filesystem>

static quill::Logger* globalLogger;
inline
void startlog(){
    #ifndef LOG_OFF

    quill::BackendOptions bopts;
    
    bopts.sink_min_flush_interval = std::chrono::hours(24);
    bopts.sleep_duration = std::chrono::hours(24);
    
    quill::Backend::start(bopts);
    auto sinkP = quill::Frontend::create_or_get_sink<quill::FileSink>(
        "pdjeLog.log",
        [](){
            quill::FileSinkConfig cfg;
            cfg.set_open_mode('w');
            return cfg;
        }(),
        quill::FileEventNotifier{}
    );
    globalLogger = quill::Frontend::create_or_get_logger(
        "global",
        sinkP
    );
    #endif
}

inline
void stoplog(){
    #ifndef LOG_OFF
    if(globalLogger) globalLogger->flush_log();

    quill::Backend::stop();
    
    #endif
}



template<typename T>
inline 
void infolog(const T& msg){
    #ifndef NDEBUG
    quill::info(
        globalLogger,
        "Infolog {}\n",
        static_cast<std::string_view>(msg)
    );
    globalLogger->flush_log();

    #endif
}


template<typename T>
inline 
void warnlog(const T& msg){
    #ifndef NDEBUG
    quill::warning(
        globalLogger,
        "Warnlog {}\n",
        static_cast<std::string_view>(msg)
    );
    globalLogger->flush_log();

    #endif
}

template<typename T>
inline 
void critlog(const T& msg){
    #ifndef LOG_OFF
    quill::critical(
        globalLogger,
        "Critical {}\n",
        static_cast<std::string_view>(msg)
    );
    globalLogger->flush_log();
    #endif
}


template<>
inline 
void critlog<uint_least32_t>(const uint_least32_t& msg){
    #ifndef LOG_OFF

    quill::critical(
        globalLogger,
        "Critical {}\n",
        std::to_string(msg)
    );
    globalLogger->flush_log();
    #endif
}
template<>
inline 
void critlog<std::filesystem::path>(const std::filesystem::path& msg){
    #ifndef LOG_OFF
    quill::critical(
        globalLogger,
        "Critical {}\n",
        static_cast<std::string_view>(msg.generic_string())
    );
    globalLogger->flush_log();
    #endif
}