#pragma once

#include "starlight/core/Core.hh"

using namespace sl;

struct StubResource {
    explicit StubResource(u64 id, int x, float y) : id(id), x(x), y(y) {}

    u64 id;
    int x;
    float y;
};
