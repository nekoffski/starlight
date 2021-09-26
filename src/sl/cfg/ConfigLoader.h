#pragma once

#include <string>

#include <kc/core/FileSystem.h>
#include <kc/json/Json.h>

#include "Config.h"

namespace sl::cfg {

class ConfigLoader {
public:
    Config loadFromFile(const std::string& path, const kc::core::FileSystem& fileSystem = kc::core::FileSystem {}) &&;

private:
    Config::Paths processPaths(kc::json::Node& root);

    void raise(const std::string& reason);
};

}