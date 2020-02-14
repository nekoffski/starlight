#include "Logger.h"

namespace starl::core::log {

void Logger::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("Starlight");
    m_logger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::getLogger() {
    return m_logger;
}

}