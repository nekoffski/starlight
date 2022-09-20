#pragma once

#include "nova/core/FileSystem.h"

#include "Vulkan.h"
#include "fwd.h"

namespace nova::platform::vulkan {

class ShaderStage {
   public:
    enum class Type { vertex, pixel };

    struct Properties {
        const std::string& name;
        Type type;
        VkShaderStageFlagBits stageFlag;
    };

    explicit ShaderStage(
        const Device* device, const Context* context, core::FileSystem* fs,
        const Properties& properties
    );

    ~ShaderStage();

   private:
    const Device* m_device;
    const Context* m_context;

    VkShaderModuleCreateInfo m_moduleCreateInfo;
    VkPipelineShaderStageCreateInfo m_stageCreateInfo;
    VkShaderModule m_handle;
};

}  // namespace nova::platform::vulkan
