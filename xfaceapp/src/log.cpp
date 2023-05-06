#include <xfaceapp/log.h>

std::shared_ptr<spdlog::logger> log::logger;

void log::initLogger() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %v");

    // 使用basic_logger_mt
    logger = spdlog::daily_logger_mt<spdlog::async_factory>("daily_logger", "logs/daily.txt", 0, 0);

    logger->set_level(spdlog::level::debug);
}