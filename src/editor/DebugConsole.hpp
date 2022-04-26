#pragma once

#include <fmt/core.h>

#include <deque>
#include <iostream>
#include <numeric>
#include <string>

#include "sl/core/ClockManager.h"
#include "sl/core/Macros.h"

namespace editor {

class DebugConsole {
    LOG_STATIC_CLASS(DebugConsole);

   public:
    template <typename... Args> static void write(std::string_view format, Args&&... args) {
        auto line = fmt::format(format, std::forward<Args>(args)...);
        auto time = sl::core::ClockManager::get().getClock().getTimeString("%H:%M:%S");

        m_buffer.emplace_front("[ " + time + " ] " + line + '\n');
    }

    static void clear() { m_buffer.clear(); }

    static std::string getContext() {
        return std::accumulate(
            m_buffer.begin(), m_buffer.end(), std::string{},
            [](const auto& sum, const auto& element) { return sum + element; }
        );
    }

   private:
    inline static std::deque<std::string> m_buffer;
};
}  // namespace editor

#define WRITE_DEBUG(...) editor::DebugConsole::write(__VA_ARGS__)
