#pragma once

#include <concepts>

template <typename T>
    requires(std::is_enum_v<T> && requires(T e) { enableBitOperations(e); })
constexpr auto operator|(const T lhs, const T rhs) {
    using underlying = std::underlying_type_t<T>;

    return static_cast<T>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs)
    );
}

template <typename T>
    requires(std::is_enum_v<T> && requires(T e) { enableBitOperations(e); })
constexpr auto operator&(const T lhs, const T rhs) {
    using underlying = std::underlying_type_t<T>;

    return static_cast<T>(
        static_cast<underlying>(lhs) & static_cast<underlying>(rhs)
    );
}

template <typename T>
    requires(std::is_enum_v<T> && requires(T e) { enableBitOperations(e); })
constexpr auto operator|=(T& a, T b) {
    return a = a | b;
}

template <typename T>
    requires(std::is_enum_v<T> && requires(T e) { enableBitOperations(e); })
constexpr auto operator&=(T& a, T b) {
    return a = a & b;
}

namespace sl {

bool checkFlag(auto value, auto flag) {
    using T = std::decay_t<decltype(value)>;
    static_assert(
        std::is_enum_v<T> && requires(T e) { enableBitOperations(e); },
        "checkFlag can only be used with enums that have "
        "enableBitOperations defined"
    );

    using underlying = std::underlying_type_t<T>;
    return (static_cast<underlying>(value) & static_cast<underlying>(flag)) !=
           0;
}

#define SL_BIT_ENUM(name) constexpr void enableBitOperations(name);

}  // namespace sl
