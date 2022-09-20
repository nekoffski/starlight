#include "ShaderStage.h"

#include "nova/core/FileSystem.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

std::string toString(ShaderStage::Type type) {
    switch (type) {
        case ShaderStage::Type::vertex:
            return "vertex";
        case ShaderStage::Type::pixel:
            return "pixel";
    }

    throw VulkanError("Unknown shader type");
}

static std::string assembleShaderPath(const std::string& name, ShaderStage::Type type) {
    static std::string_view shadersPath      = "assets/shaders";  // TODO: read from cfg
    static std::string_view shadersExtension = "spv";

    return fmt::format("{}/{}.{}.{}", shadersPath, name, toString(type), shadersExtension);
}

ShaderStage::ShaderStage(
    const Device* device, const Context* context, core::FileSystem* fs, const Properties& props
)
    : m_device(device), m_context(context) {
    const auto path = assembleShaderPath(props.name, props.type);

    ASSERT(fs->isFile(path), "Could not find shader file {}", path);

    m_moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    const auto shaderContent = fs->readFile(path);

    m_moduleCreateInfo.codeSize = shaderContent.size();
    m_moduleCreateInfo.pCode    = (uint32_t*)(shaderContent.data());

    VK_ASSERT(vkCreateShaderModule(
        device->getLogicalDevice(), &m_moduleCreateInfo, context->getAllocator(), &m_handle
    ));

    m_stageCreateInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_stageCreateInfo.stage  = props.stageFlag;
    m_stageCreateInfo.module = m_handle;
    m_stageCreateInfo.pName  = "main";
}

ShaderStage::~ShaderStage() {
    vkDestroyShaderModule(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

}  // namespace nova::platform::vulkan
