#include "Logger.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace starl::core::log {

void initLogging() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
}

Logger createLogger(const std::string& ident) {
    auto logger = spdlog::stdout_color_mt("[Starlight] " + ident);
    logger->set_level(spdlog::level::trace);
    return logger;
}
}