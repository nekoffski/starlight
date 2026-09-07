#include "Profiler.hh"

#include <fmt/std.h>

#include <algorithm>
#include <numeric>

#include "Functional.hh"

namespace sl {

thread_local ProfilerEvents* Profiler::s_threadEvents = nullptr;

static constexpr f64 toMS(f64 nanoseconds) {
    return static_cast<f64>(nanoseconds) / 1'000'000.0;
}

RegionTimer Profiler::profileRegion(const std::string& name) {
    return RegionTimer{name, threadEvents()};
}

void Profiler::registerThread() {
    const auto id = std::this_thread::get_id();
    std::lock_guard lock{m_mutex};
    auto [it, _] = m_threads.try_emplace(id);
    s_threadEvents = &it->second;
}

ProfilerEvents& Profiler::threadEvents() {
    if (s_threadEvents == nullptr) {
        registerThread();
    }
    return *s_threadEvents;
}

ProfilerSummary Profiler::generateSummary() {
    std::lock_guard lock{m_mutex};
    DEFER { clear(); };
    return ProfilerSummary{m_threads};
}

void Profiler::clear() {
    for (auto& [_, events] : m_threads) {
        events.clear();
    }
}

RegionTimer::RegionTimer(std::string name, ProfilerEvents& events)
    : m_name(std::move(name)), m_events(events) {
    m_events.push_back({
        .type = ProfilerEvent::Type::begin,
        .name = m_name,
        .timestamp = Clock::now(),
    });
}

RegionTimer::~RegionTimer() {
    m_events.push_back({
        .type = ProfilerEvent::Type::end,
        .name = m_name,
        .timestamp = Clock::now(),
    });
}

ProfilerSummary::ProfilerSummary(const ProfilerEventsPerThread& events) {
    generateSummary(events);
}

void ProfilerSummary::print() const {
    log::info("──── Profiler ────────────────────────────────────");
    for (const auto& [threadId, graph] : m_summaryGraphs) {
        log::info("  Thread {}", threadId);
        printGraph(graph, "  ");
    }
    log::info("──────────────────────────────────────────────────");
}

void ProfilerSummary::generateSummary(const ProfilerEventsPerThread& events) {
    for (const auto& [threadId, threadEvents] : events) {
        SummaryGraphNode root{.name = "root"};
        processThread(root, threadEvents);
        m_summaryGraphs[threadId] = std::move(root);
    }
}

void ProfilerSummary::processThread(
    SummaryGraphNode& root, const ProfilerEvents& events
) {
    auto* current = &root;
    for (const auto& [type, name, timestamp] : events) {
        if (type == ProfilerEvent::Type::begin) {
            auto [it, inserted] = current->children.try_emplace(name);
            auto& child = it->second;
            if (inserted) {
                child.insertionIndex = current->nextChildIndex++;
                child.name = name;
                child.parent = current;
            }
            child.times.push_back(timestamp.time_since_epoch().count());
            current = &child;
        } else {
            if (current == &root or current->times.empty()) {
                continue;
            }
            auto& times = current->times;
            const auto beginTime = times.back();
            times.back() = timestamp.time_since_epoch().count() - beginTime;
            current = current->parent;
        }
    }
}

void ProfilerSummary::printGraph(
    const SummaryGraphNode& node, const std::string& prefix
) const {
    std::vector<const SummaryGraphNode*> ordered;
    ordered.reserve(node.children.size());
    for (const auto& [_, child] : node.children) {
        ordered.push_back(&child);
    }
    std::sort(
        ordered.begin(), ordered.end(),
        [](const SummaryGraphNode* a, const SummaryGraphNode* b) {
            return a->insertionIndex < b->insertionIndex;
        }
    );
    for (size_t i = 0; i < ordered.size(); ++i) {
        const bool isLast = i + 1 == ordered.size();
        const auto& child = *ordered[i];
        const auto stats = computeStats(child, child.name);

        const auto* branch = isLast ? "└─ " : "├─ ";
        const auto* stem = isLast ? "   " : "│  ";

        if (stats.count > 1) {
            log::info(
                "{}{}{}  x{}  {:.3f}ms  (avg {:.3f}ms)", prefix, branch,
                child.name, stats.count, toMS(stats.totalTime),
                toMS(stats.averageTime)
            );
        } else {
            log::info(
                "{}{}{}  {:.3f}ms", prefix, branch, child.name,
                toMS(stats.totalTime)
            );
        }
        printGraph(child, prefix + stem);
    }
}

void ProfilerSummary::forEachNode(
    const std::function<void(const CallStatistics&, u64, std::thread::id)>& fn
) const {
    for (const auto& [threadId, graph] : m_summaryGraphs) {
        forEachNodeImpl(graph, "", fn, 0, threadId);
    }
}

ProfilerSummary::CallStatistics ProfilerSummary::computeStats(
    const SummaryGraphNode& node, const std::string& fullName
) {
    const u64 total =
        std::accumulate(node.times.begin(), node.times.end(), u64{0});
    const u64 count = node.times.size();
    return {
        .count = count,
        .totalTime = static_cast<f64>(total),
        .averageTime = count > 0 ? static_cast<f64>(total) / count : 0.0,
        .name = node.name,
        .fullName = fullName,
    };
}

void ProfilerSummary::forEachNodeImpl(
    const SummaryGraphNode& node, const std::string& parentName,
    const std::function<void(const CallStatistics&, u64, std::thread::id)>& fn,
    u64 indent, std::thread::id threadId
) const {
    const bool isRoot = node.parent == nullptr;

    if (not isRoot) {
        const auto fullName =
            parentName.empty() ? node.name : parentName + "::" + node.name;
        fn(computeStats(node, fullName), indent, threadId);
        for (const auto& [_, child] : node.children) {
            forEachNodeImpl(child, fullName, fn, indent + 1, threadId);
        }
    } else {
        for (const auto& [_, child] : node.children) {
            forEachNodeImpl(child, parentName, fn, indent, threadId);
        }
    }
}

}  // namespace sl
