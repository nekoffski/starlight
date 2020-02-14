#pragma once

#include <spdlog/spdlog.h>

#include <starlight/core/macros/class.h>

namespace starl::core::log {

class Logger {
    STARL_STATIC_CLASS(Logger);   
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& getLogger();

private:
    inline static std::shared_ptr<spdlog::logger> m_logger{nullptr};
};

#define STARL_FATAL(...) starl::core::log::Logger::getLogger()->critical(__VA_ARGS__);
#define STARL_ERROR(...) starl::core::log::Logger::getLogger()->error(__VA_ARGS__);
#define STARL_WARN(...) starl::core::log::Logger::getLogger()->warn(__VA_ARGS__)
#define STARL_INFO(...) starl::core::log::Logger::getLogger()->info(__VA_ARGS__)
#define STARL_TRACE(...) starl::core::log::Logger::getLogger()->trace(__VA_ARGS__)

}