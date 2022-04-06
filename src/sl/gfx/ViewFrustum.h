#pragma once

#include <kc/core/Log.h>

#include <string>

namespace sl::gfx {

struct ViewFrustum {
    struct Viewport {
        int width;
        int height;
        int beginX = 0;
        int beginY = 0;
    };

    explicit ViewFrustum(int width, int height, float fieldOfView = 45.0f, float nearZ = 0.1f,
                         float farZ = 100.0f)
        : viewport{width, height}, fieldOfView(fieldOfView), nearZ(nearZ), farZ(farZ) {}

    Viewport viewport;
    float fieldOfView;
    float nearZ;
    float farZ;
};

inline std::string toString(const ViewFrustum::Viewport& viewport) {
    return fmt::format("x={} y={} w={} h={}", viewport.beginX, viewport.beginY, viewport.width,
                       viewport.height);
}

}  // namespace sl::gfx
