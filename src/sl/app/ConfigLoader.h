#pragma once

#include <string>

#include "sl/core/FileSystem.h"
#include "sl/core/Json.h"
#include "sl/utils/Config.h"

namespace sl::app {

class ConfigLoader {
public:
    utils::Config loadFromFile(const std::string& path, const core::FileSystem& fileSystem) &&;

private:
    utils::Config::Paths processPaths(Json::Value& root);

    void raise(const std::string& reason);
};

}