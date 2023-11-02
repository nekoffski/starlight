#pragma once

#include <fmt/core.h>
#include <kc/json/Json.h>
#include <kc/json/Utils.hpp>

namespace sl {

auto fieldFrom(auto& root) { return kc::json::fieldFrom<kc::json::JsonError>(root); }

template <typename T> T getField(auto& root, const std::string& name) {
    return fieldFrom(root).withName(name).template ofType<T>().get();
}

template <typename T>
T getFieldOr(auto& root, const std::string& name, const T& defaultValue) {
    if (root.isMember(name)) return getField<T>(root, name);
    return defaultValue;
}

auto getArray(auto& root, const std::string& name) {
    return fieldFrom(root).withName(name).asArray().get();
}

}  // namespace sl
