/*
 * @Description: 
 * @Date: 2023-05-03 07:25:15
 * @LastEditTime: 2023-05-09 03:20:49
 * @FilePath: /XFaceSDK/xfaceapp/include/xfaceapp/log.h
 */
#pragma once

#include <spdlog/spdlog.h>
#include "spdlog/async.h"
#include <spdlog/sinks/daily_file_sink.h>


class log {
public:
    static void initLogger();

    static std::shared_ptr<spdlog::logger> getLogger() {return logger;}

private:
    static std::shared_ptr<spdlog::logger> logger;
};

// log macros
#define LOG_TRACE(...)       log::getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)       log::getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)        log::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)        log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)       log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)    log::getLogger()->critical(__VA_ARGS__)
