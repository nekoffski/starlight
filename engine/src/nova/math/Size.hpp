#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace nova::math {

template <typename T>
concept SizeType = std::is_arithmetic_v<T>;

template <SizeType T> struct Size2 {
    T width;
    T height;

    operator glm::vec2() { return glm::vec2{width, height}; };
};

using Size2f   = Size2<float>;
using Size2i   = Size2<int>;
using Size2u32 = Size2<uint32_t>;

}  // namespace nova::math