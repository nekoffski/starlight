#pragma once

#include <typeindex>
#include <typeinfo>

namespace sl::core::misc {

template <typename T>
inline constexpr std::type_index typeIndex() {
    return std::type_index{ typeid(T) };
}
}
