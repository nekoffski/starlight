#pragma once

#include <glm/glm.hpp>

namespace nova::math {

template <typename T> struct Size2 {
    T width;
    T height;

    operator glm::vec2() { return glm::vec2{width, height}; };
};

using Size2f = Size2<float>;
using Size2i = Size2<int>;

}  // namespace nova::math