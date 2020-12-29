#include "Profiler.h"

#include <fstream>
#include <unordered_map>

#include "sl/core/Clock.h"
#include "sl/core/Logger.h"
#include "sl/math/Utils.hpp"

constexpr float EtaFactor = 0.3f;

namespace sl::core {

std::unordered_map<std::string, float> Profiler::m_times;

Profiler::RegionBasedTimer::RegionBasedTimer(float& value)
    : m_value(value)
    , m_start(core::Clock::now()) {
}

Profiler::RegionBasedTimer::~RegionBasedTimer() {
    float tmp = core::Clock::now()->substract(m_start);
    m_value = math::linInterpolate(m_value, tmp, EtaFactor);
}

Profiler::RegionBasedTimer Profiler::createRegionBasedTimer(std::string name) {
    if (m_times.count(name) == 0)
        m_times[name] = 0.0f;
    return RegionBasedTimer{ m_times[name] };
}

void Profiler::saveResults(std::string logdir) {
    std::string logfile = logdir + core::Clock::getTimeString("%d-%m-%Y_%H:%M:%S") + ".perf";
    std::ofstream log(logfile);

    if (!log.good()) {
        SL_ERROR("Could not open log file: {}", logfile);
        return;
    }

    log << formatTimers();
    log.close();
    SL_INFO("Profiler logs saved as: {}", logfile);
}

void Profiler::printResults() {
// clang-format off
    #ifdef SL_PROFILER_ENABLED
        SL_WARN("\n{}\n", formatTimers()); // CREATE OTHER FORMATTER!
    #endif
    // clang-format on
}

std::string Profiler::formatTimers() {
    std::ostringstream ss;
    ss << std::fixed;

    for (const auto& [label, value] : m_times)
        ss << label << " -> " << value << "s\n";

    return ss.str();
}
}
