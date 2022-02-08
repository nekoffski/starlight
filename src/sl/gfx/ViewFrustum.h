#pragma once

namespace sl::gfx {

struct ViewFrustum {
    struct Viewport {
        int width;
        int height;
        int beginX = 0;
        int beginY = 0;
    };

    explicit ViewFrustum(int width, int height, float fieldOfView = 45.0f, float nearZ = 0.1f, float farZ = 100.0f)
        : viewport { width, height }
        , fieldOfView(fieldOfView)
        , nearZ(nearZ)
        , farZ(farZ) {
    }

    Viewport viewport;
    float fieldOfView;
    float nearZ;
    float farZ;
};

}
