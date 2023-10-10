#include "VKShaderImpl.h"

#include <array>
#include <ranges>
#include <unordered_map>

#include "starlight/core/Core.h"
#include "starlight/core/window/WindowManager.h"
#include "starlight/renderer/ShaderStage.h"

#include "VKPipeline.h"

namespace sl::vk {

constexpr u32 shaderMaxStages           = 8;
constexpr u32 shaderMaxGlobalTextures   = 31;
constexpr u32 shaderMaxInstanceTextures = 31;
constexpr u32 shaderMaxAttributes       = 16;
constexpr u32 descSetIndexGlobal        = 0;
constexpr u32 descSetIndexInstance      = 1;
constexpr u32 bindingIndexUBO           = 0;
constexpr u32 bindingIndexSampler       = 1;

static const std::unordered_map<ShaderStage::Type, VkShaderStageFlagBits>
  vkShaderStages = {
      {ShaderStage::fragment,  VK_SHADER_STAGE_FRAGMENT_BIT},
      { ShaderStage::geometry, VK_SHADER_STAGE_GEOMETRY_BIT},
      { ShaderStage::vertex,   VK_SHADER_STAGE_VERTEX_BIT  },
      { ShaderStage::compute,  VK_SHADER_STAGE_COMPUTE_BIT }
};

static const std::unordered_map<ShaderAttribute::Type, VkFormat> vkAttributeTypes = {
    {ShaderAttribute::Type::float32,    VK_FORMAT_R32_SFLOAT         },
    { ShaderAttribute::Type::float32_2, VK_FORMAT_R32G32_SFLOAT      },
    { ShaderAttribute::Type::float32_3, VK_FORMAT_R32G32B32_SFLOAT   },
    { ShaderAttribute::Type::float32_4, VK_FORMAT_R32G32B32A32_SFLOAT},
    { ShaderAttribute::Type::int8,      VK_FORMAT_R8_SINT            },
    { ShaderAttribute::Type::uint8,     VK_FORMAT_R8_UINT            },
    { ShaderAttribute::Type::int16,     VK_FORMAT_R16_SINT           },
    { ShaderAttribute::Type::uint16,    VK_FORMAT_R16_UINT           },
    { ShaderAttribute::Type::int32,     VK_FORMAT_R32_SINT           },
    { ShaderAttribute::Type::uint32,    VK_FORMAT_R32_UINT           },
};

VKShaderImpl::VKShaderImpl(
  sl::Shader& self, VKDevice* device, const VKContext* context,
  VKRenderPass* renderPass
) :
    m_self(self),
    m_device(device), m_context(context), m_renderPass(renderPass) {
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
        m_config.stages.emplace_back(stage.type, stage.source);
    }

    static const u32 maxDescriptorAllocateCount = 1024;

    m_config.maxDescriptorSetCount = maxDescriptorAllocateCount;
    m_config.poolSizes[0] =
      VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 };
    m_config.poolSizes[1] =
      VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4096 };

    DescriptorSetConfig globalDescriptorSetConfig;
    globalDescriptorSetConfig.bindings[bindingIndexUBO] =
      VkDescriptorSetLayoutBinding{
          bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
          VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
      };
    globalDescriptorSetConfig.bindingCount++;
    m_config.descriptorSets[descSetIndexGlobal] = globalDescriptorSetConfig;
    m_config.descriptorSetCount++;

    if (self.useInstances) {
        DescriptorSetConfig instanceDescriptorSetConfig;
        instanceDescriptorSetConfig.bindings[bindingIndexUBO] =
          VkDescriptorSetLayoutBinding{
              bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
              VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
          };
        instanceDescriptorSetConfig.bindingCount++;
        m_config.descriptorSets[descSetIndexInstance] = instanceDescriptorSetConfig;
        m_config.descriptorSetCount++;
    }
    m_instanceStates.resize(1024);
}

void VKShaderImpl::initialize() {
    createModules();
    processAttributes();
    processUniforms();
    createDescriptorPool();
    createDescriptorSetLayouts();

    // viewport & scissor
    const auto [w, h] = WindowManager::get().getSize();

    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = static_cast<float>(h);
    viewport.width    = static_cast<float>(w);
    viewport.height   = -static_cast<float>(h);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = w;
    scissor.extent.height               = h;

    std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos;
    stageCreateInfos.reserve(m_stages.size());
    for (auto& stage : m_stages)
        stageCreateInfos.push_back(stage.getStageCreateInfo());

    VKPipeline::Properties pipelineProps;
    pipelineProps.stride               = m_self.attributeStride;
    pipelineProps.vertexAttributes     = m_config.attributes;
    pipelineProps.descriptorSetLayouts = m_descriptorSetLayouts;
    pipelineProps.stages               = stageCreateInfos;
    pipelineProps.viewport             = viewport;
    pipelineProps.scissor              = scissor;
    pipelineProps.polygonMode          = VK_POLYGON_MODE_FILL;
    pipelineProps.depthTestEnabled     = true;
    pipelineProps.pushConstantRanges   = m_self.pushConstantRanges;

    m_pipeline.emplace(m_context, m_device, *m_renderPass, pipelineProps);

    m_self.requiredUboAlignment =
      m_device->getProperties().limits.minUniformBufferOffsetAlignment;
    m_self.globalUboStride =
      getAlignedValue(m_self.globalUboStride, m_self.requiredUboAlignment);
    m_self.uboStride = getAlignedValue(m_self.uboSize, m_self.requiredUboAlignment);

    const u32 deviceLocalBits =
      m_device->supportsDeviceLocalHostVisible()
        ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        : 0;

    // material max count should be configurable!
    const u64 totalBufferSize = m_self.globalUboStride + (m_self.uboStride * 1024);

    VKBuffer::Properties bufferProps;
    bufferProps.size = totalBufferSize;
    bufferProps.memoryPropertyFlags =
      deviceLocalBits | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
      | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferProps.usageFlags =
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferProps.bindOnCreate = true;

    m_uniformBuffer.emplace(m_context, m_device, bufferProps);

    m_self.globalUboOffset     = m_uniformBuffer->allocate(m_self.globalUboStride);
    m_mappedUniformBufferBlock = m_uniformBuffer->lockMemory(0, VK_WHOLE_SIZE, 0);

    std::array<VkDescriptorSetLayout, 3> globalLayouts = {
        m_descriptorSetLayouts[descSetIndexGlobal],
        m_descriptorSetLayouts[descSetIndexGlobal],
        m_descriptorSetLayouts[descSetIndexGlobal]
    };
    VkDescriptorSetAllocateInfo allocateInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    };
    allocateInfo.descriptorPool     = m_descriptorPool;
    allocateInfo.descriptorSetCount = 3;
    allocateInfo.pSetLayouts        = globalLayouts.data();

    VK_ASSERT(vkAllocateDescriptorSets(
      m_device->getLogicalDevice(), &allocateInfo, m_globalDescriptorSets.data()
    ));
}

void VKShaderImpl::createModules() {
    m_stages.reserve(m_config.stages.size());

    for (auto& stageConfig : m_config.stages) {
        m_stages.emplace_back(
          m_device, m_context,
          VKShaderStage::Properties{ stageConfig.source, stageConfig.type }
        );
    }
}

void VKShaderImpl::processUniforms() {
    for (const auto& uniform : m_self.uniforms | std::views::values) {
        if (uniform.type == ShaderUniform::Type::sampler) {
            const auto setIndex =
              uniform.scope == ShaderScope::global
                ? descSetIndexGlobal
                : descSetIndexInstance;
            auto setConfig = &m_config.descriptorSets[setIndex];

            if (setConfig->bindings.size() < 2) {
                // no binding yet, this is first sampler to be added
                // create a binding with a single descriptor for this sampler
                auto& binding           = setConfig->bindings[bindingIndexSampler];
                binding.binding         = bindingIndexSampler;
                binding.descriptorCount = 1;
                binding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                binding.stageFlags =
                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            } else {
                setConfig->bindings[bindingIndexSampler].descriptorCount++;
            }
        }
    }
}

void VKShaderImpl::createDescriptorPool() {
    VkDescriptorPoolCreateInfo poolInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    };
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes    = m_config.poolSizes.data();
    poolInfo.maxSets       = m_config.maxDescriptorSetCount;
    poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    VK_ASSERT(vkCreateDescriptorPool(
      m_device->getLogicalDevice(), &poolInfo, m_context->getAllocator(),
      &m_descriptorPool
    ));
}

void VKShaderImpl::createDescriptorSetLayouts() {
    for (int i = 0; i < m_config.descriptorSetCount; ++i) {
        VkDescriptorSetLayoutCreateInfo info = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
        };
        info.bindingCount = m_config.descriptorSets[i].bindingCount;
        info.pBindings    = m_config.descriptorSets[i].bindings.data();
        VK_ASSERT(vkCreateDescriptorSetLayout(
          m_device->getLogicalDevice(), &info, m_context->getAllocator(),
          &m_descriptorSetLayouts[i]
        ));
    }
}

void VKShaderImpl::processAttributes() {
    u32 offset                = 0;
    const auto attributeCount = m_self.attributes.size();
    m_config.attributes.reserve(attributeCount);

    for (int i = 0; i < attributeCount; ++i) {
        VkVertexInputAttributeDescription attribute;
        attribute.location = i;
        attribute.binding  = 0;
        attribute.offset   = offset;
        attribute.format   = vkAttributeTypes.at(m_self.attributes[i].type);

        m_config.attributes.push_back(attribute);

        offset += m_self.attributes[i].size;
    }
}

}  // namespace sl::vk