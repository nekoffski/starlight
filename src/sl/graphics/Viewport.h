#pragma once

namespace sl::graphics {

struct Viewport {
    explicit Viewport(int width, int height, float fieldOfView = 45.0f, float nearZ = 0.1f, float farZ = 100.0f)
        : width(width)
        , height(height)
        , fieldOfView(fieldOfView)
        , nearZ(nearZ)
        , farZ(farZ) {
    }

    int width;
    int height;
    float fieldOfView;
    float nearZ;
    float farZ;
};
}
