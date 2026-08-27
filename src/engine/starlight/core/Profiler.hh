#pragma once

#include <map>
#include <mutex>
#include <unordered_map>

#include "Core.hh"
#include "Macros.hh"
#include "Singleton.hh"
#include "Time.hh"

namespace starlight {

struct ProfilerEvent {
    enum class Type { begin, end };

    Type type;
    std::string name;
    Clock::time_point timestamp;
};

using ProfilerEvents = std::vector<ProfilerEvent>;
using ProfilerEventsPerThread =
    std::unordered_map<std::thread::id, ProfilerEvents>;

class RegionTimer : public NonCopyable, public NonMovable {
   public:
    explicit RegionTimer(std::string name, ProfilerEvents& events);
    ~RegionTimer();

   private:
    std::string m_name;
    ProfilerEvents& m_events;
};

class ProfilerSummary {
    struct SummaryGraphNode {
        std::string name;
        std::vector<u64> times;
        std::map<std::string, SummaryGraphNode> children;
        SummaryGraphNode* parent{nullptr};
        u64 insertionIndex{0};
        u64 nextChildIndex{0};
    };

    struct CallStatistics {
        u64 count{0};
        f64 totalTime{0.0};    // nanoseconds
        f64 averageTime{0.0};  // nanoseconds
        std::string name;
        std::string fullName;
    };

   public:
    explicit ProfilerSummary(const ProfilerEventsPerThread& events);

    void print() const;

   private:
    void generateSummary(const ProfilerEventsPerThread& events);
    void processThread(SummaryGraphNode& node, const ProfilerEvents& events);
    void printGraph(
        const SummaryGraphNode& node, const std::string& prefix = {}
    ) const;

    static CallStatistics computeStats(
        const SummaryGraphNode& node, const std::string& fullName
    );

    void forEachNode(
        const std::function<void(const CallStatistics&, u64, std::thread::id)>&
            fn
    ) const;

    void forEachNodeImpl(
        const SummaryGraphNode& node, const std::string& parentName,
        const std::function<void(const CallStatistics&, u64, std::thread::id)>&
            fn,
        u64 indent, std::thread::id threadId
    ) const;

    std::unordered_map<std::thread::id, SummaryGraphNode> m_summaryGraphs;
};

class Profiler : public Singleton<Profiler> {
   public:
    RegionTimer profileRegion(const std::string& name);
    void registerThread();

    [[nodiscard]] ProfilerSummary generateSummary();

   private:
    ProfilerEvents& threadEvents();
    void clear();

    std::mutex m_mutex;
    ProfilerEventsPerThread m_threads;
    thread_local static ProfilerEvents* s_threadEvents;
};

}  // namespace starlight

constexpr bool profilerEnabled() {
#ifdef STARLIGHT_ENABLE_PROFILING
    return true;
#else
    return false;
#endif
}

#ifdef STARLIGHT_ENABLE_PROFILING

#define STARLIGHT_PROFILE_REGION(name) \
    auto ANONYMOUS_VAR(REGION_TIMER) = \
        starlight::Profiler::get().profileRegion(name)

#define STARLIGHT_PROFILE_FUNCTION()     \
    auto ANONYMOUS_VAR(FUNCTION_TIMER) = \
        starlight::Profiler::get().profileRegion(__func__)

#define STARLIGHT_PROFILE_REGISTER_THREAD() \
    starlight::Profiler::get().registerThread()
#define STARLIGHT_PROFILE_DUMP_SUMMARY() \
    starlight::Profiler::get().generateSummary().print()

#else

#define STARLIGHT_PROFILE_REGION(name)
#define STARLIGHT_PROFILE_FUNCTION()
#define STARLIGHT_PROFILE_REGISTER_THREAD()
#define STARLIGHT_PROFILE_DUMP_SUMMARY()

#endif
