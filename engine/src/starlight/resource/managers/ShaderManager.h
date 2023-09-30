#pragma once

#include <kc/core/Singleton.hpp>

#include "starlight/renderer/ShaderScope.h"
#include "starlight/renderer/ShaderAttribute.h"
#include "starlight/renderer/ShaderUniform.h"
#include "starlight/renderer/Shader.h"

namespace sl {

class ShaderManager {
public:
    struct Config {
        u64 maxShaderCount;
    };

    Id32 getShaderId(const std::string& name);

    Shader* getById(Id32 id);
    Shader* get(const std::string& name);

private:
    Config m_conf;
};

}  // namespace sl
