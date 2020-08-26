#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "starlight/core/log/Logger.h"
#include "starlight/math/Utils.hpp"
#include "starlight/platform/time/Clock.h"

constexpr float eta = 0.3f;

namespace sl::core::perf {

class Profiler {
    class Timer {
    public:
        explicit Timer(float& value)
            : m_value(value)
            , m_start(platform::time::Clock::now()) {
        }

        void begin() {
            m_start = platform::time::Clock::now();
        }

        void end() {
            float tmp = platform::time::Clock::now()->substract(m_start);
            m_value = math::linInterpolate(m_value, tmp, eta);
        }

    private:
        float& m_value;
        std::shared_ptr<platform::time::impl::ITimestamp> m_start;
    };

public:
    static void init(std::string logfile) {
        m_logfile = std::move(logfile);
    }

    static Timer createTimer(std::string name) {
        if (m_times.count(name) > 0) {
            // todo
        }
        m_times[name] = 0.0f;
        return Timer{ m_times[name] };
    }

    static void shutdown() {
    }

    static void printResults(unsigned cycles = 10) {
        static unsigned counter = 0;

        if (counter++ < cycles)
            return;

        counter = 0;
        std::ostringstream ss;
        ss << std::fixed;

        for (const auto& [label, value] : m_times) {
            ss << "[" << label << "] -> " << value << "s\n";
        }

        std::cout << "\n"
                  << ss.str();
    }

private:
    inline static std::string m_logfile;
    inline static std::unordered_map<std::string, float> m_times;
    // inline static std::shared_ptr<platform::clock::Clock> m_clock;
};
}

#define PROFILER_ENABLED 1

#ifdef PROFILER_ENABLED
#define PROFILER_CREATE_TIMER(name) core::perf::Profiler::createTimer(name)
#define PROFILER_TIMER_BEGIN(timer) timer.begin()
#define PROFILER_TIMER_END(timer) timer.end()
#define PROFILER_PRINT(cycles) core::perf::Profiler::printResults(cycles);
#else
#define PROFILER_CREATE_TIMER(name) 0
#define PROFILER_TIMER_BEGIN(timer)
#define PROFILER_TIMER_END(timer)
#define PROFILER_PRINT(cycles)
#endif
