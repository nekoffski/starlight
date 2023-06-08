#pragma once

#include "starlight/core/FileSystem.h"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::platform::vulkan {

class ShaderStage {
   public:
    enum class Type { vertex, fragment };

    struct Properties {
        const std::string& name;
        Type type;
    };

    explicit ShaderStage(Device* device, const Context* context, const Properties& properties);

    VkPipelineShaderStageCreateInfo getStageCreateInfo() const { return m_stageCreateInfo; }

    ~ShaderStage();

   private:
    Device* m_device;
    const Context* m_context;

    VkShaderModuleCreateInfo m_moduleCreateInfo;
    VkPipelineShaderStageCreateInfo m_stageCreateInfo;
    VkShaderModule m_handle;
};

}  // namespace sl::platform::vulkan
