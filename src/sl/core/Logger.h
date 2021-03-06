#pragma once

#include <memory>
#include <type_traits>

#include <signal.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

namespace sl::core {

extern void initLogging();
}

#define SL_FATAL(...) SPDLOG_CRITICAL(__VA_ARGS__);
#define SL_ERROR(...) SPDLOG_ERROR(__VA_ARGS__);
#define SL_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define SL_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define SL_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define SL_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__);

#define SL_ASSERT(x, ...)                             \
    if (!(x)) {                                       \
        SL_FATAL(__VA_ARGS__);                        \
        throw std::runtime_error("Assertion failed"); \
    }
