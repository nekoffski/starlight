#include "Shader.h"

#include <array>
#include <unordered_map>

#include "starlight/renderer/ShaderStage.h"

namespace sl::vk {

constexpr u32 shaderMaxStages           = 8;
constexpr u32 shaderMaxGlobalTextures   = 31;
constexpr u32 shaderMaxInstanceTextures = 31;
constexpr u32 shaderMaxAttributes       = 16;
constexpr u32 descSetIndexGlobal        = 0;
constexpr u32 descSetIndexInstance      = 1;
constexpr u32 bindingIndexUBO           = 0;
constexpr u32 bindingIndexSampler       = 1;

static const std::unordered_map<sl::ShaderStage::Type, VkShaderStageFlagBits>
  vkShaderStages = {
      {sl::ShaderStage::fragment,  VK_SHADER_STAGE_FRAGMENT_BIT},
      { sl::ShaderStage::geometry, VK_SHADER_STAGE_GEOMETRY_BIT},
      { sl::ShaderStage::vertex,   VK_SHADER_STAGE_VERTEX_BIT  },
      { sl::ShaderStage::compute,  VK_SHADER_STAGE_COMPUTE_BIT }
};

Shader::Shader(sl::Shader& self, Device* device, const Context* context) :
    m_self(self), m_device(device), m_context(context) {
    // TODO: pass renderpass

    const auto stageCount = self.stages.size();

    ASSERT(
      stageCount <= shaderMaxStages, "Stage count {} exceeds maximum {}", stageCount,
      shaderMaxStages
    );

    std::vector<VkShaderStageFlags> vkStages;
    vkStages.reserve(stageCount);
    m_config.stages.reserve(stageCount);

    for (auto& stage : self.stages) {
        const auto vkStage = vkShaderStages.find(stage.type);
        if (vkStage == vkShaderStages.end()) {
            LOG_ERROR("Unknown or unsupported shader stage: {}", stage.type);
            continue;
        }
        vkStages.push_back(vkStage->second);
        m_config.stages.emplace_back(vkStage->second, stage.source);
    }

    static const u32 maxDescriptorAllocateCount = 1024;

    m_config.maxDescriptorSetCount = maxDescriptorAllocateCount;
    m_config.poolSizes[0] =
      VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 };
    m_config.poolSizes[0] =
      VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4096 };

    DescriptorSetConfig globalDescriptorSetConfig;
    globalDescriptorSetConfig.bindings.push_back(VkDescriptorSetLayoutBinding{
      bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT });
    m_config.descriptorSets[descSetIndexGlobal] = globalDescriptorSetConfig;
    m_config.descriptorSetCount++;

    if (self.useInstances) {
        DescriptorSetConfig instanceDescriptorSetConfig;
        instanceDescriptorSetConfig.bindings.push_back(VkDescriptorSetLayoutBinding{
          bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
          VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT });
        m_config.descriptorSets[descSetIndexInstance] = instanceDescriptorSetConfig;
        m_config.descriptorSetCount++;
    }
    m_instanceStates.resize(1024);
}

void Shader::initialize() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();
}

void Shader::createModule(const StageConfig& cfg) {
    // vk::ShaderStage stage;
}

}  // namespace sl::vk