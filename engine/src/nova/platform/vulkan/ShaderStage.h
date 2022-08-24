#pragma once

#include "nova/core/FileSystem.h"

#include "Vulkan.h"
#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

class ShaderStage {
   public:
    struct Args {
        const Device* device;
        const Context* context;

        const std::string& name;
        const std::string& type;  // TODO: extract to enum

        VkShaderStageFlagBits stageFlag;

        core::FileSystem* fs;
    };

    explicit ShaderStage(const Args& args) {
        static std::string_view shadersPath      = "assets/shaders";  // TODO: read from cfg
        static std::string_view shadersExtension = "spv";

        const auto path =
            fmt::format("{}/{}.{}.{}", shadersPath, args.name, args.type, shadersExtension);

        m_moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        if (not args.fs->isFile(path)) {
            // not exists
        }

        const auto shaderContent = args.fs->readFile(path);

        m_moduleCreateInfo.codeSize = shaderContent.size();
        m_moduleCreateInfo.pCode    = (uint32_t*)(shaderContent.data());

        VK_ASSERT(vkCreateShaderModule(
            args.device->getLogicalDevice(), &m_moduleCreateInfo, args.context->getAllocator(),
            &m_handle
        ));

        m_stageCreateInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_stageCreateInfo.stage  = args.stageFlag;
        m_stageCreateInfo.module = m_handle;
        m_stageCreateInfo.pName  = "main";
    }

   private:
    VkShaderModuleCreateInfo m_moduleCreateInfo;
    VkPipelineShaderStageCreateInfo m_stageCreateInfo;
    VkShaderModule m_handle;
};

}  // namespace nova::platform::vulkan
