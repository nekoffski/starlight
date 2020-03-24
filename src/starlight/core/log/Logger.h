#pragma once

#include <spdlog/spdlog.h>

#include <starlight/core/macros/class.h>

namespace starl {
using Logger = std::shared_ptr<spdlog::logger>;
}

namespace starl::core::log {

extern void initLogging();
extern Logger createLogger(const std::string&);
}