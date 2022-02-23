#pragma once

#include "sl/ecs/Component.h"

struct TestComponent : public sl::ecs::Component {
    explicit TestComponent(int x, float y) : x(x), y(y) { name = "TestComponent"; }

    int x;
    float y;
};

struct TestComponent2 : TestComponent {};