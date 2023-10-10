#include "VKShaderStage.h"

#include "starlight/core/utils/Log.h"

#include "VKDevice.h"
#include "VKContext.h"

namespace sl::vk {

VkShaderStageFlagBits getStageFlagBits(ShaderStage::Type type) {
    switch (type) {
        case ShaderStage::vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    FAIL("Unknown shader type: {}", type);
}

VKShaderStage::VKShaderStage(
  VKDevice* device, const VKContext* context, const Properties& props
) :
    m_device(device),
    m_context(context), m_handle(VK_NULL_HANDLE) {
    m_moduleCreateInfo          = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    m_moduleCreateInfo.codeSize = props.source.size();
    m_moduleCreateInfo.pCode    = (uint32_t*)(props.source.data());

    VK_ASSERT(vkCreateShaderModule(
      device->getLogicalDevice(), &m_moduleCreateInfo, context->getAllocator(),
      &m_handle
    ));

    LOG_DEBUG("Shader module created");

    m_stageCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    m_stageCreateInfo.stage  = getStageFlagBits(props.type);
    m_stageCreateInfo.module = m_handle;
    m_stageCreateInfo.pName  = "main";
}

VKShaderStage::~VKShaderStage() {
    if (m_handle)
        vkDestroyShaderModule(
          m_device->getLogicalDevice(), m_handle, m_context->getAllocator()
        );
}

}  // namespace sl::vk
