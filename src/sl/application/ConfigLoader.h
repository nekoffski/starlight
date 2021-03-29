#pragma once

#include <string>

#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Json.h"
#include "sl/utils/globals/Config.h"

namespace sl::application {

class ConfigLoader {
public:
    utils::globals::Config loadFromFile(const std::string& path, const core::FileSystem& fileSystem) && {

        return utils::globals::Config {};
    }

private:
};

}