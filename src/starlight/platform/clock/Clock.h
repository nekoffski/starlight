#pragma once

#include <functional>

namespace starl::platform::clock {

class Clock {
public:
    explicit Clock();

    void update();
    float getDeltaTime();

private:
    float getTicks();

    float m_previous;
    float m_current;
    float m_delta;
};
}
