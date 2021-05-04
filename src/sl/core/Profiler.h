#pragma once

#include <sstream>
#include <unordered_map>

#include "Clock.h"
#include "Macros.h"

#define SL_PROFILER_ENABLED 1
constexpr float ProfilerPrintInterval = 5.0f;

namespace sl::core {

class Profiler {
    SL_SINGLETON(Profiler);

    class RegionBasedTimer {
    public:
        explicit RegionBasedTimer(float& value);
        ~RegionBasedTimer();

    private:
        float& m_value;
        std::shared_ptr<core::Timestamp> m_start;
    };

public:
    RegionBasedTimer createRegionBasedTimer(const std::string& name);
    void saveResults(const std::string& logfile);
    void printResults();

private:
    std::string formatTimers();
    std::unordered_map<std::string, float> m_times;
};
}

#define PROFILER() sl::core::Profiler::instance()

// clang-format off
#ifdef SL_PROFILER_ENABLED
    #define SL_PROFILE_REGION(name) auto __sl_prf_region_timer = PROFILER().createRegionBasedTimer(name); 
    #define SL_PROFILE_FUNCTION() auto __sl_prf_func_timer = PROFILER().createRegionBasedTimer(__PRETTY_FUNCTION__);
#else
    #define SL_PROFILE_REGION(name)
    #define SL_PROFILE_FUNCTION
#endif
// clang-format on
