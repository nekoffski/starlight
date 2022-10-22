#include "ShaderStage.h"

#include "nova/core/FileSystem.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

#ifndef NOVA_ASSETS_SHADERS_PATH
#define NOVA_ASSETS_SHADERS_PATH ""
#endif

std::string toString(ShaderStage::Type type) {
    switch (type) {
        case ShaderStage::Type::vertex:
            return "vert";
        case ShaderStage::Type::fragment:
            return "frag";
    }

    throw VulkanError("Unknown shader type");
}

VkShaderStageFlagBits getStageFlagBits(ShaderStage::Type type) {
    switch (type) {
        case ShaderStage::Type::vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Type::fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    throw VulkanError("Unknown shader type");
}

static std::string assembleShaderPath(const std::string& name, ShaderStage::Type type) {
    static std::string_view shadersPath      = NOVA_ASSETS_SHADERS_PATH;  // TODO: read from cfg
    static std::string_view shadersExtension = "spv";

    return fmt::format("{}/{}.{}.{}", shadersPath, name, toString(type), shadersExtension);
}

ShaderStage::ShaderStage(
    const Device* device, const Context* context, core::FileSystem* fs, const Properties& props
)
    : m_device(device), m_context(context), m_handle(VK_NULL_HANDLE) {
    const auto path = assembleShaderPath(props.name, props.type);

    ASSERT(fs->isFile(path), "Could not find shader file {}", path);

    const auto shaderContent = fs->readFile(path);

    m_moduleCreateInfo          = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    m_moduleCreateInfo.codeSize = shaderContent.size();
    m_moduleCreateInfo.pCode    = (uint32_t*)(shaderContent.data());

    VK_ASSERT(vkCreateShaderModule(
        device->getLogicalDevice(), &m_moduleCreateInfo, context->getAllocator(), &m_handle
    ));

    LOG_DEBUG("Shader module created");

    m_stageCreateInfo        = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    m_stageCreateInfo.stage  = getStageFlagBits(props.type);
    m_stageCreateInfo.module = m_handle;
    m_stageCreateInfo.pName  = "main";
}

ShaderStage::~ShaderStage() {
    if (m_handle)
        vkDestroyShaderModule(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

}  // namespace nova::platform::vulkan
