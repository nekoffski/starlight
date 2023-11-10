#pragma once

#include "Vulkan.h"
#include "fwd.h"

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class VKShaderStage {
public:
    struct Properties {
        const std::string& source;
        Shader::Stage::Type type;
    };

    explicit VKShaderStage(
      VKDevice* device, const VKContext* context, const Properties& properties
    );

    VkPipelineShaderStageCreateInfo getStageCreateInfo() const {
        return m_stageCreateInfo;
    }

    ~VKShaderStage();

private:
    VKDevice* m_device;
    const VKContext* m_context;

    VkShaderModuleCreateInfo m_moduleCreateInfo;
    VkPipelineShaderStageCreateInfo m_stageCreateInfo;
    VkShaderModule m_handle;
};

}  // namespace sl::vk
