#pragma once

#include <array>

#include "Vulkan.h"
#include "ShaderStage.h"
#include "Pipeline.h"

namespace nova::platform::vulkan {

class ShaderObject {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit ShaderObject() {
        m_stages.reserve(s_stagesCount);
        // create shader stages
    }

   private:
    std::vector<ShaderStage> m_stages;
    Pipeline m_pipeline;
};

}  // namespace nova::platform::vulkan
