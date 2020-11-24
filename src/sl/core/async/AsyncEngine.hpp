#pragma once

#include <limits>
#include <map>
#include <vector>
#include <string>

namespace sl::core::async {

class AsyncEngine;

class Timer {
	friend class AsyncEngine;
public:
    explicit Timer(float sleepTime)
        : m_sleepTime(sleepTime)
        , m_activated(false)
        , m_timer(m_sleepTime) {}

	
	void setSleepTime(float sleepTime) {
		m_sleepTime = sleepTime;

		if (m_timer > sleepTime)
			m_timer = sleepTime;
	}

    bool asyncSleep() {
        m_activated = true;
        if (m_timer <= 0) {
			m_timer = m_sleepTime;
			return false;
        }
		return true;
    }

private:
    float m_sleepTime;
    float m_timer;
    bool m_activated;
};

class AsyncEngine {
public:
    static void update(float dtime) {
        for (auto& timer : m_timers) {
        	if (timer->m_activated) {
        		timer->m_timer -= dtime;
				timer->m_activated = false;
        	}
        }
	}

	static std::shared_ptr<Timer> createTimer(float sleepTime) {
		auto timer = std::make_shared<Timer>(sleepTime);
		m_timers.push_back(timer);

		return timer;
	}

    /*template <typename F, typename... Args>*/
    //static void callIfExpired(float time, std::string label, F&& f, Args&&... args) {
    //if (m_labels[label] > 0.0f)
    //return;
    //m_labels[label] = time;
    //f(std::forward<Args>(args)...);
    /*}*/

private:
	inline static std::vector<std::shared_ptr<Timer>> m_timers;
    /*inline static std::unordered_map<std::string, float> m_labels;*/
};
}
