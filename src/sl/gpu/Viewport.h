#pragma once

#include <string>

namespace sl::gpu {

struct Viewport {
    int width;
    int height;
    int beginX = 0;
    int beginY = 0;
};

std::string toString(const Viewport& viewport);

}  // namespace sl::gpu
