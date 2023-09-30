#pragma once

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class Shader final : public sl::Shader {
public:
private:
    u32 m_internalId;
};

}  // namespace sl::vk