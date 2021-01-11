#pragma once

#include <deque>
#include <string>

#include "sl/core/Macros.h"
#include "sl/core/utils/String.hpp"

namespace editor {

class DebugConsole {
    SL_STATIC_CLASS(DebugConsole);

public:
    template <typename... Args>
    static void write(const std::string& format, Args&& ...args) {
        auto line = sl::core::utils::format(format, std::forward<Args>(args)...);

        m_buffer.emplace_front(line + '\n');
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
