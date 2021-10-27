#pragma once

#include <deque>
#include <iostream>
#include <string>

#include <fmt/core.h>

#include "sl/core/ClockManager.h"
#include "sl/core/Macros.h"
#include "sl/core/String.hpp"

namespace editor {

class DebugConsole {
    LOG_STATIC_CLASS(DebugConsole);

public:
    template <typename... Args>
    static void write(std::string_view format, Args&&... args) {
        auto line = fmt::format(format, std::forward<Args>(args)...);
        auto time = sl::core::ClockManager::get().getClock().getTimeString("%H:%M:%S");

        m_buffer.emplace_front("[ " + time + " ] " + line + '\n');
    }

    static void clear() {
        m_buffer.clear();
    }

    static std::string getContext() {
        std::string context;
        for (auto& line : m_buffer)
            context += line;
        return context;
    }

private:
    inline static std::deque<std::string> m_buffer;
};
}

#define WRITE_DEBUG(...) editor::DebugConsole::write(__VA_ARGS__)
