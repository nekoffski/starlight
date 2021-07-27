#include "Profiler.h"

#include <fstream>
#include <unordered_map>

#include "sl/core/Logger.h"
#include "sl/math/Utils.hpp"

namespace sl::core {

Profiler::RegionTimer::RegionTimer(float& value)
    : m_value(value)
    , m_startTime(ClockManager::get()->now()) {
}

Profiler::RegionTimer::~RegionTimer() {
    blendActualValueWithPreviousOne();
}

void Profiler::RegionTimer::blendActualValueWithPreviousOne() {
    static constexpr float newValueWeight = 0.3f;

    auto deltaTime = ClockManager::get()->now() - m_startTime;
    m_value = math::lerp(m_value, ClockManager::get()->toSeconds(deltaTime), newValueWeight);
}

Profiler::RegionTimer Profiler::createRegionTimer(const std::string& name) {
    if (m_times.count(name) == 0)
        m_times[name] = 0.0f;

    return RegionTimer { m_times[name] };
}

void Profiler::saveResults(const std::string& logDestination, std::unique_ptr<FileSystem> fileSystem) {
    fileSystem->writeFile(logDestination + "logs.perf", formatTimers(), true);
}

std::string Profiler::formatTimers() {
    static auto compareStringsByLength = [](auto& lhs, auto& rhs) {
        return lhs.first.size() < rhs.first.size();
    };

    auto longestName = std::ranges::max_element(m_times, compareStringsByLength)->first;
    auto alignment = longestName.size();

    std::ostringstream ss;
    ss << std::fixed;

    for (const auto& [label, value] : m_times)
        ss << std::setw(alignment) << std::left << label << " -> " << value << "s\n";

    return ss.str();
}
}
