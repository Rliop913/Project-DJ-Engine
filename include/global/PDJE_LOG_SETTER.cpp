#include "PDJE_LOG_SETTER.hpp"

void
startlog()
{
    static std::once_flag SPD_LOG_ONCE_FLAG;
#ifndef LOG_OFF
    std::call_once(SPD_LOG_ONCE_FLAG, []() {
        std::filesystem::create_directories("logs");
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            "logs/pdjeLog.txt", false);

        std::vector<spdlog::sink_ptr> sinks{ fileSink };

        auto logger = std::make_shared<spdlog::logger>(
            "global_logger", sinks.begin(), sinks.end());

#ifndef NDEBUG
        logger->set_level(spdlog::level::debug);
#else
        logger->set_level(spdlog::level::err);
#endif
        logger->flush_on(spdlog::level::err);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        spdlog::set_default_logger(logger);
    });
#endif
}
