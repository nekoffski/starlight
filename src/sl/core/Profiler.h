#pragma once

#include <memory>
#include <sstream>
#include <unordered_map>

#include "Clock.h"
#include "FileSystem.h"
#include "Macros.h"

#define SL_PROFILER_ENABLED 1

namespace sl::core {

class Profiler {
    SL_SINGLETON(Profiler);

    class RegionTimer {
    public:
        explicit RegionTimer(float& value);
        ~RegionTimer();

    private:
        void blendActualValueWithPreviousOne();

        float& m_value;
        Clock::TimePoint m_startTime;
    };

public:
    RegionTimer createRegionTimer(const std::string& name);

    void saveResults(const std::string& logDestination = "./", std::unique_ptr<FileSystem> fileSystem = std::make_unique<FileSystem>());

private:
    std::string formatTimers();
    std::unordered_map<std::string, float> m_times;
};
}

#define PROFILER() sl::core::Profiler::instance()

// clang-format off
#ifdef SL_PROFILER_ENABLED
    #define SL_PROFILE_REGION(name) auto __sl_prf_region_timer = PROFILER().createRegionTimer("Tag: " + std::string{name}); 
    #define SL_PROFILE_FUNCTION() auto __sl_prf_func_timer = PROFILER().createRegionTimer("Function: " + std::string{__PRETTY_FUNCTION__});
#else
    #define SL_PROFILE_REGION(name)
    #define SL_PROFILE_FUNCTION
#endif
// clang-format on
