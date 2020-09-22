#pragma once

#include <typeindex>
#include <unordered_map>

#include "sl/core/Error.h"
#include "sl/core/log/Logger.h"

namespace sl::core::path {

class PathManager {
public:
    template <typename T>
    static void registerResourcePath(const std::string& path) {
        SL_INFO("registering path: {} - {}", typeid(T).name(), path);
        if (0 < m_resourcePaths.count(typeid(T)))
            throw CoreException(ErrorCode::PATH_ALREADY_REGISTERED);
        m_resourcePaths[typeid(T)] = path;
    }

    template <typename T>
    static std::string createGlobalPath(const std::string& path) {
        if (0 >= m_resourcePaths.count(typeid(T)))
            throw CoreException(ErrorCode::PATH_NOT_REGISTERED);
        return m_resourcePaths.at(typeid(T)) + path;
    }

private:
    inline static std::unordered_map<std::type_index, std::string> m_resourcePaths;
};
}
