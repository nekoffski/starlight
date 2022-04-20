#pragma once

#include <string>

#include "Viewport.h"

namespace sl::gfx {

struct ViewFrustum {
    explicit ViewFrustum(int width, int height, float fieldOfView = 45.0f, float nearZ = 0.1f,
                         float farZ = 100.0f)
        : viewport{width, height}, fieldOfView(fieldOfView), nearZ(nearZ), farZ(farZ) {}

    Viewport viewport;
    float fieldOfView;
    float nearZ;
    float farZ;
};

}  // namespace sl::gfx
