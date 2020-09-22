#include "Logger.h"

namespace sl::core::log {

void initLogging() {
    spdlog::set_pattern("%^[%d-%m-%Y %T] [Th: %t] %-7l [starlight]: %v - [%s:%#]%$");
    spdlog::set_level(spdlog::level::trace);
}
}
