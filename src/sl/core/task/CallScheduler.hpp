#pragma once

#include <map>
#include <string>

namespace sl::core::task {

class CallScheduler {
public:
    static void update(float dtime) {
        for (auto& [_, timer] : m_labels)
            timer -= dtime;
    }

    template <typename F, typename... Args>
    static void callIfExpired(float time, std::string label, F&& f, Args&&... args) {
        if (m_labels[label] > 0.0f)
            return;
        m_labels[label] = time;
        f(std::forward<Args>(args)...);
    }

private:
    inline static std::unordered_map<std::string, float> m_labels;
};
}
