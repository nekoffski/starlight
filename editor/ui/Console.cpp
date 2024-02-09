#include "Console.h"

#include "starlight/core/utils/TimeManager.h"
#include "starlight/ui/UI.h"

Console::Console() : m_logger(m_buffer) {}

void Console::render() {
    sl::ui::namedScope("console-text", [&]() { sl::ui::text("{}", m_buffer); });
}

void Console::clear() { m_buffer.clear(); }

Logger* Console::getLogger() { return &m_logger; }

Logger::Logger(std::string& buffer) : m_buffer(buffer) {}

void Logger::writeLog(const std::string& severity, const std::string& message) {
    auto clock     = sl::TimeManager::get().getClock();
    auto timestamp = clock->getTimeString("%Y-%m-%d %H:%M:%S");
    // TODO: append in some more performance-friendly way
    m_buffer =
      fmt::format("{} - [{}]: {}\n{}", timestamp, severity, message, m_buffer);
}
