#pragma once

#include <stdlib.h>

#include <string_view>
#include <source_location>

#include <spdlog/spdlog.h>

namespace sl::log {

namespace details {

struct FormatWithLocation {
    std::string_view fmt;
    spdlog::source_loc loc;

    template <typename String>
    FormatWithLocation(
      const String& s,
      const std::source_location& location = std::source_location::current()
    )
        : fmt(s)
        , loc(
            location.file_name(), static_cast<int>(location.line()),
            location.function_name()
          ) {}
};

template <typename... Args>
[[noreturn]] constexpr inline void abort(
  FormatWithLocation&& formatString, Args&&... args
) {
    fmt::println(fmt::runtime(formatString.fmt), std::forward<Args>(args)...);
    std::abort();
}
}  // namespace details

void init(std::string_view applicationName);

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
      fmt.loc, spdlog::level::err, fmt::runtime(fmt.fmt), std::forward<Args>(args)...
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
    details::abort(std::move(fmt), std::forward<Args>(args)...);
}

template <typename... Args>
void expect(bool condition, details::FormatWithLocation fmt, Args&&... args) {
    if (not condition) [[unlikely]] {
        fmt::println(
          "!! ASSERTION FAILED: {}:{} - {}", fmt.loc.filename, fmt.loc.line,
          fmt.loc.funcname
        );
        details::abort(std::move(fmt), std::forward<Args>(args)...);
    }
}

}  // namespace sl::log
