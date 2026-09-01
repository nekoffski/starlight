#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

#include <map>
#include <source_location>
#include <string_view>

#include "Core.hh"
#include "Error.hh"

namespace sl::log {

enum class Level {
    off,
    critical,
    error,
    warn,
    info,
    debug,
    trace,
};

const std::map<std::string, Level>& levelMap();

Level levelFromString(const Str& levelStr);
Str levelToString(Level level);

namespace details {

struct FormatWithLocation {
    std::string_view fmt;
    spdlog::source_loc loc;

    template <typename String>
        requires std::constructible_from<std::string_view, String>
    FormatWithLocation(
        const String& s,
        const std::source_location& location = std::source_location::current()
    )
        : fmt(s),
          loc(location.file_name(), static_cast<int>(location.line()),
              location.function_name()) {}
};

template <typename... Args>
[[noreturn]] constexpr inline void abort(
    const FormatWithLocation& formatString, Args&&... args
) {
    fmt::println(fmt::runtime(formatString.fmt), std::forward<Args>(args)...);
    std::abort();
}
}  // namespace details

struct LoggerOptions {
    bool enableColors{false};
    std::string formatPattern{
        "%^[%d-%m-%Y %T] [Th: %t] %-7l [starlight]: %v %$"
    };
};

void init(const LoggerOptions& options = LoggerOptions{});
void setLogLevel(Level level);
Level level();

template <typename... Args>
void debug(details::FormatWithLocation fmt, Args&&... args) {
    spdlog::default_logger_raw()->log(
        fmt.loc, spdlog::level::debug, fmt::runtime(fmt.fmt),
        std::forward<Args>(args)...
    );
}

template <typename... Args>
void error(details::FormatWithLocation fmt, Args&&... args) {
    spdlog::default_logger_raw()->log(
        fmt.loc, spdlog::level::err, fmt::runtime(fmt.fmt),
        std::forward<Args>(args)...
    );
}

template <typename... Args>
void info(details::FormatWithLocation fmt, Args&&... args) {
    spdlog::default_logger_raw()->log(
        fmt.loc, spdlog::level::info, fmt::runtime(fmt.fmt),
        std::forward<Args>(args)...
    );
}

template <typename... Args>
void trace(details::FormatWithLocation fmt, Args&&... args) {
    spdlog::default_logger_raw()->log(
        fmt.loc, spdlog::level::trace, fmt::runtime(fmt.fmt),
        std::forward<Args>(args)...
    );
}

template <typename... Args>
void warn(details::FormatWithLocation fmt, Args&&... args) {
    spdlog::default_logger_raw()->log(
        fmt.loc, spdlog::level::warn, fmt::runtime(fmt.fmt),
        std::forward<Args>(args)...
    );
}

template <typename... Args>
[[noreturn]] void panic(details::FormatWithLocation fmt, Args&&... args) {
    fmt::println(
        "!! PANIC, unexpected path executed: {}:{} - {}", fmt.loc.filename,
        fmt.loc.line, fmt.loc.funcname
    );
    details::abort(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void expect(bool condition, details::FormatWithLocation fmt, Args&&... args) {
    if (not condition) [[unlikely]] {
        fmt::println(
            "!! ASSERTION FAILED: {}:{} - {}", fmt.loc.filename, fmt.loc.line,
            fmt.loc.funcname
        );
        details::abort(fmt, std::forward<Args>(args)...);
    }
}

template <typename T>
void expect(
    const Result<T>& r,
    const std::source_location& location = std::source_location::current()
) {
    if (not r) [[unlikely]] {
        log::panic(
            details::FormatWithLocation{
                "Unexpected error: Error code: {}, message: {}", location
            },
            fmt::underlying(r.error().code()), r.error().message()
        );
    }
}

namespace internal {

template <typename... Args>
void debug(details::FormatWithLocation fmt, Args&&... args) {
#ifdef SL_ENABLE_INTERNAL_LOGGING
    log::debug(std::move(fmt), std::forward<Args>(args)...);
#endif
}

template <typename... Args>
void error(details::FormatWithLocation fmt, Args&&... args) {
#ifdef SL_ENABLE_INTERNAL_LOGGING
    log::error(std::move(fmt), std::forward<Args>(args)...);
#endif
}

template <typename... Args>
void info(details::FormatWithLocation fmt, Args&&... args) {
#ifdef SL_ENABLE_INTERNAL_LOGGING
    log::info(std::move(fmt), std::forward<Args>(args)...);
#endif
}

template <typename... Args>
void trace(details::FormatWithLocation fmt, Args&&... args) {
#ifdef SL_ENABLE_INTERNAL_LOGGING
    log::trace(std::move(fmt), std::forward<Args>(args)...);
#endif
}

template <typename... Args>
void warn(details::FormatWithLocation fmt, Args&&... args) {
#ifdef SL_ENABLE_INTERNAL_LOGGING
    log::warn(std::move(fmt), std::forward<Args>(args)...);
#endif
}

}  // namespace internal

}  // namespace sl::log