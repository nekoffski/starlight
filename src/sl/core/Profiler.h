#pragma once

#include <sstream>
#include <unordered_map>

#include "sl/platform/time/Clock.h"

#define PRF_PROFILER_ENABLED 1
constexpr float PROFILER_PRINT_INTERVAL = 5.0f;

namespace sl::core {

class Profiler {
    class RegionBasedTimer {
    public:
        explicit RegionBasedTimer(float& value);
        ~RegionBasedTimer();

    private:
        float& m_value;
        std::shared_ptr<platform::time::impl::ITimestamp> m_start;
    };

public:
    static RegionBasedTimer createRegionBasedTimer(std::string name);
    static void saveResults(std::string logfile);
    static void printResults();

private:
    static std::string formatTimers();
    static std::unordered_map<std::string, float> m_times;
};
}

// clang-format off
#ifdef PRF_PROFILER_ENABLED
    #define PRF_PROFILE_REGION(name) auto __sl_prf_region_timer = core::Profiler::createRegionBasedTimer(name); 
    #define PRF_PROFILE_FUNCTION() auto __sl_prf_func_timer = core::Profiler::createRegionBasedTimer(__PRETTY_FUNCTION__);
#else
    #define PRF_PROFILE_REGION(name)
    #define PRF_PROFILE_FUNCTION
#endif
// clang-format on
