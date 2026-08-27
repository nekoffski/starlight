#pragma once

#include <cstdlib>
#include <optional>
#include <stdexcept>
#include <string>

#include "Concepts.hh"
#include "Core.hh"

namespace starlight {

template <typename T = std::string>
    requires OneOf<T, std::string, u64, i64, f32, f64>
inline Opt<T> getEnv(const std::string& env) {
    if (const auto value = std::getenv(env.c_str()); value) {
        try {
            if constexpr (std::is_same_v<T, std::string>) {
                return std::string{value};
            } else if constexpr (std::is_same_v<T, u64>) {
                return std::stoul(value);
            } else if constexpr (std::is_same_v<T, i64>) {
                return std::stol(value);
            } else if constexpr (std::is_same_v<T, f32>) {
                return std::stof(value);
            } else if constexpr (std::is_same_v<T, f64>) {
                return std::stod(value);
            }
        } catch ([[maybe_unused]] const std::logic_error&) {
            return {};
        }
    }
    return {};
}

}  // namespace starlight
