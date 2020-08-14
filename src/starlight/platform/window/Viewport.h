#pragma once

namespace sl::platform::window {

struct Viewport {
    int width;
    int height;
    float fov{ 45.0f };
    float nearZ{ 0.1f };
    float farZ{ 100.0f };
};
}
