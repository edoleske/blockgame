#ifndef BLOCKGAME_LOG_H
#define BLOCKGAME_LOG_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "common.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"


class Log {
public:
    static Log& instance() {
        static Log l;
        return l;
    }

    shared_ptr<spdlog::logger>& getLogger() {
        return logger;
    }

    // Delete copy/move/assignment
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

private:
    Log() {
        spdlog::init_thread_pool(8192, 1);

        spdlog::set_pattern("%^[%T] [%L]: %v%$");
        logger = spdlog::stdout_color_mt<spdlog::async_factory>("BLOCKGAME");

#if defined(DEBUG)
        logger->set_level(spdlog::level::trace);
#else
        logger->set_level(spdlog::level::info);
#endif
    }

    ~Log() {
        spdlog::shutdown();
    };

    shared_ptr<spdlog::logger> logger;
};

#define LOG_TRACE(...) Log::instance().getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Log::instance().getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) Log::instance().getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Log::instance().getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::instance().getLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) Log::instance().getLogger()->critical(__VA_ARGS__)

#endif //BLOCKGAME_LOG_H
