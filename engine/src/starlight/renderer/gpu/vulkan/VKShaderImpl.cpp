#include "VKShaderImpl.h"

#include <array>
#include <ranges>
#include <unordered_map>

#include "starlight/core/Core.h"
#include "starlight/core/window/WindowManager.h"
#include "starlight/renderer/ShaderStage.h"
#include "starlight/renderer/Texture.h"

#include "VKTexture.h"
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
  VKRenderPass* renderPass, VKRendererContext& rendererContext
) :
    m_self(self),
    m_device(device), m_context(context), m_renderPass(renderPass),
    m_rendererContext(rendererContext) {
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

VKShaderImpl::~VKShaderImpl() {
    const auto allocator     = m_context->getAllocator();
    const auto logicalDevice = m_device->getLogicalDevice();

    for (int i = 0; i < m_config.descriptorSetCount; ++i) {
        if (m_descriptorSetLayouts[i]) {
            vkDestroyDescriptorSetLayout(
              logicalDevice, m_descriptorSetLayouts[i], allocator
            );
        }
    }
    if (m_descriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, allocator);
    m_uniformBuffer->unlockMemory();
    m_mappedUniformBufferBlock = nullptr;
}

void VKShaderImpl::initialize() {
    createModules();
    processAttributes();
    processUniforms();
    createDescriptorPool();
    createDescriptorSetLayouts();
    createPipeline();
    createUniformBuffer();
}

void VKShaderImpl::use() {
    m_pipeline->bind(
      *m_rendererContext.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS
    );
}

void VKShaderImpl::bindGlobals() {
    // Global UBO is always at the beginning
    m_self.boundUboOffset = m_self.globalUboOffset;
}

void VKShaderImpl::bindInstance(u32 instanceId) {
    m_self.boundInstanceId = instanceId;
    m_self.boundUboOffset  = *m_instanceStates[instanceId].offset;
    LOG_TRACE("Binding instance: {}, offset={}", instanceId, m_self.boundUboOffset);
}

void VKShaderImpl::applyGlobals() {
    // Apply UBO
    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_uniformBuffer->getHandle();
    bufferInfo.offset = m_self.globalUboOffset;
    bufferInfo.range  = m_self.globalUboStride;

    const auto imageIndex = m_rendererContext.getImageIndex();

    // Update desriptor sets
    VkWriteDescriptorSet uboWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
    uboWrite.dstSet               = m_globalDescriptorSets[imageIndex];
    uboWrite.dstBinding           = 0;
    uboWrite.dstArrayElement      = 0;
    uboWrite.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboWrite.descriptorCount      = 1;
    uboWrite.pBufferInfo          = &bufferInfo;

    std::array<VkWriteDescriptorSet, 2> descriptorWrites;
    descriptorWrites[0] = uboWrite;

    const auto globalSetBindingCount =
      m_config.descriptorSets[descSetIndexGlobal].bindingCount;
    ASSERT(globalSetBindingCount <= 1, "Global image samplers not supported yet");

    vkUpdateDescriptorSets(
      m_device->getLogicalDevice(), globalSetBindingCount, descriptorWrites.data(),
      0, 0
    );

    auto globalDescriptor = &m_globalDescriptorSets[imageIndex];
    vkCmdBindDescriptorSets(
      m_rendererContext.getCommandBuffer()->getHandle(),
      VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 0, 1,
      globalDescriptor, 0, 0
    );
}

void VKShaderImpl::applyInstance() {
    const auto imageIndex = m_rendererContext.getImageIndex();

    auto objectState = &m_instanceStates[*m_self.boundInstanceId];
    auto objectDescriptorSet =
      objectState->descriptorSetState.descriptorSets[imageIndex];

    std::array<VkWriteDescriptorSet, 2> descriptorWrites;

    u32 descriptorCount = 0;
    u32 descriptorIndex = 0;

    // 0 - uniform buffer
    auto& instanceUboGeneration =
      objectState->descriptorSetState.descriptorStates[descriptorIndex]
        .generations[imageIndex];

    VkDescriptorBufferInfo bufferInfo;
    if (not instanceUboGeneration.hasValue()) {
        bufferInfo.buffer = m_uniformBuffer->getHandle();
        bufferInfo.offset = *objectState->offset;
        bufferInfo.range  = m_self.uboStride;

        VkWriteDescriptorSet uboDescriptor = {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
        };
        uboDescriptor.dstSet          = objectDescriptorSet;
        uboDescriptor.dstBinding      = descriptorIndex;
        uboDescriptor.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboDescriptor.pBufferInfo     = &bufferInfo;
        uboDescriptor.descriptorCount = 1;

        descriptorWrites[descriptorCount++] = uboDescriptor;
        instanceUboGeneration               = 1;
    }
    descriptorIndex++;

    // Samplers will always be in the binding. If the binding count is less than 2,
    // there are no samplers
    std::vector<VkDescriptorImageInfo> imageInfos;
    if (m_config.descriptorSets[descSetIndexInstance].bindingCount > 1) {
        // iterate samplers
        auto totalSamplerCount =
          m_config.descriptorSets[descSetIndexInstance]
            .bindings[bindingIndexSampler]
            .descriptorCount;

        imageInfos.reserve(totalSamplerCount);
        for (int i = 0; i < totalSamplerCount; ++i) {
            VKTexture* texture = static_cast<VKTexture*>(
              m_instanceStates[*m_self.boundInstanceId].instanceTextures[i]
            );
            ASSERT(
              texture,
              "Could not cast texture to internal type, something went wrong, is texture pointer null: {}",
              m_instanceStates[*m_self.boundInstanceId].instanceTextures[i]
                == nullptr
            );
            imageInfos.emplace_back(
              texture->getSampler(), texture->getImage()->getView(),
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );
        }

        VkWriteDescriptorSet samplerDescriptor = {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
        };
        samplerDescriptor.dstSet         = objectDescriptorSet;
        samplerDescriptor.dstBinding     = descriptorIndex;
        samplerDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerDescriptor.descriptorCount = totalSamplerCount;
        samplerDescriptor.pImageInfo      = imageInfos.data();

        descriptorWrites[descriptorCount++] = samplerDescriptor;
    }

    if (descriptorCount > 0) {
        vkUpdateDescriptorSets(
          m_device->getLogicalDevice(), descriptorCount, descriptorWrites.data(), 0,
          0
        );
    }
    vkCmdBindDescriptorSets(
      m_rendererContext.getCommandBuffer()->getHandle(),
      VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 1, 1,
      &objectDescriptorSet, 0, 0
    );
}

u32 VKShaderImpl::acquireInstanceResources() {
    Id32 id = 0;

    for (int i = 0; i < 1024; ++i) {
        if (not m_instanceStates[i].id.hasValue()) {
            id                     = i;
            m_instanceStates[i].id = i;
            break;
        }
    }
    ASSERT(id.hasValue(), "Coult not acquire new resource id");

    auto& instanceState = m_instanceStates[*id];
    const auto instanceTextureCount =
      m_config.descriptorSets[descSetIndexInstance]
        .bindings[bindingIndexSampler]
        .descriptorCount;

    // todo: should we set all to default?
    instanceState.instanceTextures.resize(m_self.instanceTextureCount, nullptr);
    // allocate space in the UBO - by the stride, not the size
    instanceState.offset = m_uniformBuffer->allocate(m_self.uboStride);

    auto& setState = instanceState.descriptorSetState;
    const auto bindingCount =
      m_config.descriptorSets[descSetIndexInstance].bindingCount;
    setState.descriptorStates.resize(maxBindings);

    // allocate 3 descirptor sets, one per frame
    std::array<VkDescriptorSetLayout, 3> layouts = {
        m_descriptorSetLayouts[descSetIndexInstance],
        m_descriptorSetLayouts[descSetIndexInstance],
        m_descriptorSetLayouts[descSetIndexInstance]
    };

    VkDescriptorSetAllocateInfo allocateInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    };
    allocateInfo.descriptorPool     = m_descriptorPool;
    allocateInfo.descriptorSetCount = 3;
    allocateInfo.pSetLayouts        = layouts.data();

    VK_ASSERT(vkAllocateDescriptorSets(
      m_device->getLogicalDevice(), &allocateInfo,
      instanceState.descriptorSetState.descriptorSets.data()
    ));

    return *id;
}

void VKShaderImpl::releaseInstanceResources(u32 instanceId) {
    const auto logicalDevice = m_device->getLogicalDevice();
    vkDeviceWaitIdle(logicalDevice);

    auto& instanceState = m_instanceStates[instanceId];

    VK_ASSERT(vkFreeDescriptorSets(
      logicalDevice, m_descriptorPool, 3,
      instanceState.descriptorSetState.descriptorSets.data()
    ));

    // TODO: clear in place instead
    instanceState.descriptorSetState.descriptorStates.clear();
    instanceState.descriptorSetState.descriptorStates.resize(maxBindings);
    instanceState.instanceTextures.clear();

    m_uniformBuffer->free(m_self.uboStride, *instanceState.offset);

    instanceState.id.invalidate();
    instanceState.offset.invalidate();
}

void VKShaderImpl::setUniform(const ShaderUniform& uniform, void* value) {
    if (uniform.isSampler()) {
        if (uniform.scope == ShaderScope::global) {
            m_self.globalTextures[uniform.location] = static_cast<Texture*>(value);
        } else {
            m_instanceStates[*m_self.boundInstanceId]
              .instanceTextures[uniform.location] = static_cast<Texture*>(value);
        }
    } else {
        if (uniform.scope == ShaderScope::local) {
            vkCmdPushConstants(
              m_rendererContext.getCommandBuffer()->getHandle(),
              m_pipeline->getLayout(),
              VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
              uniform.offset, uniform.size, value
            );
        } else {
            const auto totalOffset = m_self.boundUboOffset + uniform.offset;
            LOG_TRACE(
              "Setting uniform {} size {}, bound offset = {}, total offset = {}",
              uniform.typeToString(uniform.type), uniform.size,
              m_self.boundUboOffset, totalOffset
            );
            char* address =
              static_cast<char*>(m_mappedUniformBufferBlock) + totalOffset;
            std::memcpy(address, value, uniform.size);
        }
    }
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
        if (uniform.isSampler()) {
            const auto setIndex =
              (uniform.scope == ShaderScope::global
                 ? descSetIndexGlobal
                 : descSetIndexInstance);
            auto setConfig = &m_config.descriptorSets[setIndex];

            if (setConfig->bindingCount < 2) {
                // no binding yet, this is first sampler to be added
                // create a binding with a single descriptor for this sampler
                auto& binding           = setConfig->bindings[bindingIndexSampler];
                binding.binding         = bindingIndexSampler;
                binding.descriptorCount = 1;
                binding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                binding.stageFlags =
                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                setConfig->bindingCount++;
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
        LOG_TRACE(
          "Creating descriptor set layout: {} - bindings: {}", i,
          m_config.descriptorSets[i].bindingCount
        );
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

void VKShaderImpl::createPipeline() {
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

    // todo: use vector
    pipelineProps.pushConstantRanges = {
        m_self.pushConstantRanges.data(),
        m_self.pushConstantRanges.data() + m_self.pushConstantRangeCount
    };

    m_pipeline.emplace(m_context, m_device, *m_renderPass, pipelineProps);
}

void VKShaderImpl::createUniformBuffer() {
    m_self.requiredUboAlignment =
      m_device->getProperties().limits.minUniformBufferOffsetAlignment;
    m_self.globalUboStride =
      getAlignedValue(m_self.globalUboSize, m_self.requiredUboAlignment);
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
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferProps.bindOnCreate = true;

    LOG_TRACE("Creating uniform buffer");
    m_uniformBuffer.emplace(m_context, m_device, bufferProps);

    LOG_TRACE("Allocating {}b of memory", m_self.globalUboStride);
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