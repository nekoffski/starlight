#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "starlight/core/log/Logger.h"
#include "starlight/platform/clock/Clock.h"

constexpr float eta = 0.3f;

namespace sl::core::perf {

class Profiler {
    class Timer {
    public:
        explicit Timer(float& value, std::shared_ptr<platform::clock::Clock> clock)
            : m_value(value)
            , m_clock(clock) {}

        void begin() {
            m_start = m_clock->now();
        }

        void end() {
            float tmp = m_clock->now() - m_start;
            m_value = (1 - eta) * m_value + eta * tmp;
        }

    private:
        float& m_value;
        float m_start;
        std::shared_ptr<platform::clock::Clock> m_clock;
    };

public:
    static void init(std::shared_ptr<platform::clock::Clock> clock, std::string logfile) {
        m_clock = clock;
        m_logfile = std::move(logfile);
    }

    static Timer createTimer(std::string name) {
        if (m_times.count(name) > 0) {
            // todo
        }
        m_times[name] = 0.0f;
        return Timer{ m_times[name], m_clock };
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
    inline static std::shared_ptr<platform::clock::Clock> m_clock;
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
