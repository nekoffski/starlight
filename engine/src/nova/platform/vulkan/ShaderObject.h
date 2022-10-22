#pragma once

#include <array>
#include <string>

#include "Vulkan.h"
#include "ShaderStage.h"
#include "Pipeline.h"

namespace nova::platform::vulkan {

class ShaderObject {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit ShaderObject(const Context* context, const Device* device) {
        static std::string defaultVertexShader = "Simple.vert";
        static std::string defaultPixelShader  = "Simple.frag";

        core::FileSystem fs;

        m_stages.reserve(s_stagesCount);

        m_stages.emplace_back(
            device, context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::vertex}
        );

        m_stages.emplace_back(
            device, context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::fragment}
        );
    }

    const std::vector<ShaderStage>& getStages() const { return m_stages; }

   private:
    std::vector<ShaderStage> m_stages;
    // Pipeline m_pipeline;
};

}  // namespace nova::platform::vulkan
