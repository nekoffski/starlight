#pragma once

#include <string>

#include <kc/core/FileSystem.h>
#include <kc/json/Json.h>

#include "sl/utils/Config.h"

namespace sl::app {

class ConfigLoader {
public:
    utils::Config loadFromFile(const std::string& path, const kc::core::FileSystem& fileSystem = kc::core::FileSystem {}) &&;

private:
    utils::Config::Paths processPaths(kc::json::Node& root);

    void raise(const std::string& reason);
};

}