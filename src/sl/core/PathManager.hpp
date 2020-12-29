#pragma once

#include <typeindex>
#include <unordered_map>

#include "sl/core/Logger.h"
#include "sl/core/error/ErrorCode.h"
#include "sl/core/error/Errors.hpp"

namespace sl::core {

class PathManager {
public:
    template <typename T>
    static void registerResourcePath(const std::string& path) {
        SL_INFO("registering path: {} - {}", typeid(T).name(), path);
        if (m_resourcePaths.contains(typeid(T)))
            throw core::error::CoreError{ core::error::ErrorCode::PathAlreadyRegistered };
        m_resourcePaths[typeid(T)] = path;
    }

    template <typename T>
    static std::string get() {
        if (not m_resourcePaths.contains(typeid(T)))
            throw core::error::CoreError{ core::error::ErrorCode::PathNotRegistered };
        return m_resourcePaths.at(typeid(T)) + "/";
    }

    template <typename T>
    static std::string createGlobalPath(const std::string& path) {
        if (not m_resourcePaths.contains(typeid(T)))
            throw core::error::CoreError{ core::error::ErrorCode::PathNotRegistered };
        return m_resourcePaths.at(typeid(T)) + path;
    }

private:
    inline static std::unordered_map<std::type_index, std::string> m_resourcePaths;
};
}
