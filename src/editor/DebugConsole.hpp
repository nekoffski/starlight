#pragma once

#include <deque>
#include <string>

#include "sl/core/Clock.h"
#include "sl/core/Macros.h"
#include "sl/core/String.hpp"

#include <iostream>

namespace editor {

class DebugConsole {
    SL_STATIC_CLASS(DebugConsole);

public:
    template <typename... Args>
    static void write(const std::string& format, Args&&... args) {
        auto line = sl::core::format(format, std::forward<Args>(args)...);
        auto time = sl::core::Clock::getTimeString("%H:%M:%S");

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
