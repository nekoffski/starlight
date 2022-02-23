#pragma once

#include <kc/json/JsonConfigLoader.hpp>
#include <kc/json/Utils.hpp>

#include "Config.h"
#include "sl/core/Errors.hpp"

namespace sl::cfg {

DEFINE_ERROR(ConfigError);

class ConfigLoader : public kc::json::JsonConfigLoader<Config>,
                     protected kc::json::NodeHelper<kc::json::JsonError> {
   public:
    void processFields(const kc::json::Node& root) override;
};

}  // namespace sl::cfg