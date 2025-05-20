#pragma once

#include <nlohmann/json.hpp>

#include "Core.hh"

namespace glm {

template <typename T>
concept GlmRandomAccessIterable = requires(T value) {
    { value[0] };
    { glm::value_ptr(value) };
};

template <typename T>
requires GlmRandomAccessIterable<T>
void from_json(const nlohmann::json& j, T& v) {
    auto iterator = glm::value_ptr(v);
    for (sl::u64 i = 0; i < sl::getSize<T>(); ++i) iterator[i] = j[i].get<f32>();
}

template <typename T>
requires GlmRandomAccessIterable<T>
void to_json(nlohmann::json& j, const T& v) {
    auto iterator = glm::value_ptr(v);
    for (sl::u64 i = 0; i < sl::getSize<T>(); ++i) j.push_back(iterator[i]);
}

}  // namespace glm
