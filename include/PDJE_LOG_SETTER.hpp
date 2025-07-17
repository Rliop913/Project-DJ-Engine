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

#define infolog(...)        SPDLOG_INFO(__VA_ARGS__)
#define warnlog(...)        SPDLOG_WARN(__VA_ARGS__)
#define criticallog(...)    SPDLOG_CRITICAL(__VA_ARGS__)

// inline
// void stoplog(){
//     #ifndef LOG_OFF
//     spdlog::shutdown();
    
//     #endif
// }


// // inline 
// // void infolog(const std::string& msg){
// //     #ifndef NDEBUG
// //     quill::info(
// //         globalLogger,
// //         "Infolog {}\n",
// //         msg
// //     );
// //     globalLogger->flush_log();

// //     #endif
// // }

// #define infolog(MSG){\
//     LOGV_INFO(globalLogger, MSG);\
//     globalLogger->flush_log();\
// }

// #define Vinfolog(MSG, V){\
//     LOGV_INFO(globalLogger, MSG, V);\
//     globalLogger->flush_log();\
// }

// inline 
// void infolog(const std::filesystem::path& msg){
//     #ifndef NDEBUG
//     std::string logPath = msg.generic_string();
//     quill::info(
//         globalLogger,
//         "Infolog {}\n",
//         std::string_view(logPath)
//     );
//     globalLogger->flush_log();

//     #endif
// }

// template<toStringAbles T>
// inline 
// void infolog(const T& msg){
//     #ifndef NDEBUG
//     quill::info(
//         globalLogger,
//         "Infolog {}\n",
//         std::string_view(std::to_string(msg))
//     );
//     globalLogger->flush_log();

//     #endif
// }


// inline 
// void warnlog(const std::string& msg){
//     #ifndef NDEBUG
//     quill::warning(
//         globalLogger,
//         "Warnlog {}\n",
//         std::string_view(msg)
//     );
//     globalLogger->flush_log();

//     #endif
// }


// // inline 
// // void warnlog(const std::filesystem::path& msg){
// //     #ifndef NDEBUG
// //     std::string logpath = msg.generic_string();
// //     quill::warning(
// //         globalLogger,
// //         "Warnlog {}\n",
// //         std::string_view(logpath)
// //     );
// //     globalLogger->flush_log();

// //     #endif
// // }


// // template<toStringAbles T>
// // inline 
// // void warnlog(const T& msg){
// //     #ifndef NDEBUG
// //     quill::warning(
// //         globalLogger,
// //         "Warnlog {}\n",
// //         std::string_view(std::to_string(msg))
// //     );
// //     globalLogger->flush_log();

// //     #endif
// // }


// inline void
// critlog(const std::string& msg){
//     #ifndef LOG_OFF
//     quill::critical(
//         globalLogger,
//         "Critical {}\n",
//         std::string_view(msg)
//     );
//     globalLogger->flush_log();
//     #endif
// }



// inline void
// critlog(const T& msg){
//     #ifndef LOG_OFF

//     quill::critical(
//         globalLogger,
//         "Critical {}\n",
//         std::string_view(std::to_string(msg))
//     );
//     globalLogger->flush_log();
//     #endif
// }

// inline void
// critlog(const std::filesystem::path& msg){
//     #ifndef LOG_OFF
//     std::string logpath = msg.generic_string();
//     quill::critical(
//         globalLogger,
//         "Critical {}\n",
//         std::string_view(logpath)
//     );
//     globalLogger->flush_log();
//     #endif
// }