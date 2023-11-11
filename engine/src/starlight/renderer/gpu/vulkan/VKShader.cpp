#include "VKShader.h"

#include <array>
#include <ranges>
#include <unordered_map>

#include "starlight/core/Core.h"
#include "starlight/core/window/WindowManager.h"
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

static const std::unordered_map<Shader::Stage::Type, VkShaderStageFlagBits>
  vkShaderStages = {
      {Shader::Stage::Type::fragment,  VK_SHADER_STAGE_FRAGMENT_BIT},
      { Shader::Stage::Type::geometry, VK_SHADER_STAGE_GEOMETRY_BIT},
      { Shader::Stage::Type::vertex,   VK_SHADER_STAGE_VERTEX_BIT  },
      { Shader::Stage::Type::compute,  VK_SHADER_STAGE_COMPUTE_BIT }
};

static const std::unordered_map<Shader::Attribute::Type, VkFormat>
  vkAttributeTypes = {
      {Shader::Attribute::Type::float32,    VK_FORMAT_R32_SFLOAT         },
      { Shader::Attribute::Type::float32_2, VK_FORMAT_R32G32_SFLOAT      },
      { Shader::Attribute::Type::float32_3, VK_FORMAT_R32G32B32_SFLOAT   },
      { Shader::Attribute::Type::float32_4, VK_FORMAT_R32G32B32A32_SFLOAT},
      { Shader::Attribute::Type::int8,      VK_FORMAT_R8_SINT            },
      { Shader::Attribute::Type::uint8,     VK_FORMAT_R8_UINT            },
      { Shader::Attribute::Type::int16,     VK_FORMAT_R16_SINT           },
      { Shader::Attribute::Type::uint16,    VK_FORMAT_R16_UINT           },
      { Shader::Attribute::Type::int32,     VK_FORMAT_R32_SINT           },
      { Shader::Attribute::Type::uint32,    VK_FORMAT_R32_UINT           },
};

VKShader::VKShader(
  VKDevice* device, const VKContext* context, VKRenderPass* renderPass,
  VKRendererContext& rendererContext, const Shader::Properties& props, u32 id
) :
    Shader(props, id),
    m_device(device), m_context(context), m_renderPass(renderPass),
    m_rendererContext(rendererContext), m_requiredUboAlignment(0),
    m_globalUboSize(0), m_globalUboStride(0), m_globalUboOffset(0), m_uboSize(0),
    m_uboStride(0), m_pushConstantSize(0), m_pushConstantStride(128),
    m_instanceTextureCount(0), m_boundInstanceId(0), m_boundUboOffset(0),
    m_pushConstantRangeCount(0), m_attributeStride(0), m_maxDescriptorSetCount(0),
    m_descriptorSetCount(0) {
    addAttributes(props.attributes);
    addUniforms(props.uniformProperties, props.defaultTexture);

    static constexpr int maxStages = 2;

    const auto stageCount = props.stages.size();

    ASSERT(
      stageCount <= maxStages, "Stage count {} exceeds maximum {}", stageCount,
      maxStages
    );

    static const u32 maxDescriptorAllocateCount = 1024;

    m_maxDescriptorSetCount = maxDescriptorAllocateCount;
    m_poolSizes[0] = VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 };
    m_poolSizes[1] =
      VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4096 };

    DescriptorSetConfig globalDescriptorSetConfig;
    globalDescriptorSetConfig.bindings[bindingIndexUBO] =
      VkDescriptorSetLayoutBinding{
          bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
          VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
      };
    globalDescriptorSetConfig.bindingCount++;
    m_descriptorSets[descSetIndexGlobal] = globalDescriptorSetConfig;
    m_descriptorSetCount++;

    if (m_useInstances) {
        DescriptorSetConfig instanceDescriptorSetConfig;
        instanceDescriptorSetConfig.bindings[bindingIndexUBO] =
          VkDescriptorSetLayoutBinding{
              bindingIndexUBO, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
              VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
          };
        instanceDescriptorSetConfig.bindingCount++;
        m_descriptorSets[descSetIndexInstance] = instanceDescriptorSetConfig;
        m_descriptorSetCount++;
    }
    m_instanceStates.resize(1024);

    createModules(props.stages);
    processAttributes();
    processUniforms();
    createDescriptorPool();
    createDescriptorSetLayouts();
    createPipeline();
    createUniformBuffer();

    LOG_DEBUG(
      "Shader: {}, global ubo size={} offset={}", props.name, m_globalUboSize,
      m_globalUboOffset
    );
}

VKShader::~VKShader() {
    const auto allocator     = m_context->getAllocator();
    const auto logicalDevice = m_device->getLogicalDevice();

    for (int i = 0; i < m_descriptorSetCount; ++i) {
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

void VKShader::addAttributes(std::span<const Shader::Attribute> attributes) {
    m_attributes.reserve(attributes.size());
    for (const auto& attribute : attributes) {
        m_attributeStride += attribute.size;
        m_attributes.push_back(attribute);
    }
}

void VKShader::addUniforms(
  std::span<const Shader::Uniform::Properties> uniforms, Texture* defaultTexture
) {
    for (const auto& uniformProps : uniforms) {
        ASSERT(
          not m_uniforms.contains(uniformProps.name), "Uniform '{}' already added",
          uniformProps.name
        );
        if (uniformProps.type == Uniform::Type::sampler) {
            addSampler(uniformProps, defaultTexture);
        } else {
            addUniform(
              uniformProps.name, uniformProps.size, uniformProps.type,
              uniformProps.scope, 0, false
            );
        }
    }
}

void VKShader::addSampler(
  const Shader::Uniform::Properties& props, Texture* defaultTexture
) {
    ASSERT(
      props.scope != Scope::instance || m_useInstances,
      "Cannot ad instance sampler for shader that doesn't support instances"
    );
    ASSERT(props.scope != Scope::local, "Samplers cannot be used at local scope");

    u32 location                             = 0;
    static constexpr int maxGlobalTextures   = 128;  // TODO: configurable
    static constexpr int maxInstanceTextures = 128;  // TODO: configurable

    if (props.scope == Scope::global) {
        const auto globalTextureCount = m_globalTextures.size();
        ASSERT(
          globalTextureCount + 1 <= maxGlobalTextures,
          "Shader global texture count {} exceed maximum {}", globalTextureCount + 1,
          globalTextureCount
        );
        location = globalTextureCount;
        m_globalTextures.push_back(defaultTexture);
    } else {
        ASSERT(
          m_instanceTextureCount + 1 <= maxInstanceTextures,
          "Shader instance texture count {} exceed maximum {}",
          m_instanceTextureCount + 1, maxInstanceTextures
        );
        location = m_instanceTextureCount;
        ++m_instanceTextureCount;
    }
    addUniform(props.name, 0, props.type, props.scope, location, true);
}

void VKShader::addUniform(
  const std::string& name, u32 size, Uniform::Type type, Scope scope,
  u32 setLocation, bool isSampler
) {
    static constexpr int maxUniforms = 32;  // TODO: configurable

    const auto uniformCount = m_uniforms.size();
    LOG_TRACE("Adding uniform: {}/{}/{}", name, size, scopeToString(scope));

    ASSERT(
      uniformCount + 1 <= maxUniforms, "Uniform count {} exceeds limit {}",
      uniformCount + 1, maxUniforms
    );

    Uniform uniform;

    uniform.index    = uniformCount;
    uniform.scope    = scope;
    uniform.type     = type;
    uniform.location = isSampler ? setLocation : uniform.index;

    if (scope != Scope::local) {
        uniform.setIndex = static_cast<u32>(scope);
        uniform.offset =
          isSampler ? 0
          : (scope == Scope::global)
            ? m_globalUboSize
            : m_uboSize;
        uniform.size = isSampler ? 0 : size;
        LOG_TRACE("Uniform {} offset: {}", name, uniform.offset);
    } else {
        ASSERT(
          scope != Scope::instance || m_useInstances,
          "Cannot add a local uniform for shader that doesn't support locals"
        );

        uniform.setIndex.invalidate();

        const auto r   = Range::aligned(m_pushConstantSize, size, 4);
        uniform.offset = r.offset;
        uniform.size   = r.size;
        m_pushConstantRanges[m_pushConstantRangeCount++] = r;
        m_pushConstantSize += r.size;

        LOG_TRACE(
          "Push constant range: {} - {}, total push constants: {}", r.offset, r.size,
          m_pushConstantRangeCount
        );
    }

    m_uniforms[name] = uniform;

    if (not isSampler) {
        if (uniform.scope == Scope::global)
            m_globalUboSize += uniform.size;
        else
            m_uboSize += uniform.size;
    }
}

void VKShader::use() {
    m_pipeline->bind(
      *m_rendererContext.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS
    );
}

void VKShader::bindGlobals() {
    // Global UBO is always at the beginning
    m_boundUboOffset = m_globalUboOffset;
}

void VKShader::bindInstance(u32 instanceId) {
    m_boundInstanceId = instanceId;
    m_boundUboOffset  = *m_instanceStates[instanceId].offset;
}

void VKShader::applyGlobals() {
    // // Apply UBO
    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_uniformBuffer->getHandle();
    bufferInfo.offset = m_globalUboOffset;
    bufferInfo.range  = m_globalUboStride;

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
      m_descriptorSets[descSetIndexGlobal].bindingCount;
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

void VKShader::applyInstance() {
    const auto imageIndex = m_rendererContext.getImageIndex();

    auto objectState = &m_instanceStates[m_boundInstanceId];
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
        bufferInfo.range  = m_uboStride;

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
    if (m_descriptorSets[descSetIndexInstance].bindingCount > 1) {
        // iterate samplers
        auto totalSamplerCount =
          m_descriptorSets[descSetIndexInstance]
            .bindings[bindingIndexSampler]
            .descriptorCount;
        imageInfos.reserve(totalSamplerCount);
        for (int i = 0; i < totalSamplerCount; ++i) {
            const VKTexture* texture = static_cast<const VKTexture*>(
              m_instanceStates[m_boundInstanceId].instanceTextures[i]
            );
            ASSERT(
              texture,
              "Could not cast texture to internal type, something went wrong"
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

u32 VKShader::acquireInstanceResources() {
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
      m_descriptorSets[descSetIndexInstance]
        .bindings[bindingIndexSampler]
        .descriptorCount;

    // todo: should we set all to default?
    instanceState.instanceTextures.resize(m_instanceTextureCount, nullptr);
    // allocate space in the UBO - by the stride, not the size
    instanceState.offset = m_uniformBuffer->allocate(m_uboStride);
    LOG_INFO(
      "Shader {} allocated offset={} for instance resources", m_name,
      instanceState.offset.get()
    );

    auto& setState          = instanceState.descriptorSetState;
    const auto bindingCount = m_descriptorSets[descSetIndexInstance].bindingCount;
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

void VKShader::releaseInstanceResources(u32 instanceId) {
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

    m_uniformBuffer->free(m_uboStride, *instanceState.offset);

    instanceState.id.invalidate();
    instanceState.offset.invalidate();
}

void VKShader::setUniform(const std::string& name, const void* value) {
    auto& uniform = m_uniforms[name];
    if (uniform.isSampler()) {
        if (uniform.scope == Scope::global) {
            m_globalTextures[uniform.location] = static_cast<const Texture*>(value);
        } else {
            m_instanceStates[m_boundInstanceId].instanceTextures[uniform.location] =
              static_cast<const Texture*>(value);
        }
    } else {
        if (uniform.scope == Scope::local) {
            vkCmdPushConstants(
              m_rendererContext.getCommandBuffer()->getHandle(),
              m_pipeline->getLayout(),
              VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
              uniform.offset, uniform.size, value
            );
        } else {
            const auto offset = m_boundUboOffset + uniform.offset;
            void* address     = static_cast<void*>(
              static_cast<char*>(m_mappedUniformBufferBlock) + offset
            );
            if (false) {
                LOG_TRACE(
                  "Setting uniform {}/{} offset={}, totalOffset={}",
                  uniform.typeToString(uniform.type), uniform.size, uniform.offset,
                  offset
                );
            }
            std::memcpy(address, value, uniform.size);
        }
    }
}

void VKShader::createModules(std::span<const Stage> stages) {
    m_stages.reserve(stages.size());
    for (const auto& stageConfig : stages) {
        m_stages.emplace_back(
          m_device, m_context,
          VKShaderStage::Properties{ stageConfig.source, stageConfig.type }
        );
    }
}

void VKShader::processUniforms() {
    for (const auto& uniform : m_uniforms | std::views::values) {
        if (uniform.isSampler()) {
            const auto setIndex =
              (uniform.scope == Scope::global
                 ? descSetIndexGlobal
                 : descSetIndexInstance);
            auto setConfig = &m_descriptorSets[setIndex];

            if (setConfig->bindingCount < 2) {
                // no binding yet, this is first sampler to be added
                // create a binding with a single descriptor for this sampler
                auto& binding           = setConfig->bindings[bindingIndexSampler];
                binding.binding         = bindingIndexSampler;
                binding.descriptorCount = 1;
                binding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                binding.stageFlags =
                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                binding.pImmutableSamplers = nullptr;

                setConfig->bindingCount++;
            } else {
                setConfig->bindings[bindingIndexSampler].descriptorCount++;
            }
        }
    }
}

void VKShader::createDescriptorPool() {
    VkDescriptorPoolCreateInfo poolInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    };
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes    = m_poolSizes.data();
    poolInfo.maxSets       = m_maxDescriptorSetCount;
    poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    VK_ASSERT(vkCreateDescriptorPool(
      m_device->getLogicalDevice(), &poolInfo, m_context->getAllocator(),
      &m_descriptorPool
    ));
}

void VKShader::createDescriptorSetLayouts() {
    for (int i = 0; i < m_descriptorSetCount; ++i) {
        LOG_TRACE(
          "Creating descriptor set layout: {} - bindings: {}", i,
          m_descriptorSets[i].bindingCount
        );
        VkDescriptorSetLayoutCreateInfo info = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
        };
        info.bindingCount = m_descriptorSets[i].bindingCount;
        info.pBindings    = m_descriptorSets[i].bindings.data();

        VK_ASSERT(vkCreateDescriptorSetLayout(
          m_device->getLogicalDevice(), &info, m_context->getAllocator(),
          &m_descriptorSetLayouts[i]
        ));
    }
}

void VKShader::createPipeline() {
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
    pipelineProps.stride               = m_attributeStride;
    pipelineProps.vertexAttributes     = m_attributeDescriptions;
    pipelineProps.descriptorSetLayouts = m_descriptorSetLayouts;
    pipelineProps.stages               = stageCreateInfos;
    pipelineProps.viewport             = viewport;
    pipelineProps.scissor              = scissor;
    pipelineProps.polygonMode          = VK_POLYGON_MODE_FILL;
    pipelineProps.depthTestEnabled     = true;

    // todo: use vector
    pipelineProps.pushConstantRanges = {
        m_pushConstantRanges.data(),
        m_pushConstantRanges.data() + m_pushConstantRangeCount
    };

    m_pipeline.emplace(m_context, m_device, *m_renderPass, pipelineProps);
}

void VKShader::createUniformBuffer() {
    m_requiredUboAlignment =
      m_device->getProperties().limits.minUniformBufferOffsetAlignment;
    m_globalUboStride = getAlignedValue(m_globalUboSize, m_requiredUboAlignment);
    m_uboStride       = getAlignedValue(m_uboSize, m_requiredUboAlignment);

    LOG_INFO("Minimal uniform buffer offset alignment={}", m_requiredUboAlignment);

    const u32 deviceLocalBits =
      m_device->supportsDeviceLocalHostVisible()
        ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        : 0;

    // material max count should be configurable!
    const u64 totalBufferSize = m_globalUboStride + (m_uboStride * 1024);

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

    LOG_TRACE("Allocating {}b of memory", m_globalUboStride);
    m_globalUboOffset          = m_uniformBuffer->allocate(m_globalUboStride);
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

void VKShader::processAttributes() {
    u32 offset                = 0;
    const auto attributeCount = m_attributes.size();
    m_attributeDescriptions.reserve(attributeCount);

    for (int i = 0; i < attributeCount; ++i) {
        VkVertexInputAttributeDescription attribute;
        attribute.location = i;
        attribute.binding  = 0;
        attribute.offset   = offset;
        attribute.format   = vkAttributeTypes.at(m_attributes[i].type);

        m_attributeDescriptions.push_back(attribute);

        offset += m_attributes[i].size;
    }
}

}  // namespace sl::vk