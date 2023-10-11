#include "VKMaterialShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <kc/core/Log.h>

#include "VKShaderStage.h"
#include "VKPipeline.h"
#include "VKBuffer.h"
#include "VKTexture.h"

#include "starlight/resource/ResourceManager.h"

namespace sl::vk {

void VKMaterialShader::createGlobalDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = 0;
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
    };
    globalLayoutInfo.bindingCount = 1;
    globalLayoutInfo.pBindings    = &uboLayoutBinding;

    VK_ASSERT(vkCreateDescriptorSetLayout(
      m_device->getLogicalDevice(), &globalLayoutInfo, m_context->getAllocator(),
      &m_globalDescriptorSetLayout
    ));
}

void VKMaterialShader::createGlobalDescriptorPool(int swapchainImageCount) {
    // Global descriptor pool: Used for global items such as view/projection matrix.
    VkDescriptorPoolSize globalPoolSize;
    globalPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalPoolSize.descriptorCount = swapchainImageCount;

    VkDescriptorPoolCreateInfo globalPoolInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    };
    globalPoolInfo.poolSizeCount = 1;
    globalPoolInfo.pPoolSizes    = &globalPoolSize;
    globalPoolInfo.maxSets       = swapchainImageCount;

    VK_ASSERT(vkCreateDescriptorPool(
      m_device->getLogicalDevice(), &globalPoolInfo, m_context->getAllocator(),
      &m_globalDescriptorPool
    ));
}

void VKMaterialShader::createLocalDescriptorPool() {
    // Local/Object descriptor pool: Used for object-specific items like diffuse
    // colour
    VkDescriptorPoolSize objectPoolSizes[2];
    // The first section will be used for uniform buffers
    objectPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objectPoolSizes[0].descriptorCount = maxMaterialCount;
    // The second section will be used for image samplers.
    objectPoolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    objectPoolSizes[1].descriptorCount = samplerCount * maxMaterialCount;

    VkDescriptorPoolCreateInfo objectPoolInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    };
    objectPoolInfo.poolSizeCount = 2;
    objectPoolInfo.pPoolSizes    = objectPoolSizes;
    objectPoolInfo.maxSets       = maxMaterialCount;
    objectPoolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    // Create object descriptor pool.
    VK_ASSERT(vkCreateDescriptorPool(
      m_device->getLogicalDevice(), &objectPoolInfo, m_context->getAllocator(),
      &m_objectDescriptorPool
    ));
}

void VKMaterialShader::createLocalDescriptorSetLayout() {
    // Local/Object Descriptors

    VkDescriptorType descriptorTypes[descriptorCount] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // Binding 0 - uniform buffer
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // Binding 1 - Diffuse sampler
                                                    // layout.
    };
    VkDescriptorSetLayoutBinding bindings[descriptorCount];
    zeroMemory(bindings);

    for (uint32_t i = 0; i < descriptorCount; ++i) {
        bindings[i].binding         = i;
        bindings[i].descriptorCount = 1;
        bindings[i].descriptorType  = descriptorTypes[i];
        bindings[i].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
    };
    layoutInfo.bindingCount = descriptorCount;
    layoutInfo.pBindings    = bindings;

    VK_ASSERT(vkCreateDescriptorSetLayout(
      m_device->getLogicalDevice(), &layoutInfo, nullptr,
      &m_objectDescriptorSetLayout
    ));
}

void VKMaterialShader::createShaderStages() {
    m_stages.reserve(stagesCount);

    kc::core::FileSystem fs;
    m_stages.emplace_back(
      m_device, m_context,
      VKShaderStage::Properties{
        fs.readFile(
          "/home/nek0/kapik/projects/starlight/assets/shaders/MaterialShader.vert.spv"
        ),
        ShaderStage::vertex }
    );

    m_stages.emplace_back(
      m_device, m_context,
      VKShaderStage::Properties{
        fs.readFile(
          "/home/nek0/kapik/projects/starlight/assets/shaders/MaterialShader.frag.spv"
        ),
        ShaderStage::fragment }
    );
}

void VKMaterialShader::use(VKCommandBuffer& buffer) {
    m_pipeline->bind(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
}

void VKMaterialShader::createPipeline(
  const Size2u32& framebuffferSize, VKRenderPass& renderPass
) {
    // VKPipeline creation
    const auto& [w, h] = framebuffferSize;

    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = (float)w;
    viewport.width    = (float)w;
    viewport.height   = -(float)h;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = w;
    scissor.extent.height               = h;

    // Attributes
    uint32_t offset               = 0;
    const int32_t attribute_count = 2;
    std::vector<VkVertexInputAttributeDescription> attribute_descriptions(
      attribute_count
    );
    // Position

    VkFormat formats[attribute_count] = {
        VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32_SFLOAT
    };
    uint64_t sizes[attribute_count] = { sizeof(Vec3f), sizeof(Vec2f) };

    for (uint32_t i = 0; i < attribute_count; ++i) {
        attribute_descriptions[i].binding =
          0;  // binding index - should match binding desc
        attribute_descriptions[i].location = i;  // attrib location
        attribute_descriptions[i].format   = formats[i];
        attribute_descriptions[i].offset   = offset;
        offset += sizes[i];
    }

    // TODO: Desciptor set layouts.

    // Stages
    // NOTE: Should match the number of shader->stages.
    std::vector<VkPipelineShaderStageCreateInfo> stage_create_infos(2);
    const auto& shaderStages = m_stages;

    for (uint32_t i = 0; i < 2; ++i) {
        auto stageCreateInfo = shaderStages[i].getStageCreateInfo();

        stage_create_infos[i].sType = stageCreateInfo.sType;
        stage_create_infos[i]       = stageCreateInfo;
    }

    VKPipeline::Properties props;

    std::vector<VkDescriptorSetLayout> descriptorSetLayout = {
        m_globalDescriptorSetLayout, m_objectDescriptorSetLayout
    };

    // TODO: seems like a lot of copying, consider passing a vector view?
    props.vertexAttributes     = attribute_descriptions;
    props.stages               = stage_create_infos;
    props.stride               = sizeof(Vertex3);
    props.scissor              = scissor;
    props.viewport             = viewport;
    props.polygonMode          = VK_POLYGON_MODE_FILL;
    props.descriptorSetLayouts = descriptorSetLayout;
    props.depthTestEnabled     = true;
    props.pushConstantRanges   = {};

    m_pipeline = createUniqPtr<VKPipeline>(m_context, m_device, renderPass, props);

    LOG_DEBUG("VKPipeline created");
}

VKMaterialShader::VKMaterialShader(
  const VKContext* context, VKDevice* device, int swapchainImageCount,
  const Size2u32& framebuffferSize, VKRenderPass& renderPass
) :
    m_context(context),
    m_device(device) {
    LOG_TRACE("Creating VKMaterialShader instance");

    createShaderStages();
    createGlobalDescriptorSetLayout();
    createGlobalDescriptorPool(swapchainImageCount);

    m_samplerUses[0] = Texture::Use::diffuseMap;

    createLocalDescriptorSetLayout();
    createLocalDescriptorPool();
    createPipeline(framebuffferSize, renderPass);
    createUniformBuffer();
}

VKMaterialShader::~VKMaterialShader() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    LOG_TRACE("Destroying VKMaterialShader instance");

    if (m_globalDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_globalDescriptorPool, allocator);

    if (m_globalDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(
          logicalDevice, m_globalDescriptorSetLayout, allocator
        );

    if (m_objectDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_objectDescriptorPool, allocator);

    if (m_objectDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(
          logicalDevice, m_objectDescriptorSetLayout, allocator
        );
}

VkDescriptorSetLayout VKMaterialShader::getObjectDescriptorSetLayout() const {
    return m_objectDescriptorSetLayout;
}

void VKMaterialShader::setModel(VkCommandBuffer commandBuffer, const Mat4f& model) {
    vkCmdPushConstants(
      commandBuffer, m_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
      sizeof(model), glm::value_ptr(model)
    );
}

void VKMaterialShader::applyMaterial(
  VkCommandBuffer commandBuffer, uint32_t imageIndex, const Material& material
) {
    auto& instanceState       = m_instanceStates[material.internalId];
    auto& objectDescriptorSet = instanceState.descriptorSets[imageIndex];

    // TODO: if needs update
    VkWriteDescriptorSet descriptor_writes[descriptorCount];
    std::memset(
      &descriptor_writes, 0, sizeof(VkWriteDescriptorSet) * descriptorCount
    );

    uint32_t descriptor_count = 0;
    uint32_t descriptorIndex  = 0;

    // Descriptor 0 - Uniform buffer
    uint32_t range = sizeof(InstanceUBO);
    uint64_t offset =
      sizeof(InstanceUBO) * material.internalId;  // also the index into the
    InstanceUBO obo;

    obo.diffuseColor = material.diffuseColor;

    // Load the data into the buffer.
    m_objectUniformBuffer->loadData(offset, range, 0, &obo);

    // Only do this if the descriptor has not yet been updated.
    VkDescriptorBufferInfo bufferInfo;
    auto uboGeneration =
      instanceState.descriptorStates[descriptorIndex].generations[imageIndex];

    if (uboGeneration == invalidId || uboGeneration != material.generation) {
        bufferInfo.buffer = m_objectUniformBuffer->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range  = range;

        VkWriteDescriptorSet descriptor = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        descriptor.dstSet               = objectDescriptorSet;
        descriptor.dstBinding           = descriptorIndex;
        descriptor.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor.descriptorCount      = 1;
        descriptor.pBufferInfo          = &bufferInfo;

        descriptor_writes[descriptor_count] = descriptor;
        descriptor_count++;

        // Update the frame generation. In this case it is only needed once since
        // this is a
        uboGeneration = material.generation;
    }
    descriptorIndex++;

    const uint32_t samplerCount = 1;
    VkDescriptorImageInfo imageInfos[1];

    for (uint32_t samplerIndex = 0; samplerIndex < samplerCount; ++samplerIndex) {
        auto use     = m_samplerUses[samplerIndex];
        VKTexture* t = nullptr;

        switch (use) {
            case Texture::Use::diffuseMap:
                t = static_cast<VKTexture*>(material.diffuseMap.texture);
                break;
            case Texture::Use::unknown:
                LOG_FATAL("Unable to bind sampler to unknown use");
                return;
        }

        auto& descriptorState = instanceState.descriptorStates[descriptorIndex];

        uint32_t* generation = &descriptorState.generations[imageIndex];
        uint32_t* id         = &descriptorState.ids[imageIndex];

        // Check if the descriptor needs updating first.
        if (t && (*id != t->id || *generation != t->generation || *generation == invalidId)) {
            // Assign view and sampler.
            imageInfos[samplerIndex].imageLayout =
              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[samplerIndex].imageView = t->getImage()->getView();
            imageInfos[samplerIndex].sampler   = t->getSampler();

            VkWriteDescriptorSet descriptor = {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
            };
            descriptor.dstSet          = objectDescriptorSet;
            descriptor.dstBinding      = descriptorIndex;
            descriptor.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor.descriptorCount = 1;
            descriptor.pImageInfo      = &imageInfos[samplerIndex];

            descriptor_writes[descriptor_count] = descriptor;
            descriptor_count++;

            // Sync frame generation if not using a default texture.
            if (t->generation != invalidId) {
                *generation = t->generation;
                *id         = t->id;
            }
            descriptorIndex++;
        }
    }
    if (descriptor_count > 0) {
        vkUpdateDescriptorSets(
          m_device->getLogicalDevice(), descriptor_count, descriptor_writes, 0, 0
        );
    }

    // Bind the descriptor set to be updated, or in case the shader changed.
    vkCmdBindDescriptorSets(
      commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 1, 1,
      &objectDescriptorSet, 0, 0
    );
}

void VKMaterialShader::updateGlobalWorldState(
  VkCommandBuffer commandBuffer, uint32_t imageIndex
) {
    auto globalDescriptor = m_globalDescriptorSets[imageIndex];

    const uint32_t range = sizeof(GlobalUBO);
    uint64_t offset      = 0;

    m_globalUniformBuffer->loadData(offset, range, 0, (void*)&m_globalUBO);

    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_globalUniformBuffer->getHandle();
    bufferInfo.offset = offset;
    bufferInfo.range  = range;

    // Update descriptor sets.
    VkWriteDescriptorSet descriptor_write = {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
    };
    descriptor_write.dstSet          = globalDescriptor;
    descriptor_write.dstBinding      = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo     = &bufferInfo;

    vkUpdateDescriptorSets(m_device->getLogicalDevice(), 1, &descriptor_write, 0, 0);

    vkCmdBindDescriptorSets(
      commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 0, 1,
      &globalDescriptor, 0, 0
    );
}

void VKMaterialShader::createUniformBuffer() {
    VKBuffer::Properties props;

    props.size = sizeof(GlobalUBO);
    props.usageFlags =
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    props.memoryPropertyFlags =
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
      | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    props.bindOnCreate = true;

    m_globalUniformBuffer = createUniqPtr<VKBuffer>(m_context, m_device, props);

    props.size = sizeof(InstanceUBO) * maxMaterialCount;

    m_objectUniformBuffer = createUniqPtr<VKBuffer>(m_context, m_device, props);

    std::array<VkDescriptorSetLayout, 3> globalLayouts = {
        m_globalDescriptorSetLayout, m_globalDescriptorSetLayout,
        m_globalDescriptorSetLayout
    };

    VkDescriptorSetAllocateInfo alloc_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    };
    alloc_info.descriptorPool     = m_globalDescriptorPool;
    alloc_info.descriptorSetCount = 3;
    alloc_info.pSetLayouts        = globalLayouts.data();

    VK_ASSERT(vkAllocateDescriptorSets(
      m_device->getLogicalDevice(), &alloc_info, m_globalDescriptorSets
    ));
}

VkDescriptorSetLayout VKMaterialShader::getGlobalDescriptorSetLayout() const {
    return m_globalDescriptorSetLayout;
}

const std::vector<VKShaderStage>& VKMaterialShader::getStages() const {
    return m_stages;
}

VKMaterialShader::GlobalUBO& VKMaterialShader::getGlobalUBO() { return m_globalUBO; }

void VKMaterialShader::acquireResources(Material& material) {  // TODO: free list
    material.internalId = m_objectUniformBufferIndex;
    m_objectUniformBufferIndex++;

    auto& instanceState = m_instanceStates[material.internalId];

    for (uint32_t i = 0; i < descriptorCount; ++i) {
        for (uint32_t j = 0; j < 3; ++j) {
            instanceState.descriptorStates[i].generations[j] = invalidId;
            instanceState.descriptorStates[i].ids[j]         = invalidId;
        }
    }

    // Allocate descriptor sets.
    VkDescriptorSetLayout layouts[3] = {
        m_objectDescriptorSetLayout, m_objectDescriptorSetLayout,
        m_objectDescriptorSetLayout
    };

    VkDescriptorSetAllocateInfo alloc_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    };
    alloc_info.descriptorPool     = m_objectDescriptorPool;
    alloc_info.descriptorSetCount = 3;  // one per frame
    alloc_info.pSetLayouts        = layouts;

    VK_ASSERT(vkAllocateDescriptorSets(
      m_device->getLogicalDevice(), &alloc_info, instanceState.descriptorSets
    ));
}

void VKMaterialShader::releaseResources(Material& material) {
    auto& instanceState = m_instanceStates[material.internalId];

    vkDeviceWaitIdle(m_device->getLogicalDevice());

    const uint32_t descriptor_set_count = 3;
    // Release object descriptor sets.
    VkResult result = vkFreeDescriptorSets(
      m_device->getLogicalDevice(), m_objectDescriptorPool, descriptor_set_count,
      instanceState.descriptorSets
    );

    if (result != VK_SUCCESS)
        LOG_ERROR("Error freeing object shader descriptor sets!");

    for (uint32_t i = 0; i < descriptorCount; ++i) {
        for (uint32_t j = 0; j < 3; ++j) {
            instanceState.descriptorStates[i].generations[j] = invalidId;
            instanceState.descriptorStates[i].ids[j]         = invalidId;
        }
    }

    material.internalId = invalidId;
}

}  // namespace sl::vk
