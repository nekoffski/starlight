#pragma once

#include <memory>

#include <kc/core/ErrorBase.hpp>

namespace nova::core {

template <typename T> using RefPtr = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr RefPtr<T> createRefPtr(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T> using UniqPtr = std::unique_ptr<T>;

template <typename T, typename... Args> constexpr UniqPtr<T> createUniqPtr(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

DEFINE_ERROR(NovaError);

}  // namespace nova::core