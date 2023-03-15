#include "MaterialShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <kc/core/Log.h>

#include "ShaderStage.h"
#include "Pipeline.h"
#include "Buffer.h"
#include "Texture.h"

namespace nova::platform::vulkan {

void MaterialShader::createGlobalDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = 0;
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    globalLayoutInfo.bindingCount = 1;
    globalLayoutInfo.pBindings    = &uboLayoutBinding;

    VK_ASSERT(vkCreateDescriptorSetLayout(
        m_device->getLogicalDevice(), &globalLayoutInfo, m_context->getAllocator(),
        &m_globalDescriptorSetLayout
    ));
}

void MaterialShader::createGlobalDescriptorPool(int swapchainImageCount) {
    // Global descriptor pool: Used for global items such as view/projection matrix.
    VkDescriptorPoolSize globalPoolSize;
    globalPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalPoolSize.descriptorCount = swapchainImageCount;

    VkDescriptorPoolCreateInfo globalPoolInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    globalPoolInfo.poolSizeCount              = 1;
    globalPoolInfo.pPoolSizes                 = &globalPoolSize;
    globalPoolInfo.maxSets                    = swapchainImageCount;

    VK_ASSERT(vkCreateDescriptorPool(
        m_device->getLogicalDevice(), &globalPoolInfo, m_context->getAllocator(),
        &m_globalDescriptorPool
    ));
}

void MaterialShader::createLocalDescriptorPool() {
    // Local/Object descriptor pool: Used for object-specific items like diffuse colour
    VkDescriptorPoolSize objectPoolSizes[2];
    // The first section will be used for uniform buffers
    objectPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objectPoolSizes[0].descriptorCount = VULKAN_MAX_MATERIAL_COUNT;
    // The second section will be used for image samplers.
    objectPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    objectPoolSizes[1].descriptorCount =
        VULKAN_MATERIAL_SHADER_SAMPLER_COUNT * VULKAN_MAX_MATERIAL_COUNT;

    VkDescriptorPoolCreateInfo objectPoolInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    objectPoolInfo.poolSizeCount              = 2;
    objectPoolInfo.pPoolSizes                 = objectPoolSizes;
    objectPoolInfo.maxSets                    = VULKAN_MAX_MATERIAL_COUNT;
    objectPoolInfo.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    // Create object descriptor pool.
    VK_ASSERT(vkCreateDescriptorPool(
        m_device->getLogicalDevice(), &objectPoolInfo, m_context->getAllocator(),
        &m_objectDescriptorPool
    ));
}

void MaterialShader::createLocalDescriptorSetLayout() {
    // Local/Object Descriptors

    VkDescriptorType descriptorTypes[VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // Binding 0 - uniform buffer
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // Binding 1 - Diffuse sampler layout.
    };
    VkDescriptorSetLayoutBinding bindings[VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT];
    zeroMemory(bindings);

    for (uint32_t i = 0; i < VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT; ++i) {
        bindings[i].binding         = i;
        bindings[i].descriptorCount = 1;
        bindings[i].descriptorType  = descriptorTypes[i];
        bindings[i].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    layoutInfo.bindingCount = VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT;
    layoutInfo.pBindings    = bindings;

    VK_ASSERT(vkCreateDescriptorSetLayout(
        m_device->getLogicalDevice(), &layoutInfo, nullptr, &m_objectDescriptorSetLayout
    ));
}

void MaterialShader::createShaderStages() {
    core::FileSystem fs;
    m_stages.reserve(s_stagesCount);

    m_stages.emplace_back(
        m_device, m_context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::vertex}
    );

    m_stages.emplace_back(
        m_device, m_context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::fragment}
    );
}

MaterialShader::MaterialShader(const Context* context, Device* device, int swapchainImageCount)
    : m_context(context), m_device(device) {
    LOG_TRACE("Creating MaterialShader instance");

    createShaderStages();
    createGlobalDescriptorSetLayout();
    createGlobalDescriptorPool(swapchainImageCount);

    m_samplerUses[0] = gfx::Texture::Use::diffuseMap;

    createLocalDescriptorSetLayout();
    createLocalDescriptorPool();
}

MaterialShader::~MaterialShader() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    LOG_TRACE("Destroying MaterialShader instance");

    if (m_globalDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_globalDescriptorPool, allocator);

    if (m_globalDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(logicalDevice, m_globalDescriptorSetLayout, allocator);

    if (m_objectDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_objectDescriptorPool, allocator);

    if (m_objectDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(logicalDevice, m_objectDescriptorSetLayout, allocator);
}

VkDescriptorSetLayout MaterialShader::getObjectDescriptorSetLayout() const {
    return m_objectDescriptorSetLayout;
}

void MaterialShader::drawGeometry(
    Pipeline& pipeline, VkCommandBuffer commandBuffer, const gfx::GeometryRenderData& renderData,
    uint32_t imageIndex
) {
    // TODO: refactor it for God sake
    vkCmdPushConstants(
        commandBuffer, pipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
        sizeof(renderData.model), glm::value_ptr(renderData.model)
    );

    // Obtain material data.
    auto& material = renderData.material;

    auto& instanceState       = m_instanceStates[material->internalId];
    auto& objectDescriptorSet = instanceState.descriptorSets[imageIndex];

    // TODO: if needs update
    VkWriteDescriptorSet descriptor_writes[VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT];
    std::memset(
        &descriptor_writes, 0,
        sizeof(VkWriteDescriptorSet) * VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT
    );

    uint32_t descriptor_count = 0;
    uint32_t descriptorIndex  = 0;

    // Descriptor 0 - Uniform buffer
    uint32_t range = sizeof(gfx::MaterialUniformObject);
    uint64_t offset =
        sizeof(gfx::MaterialUniformObject) * material->internalId;  // also the index into the
    gfx::MaterialUniformObject obo;

    obo.diffuseColor = material->diffuseColor;

    // Load the data into the buffer.
    m_objectUniformBuffer->loadData(offset, range, 0, &obo);

    // Only do this if the descriptor has not yet been updated.
    VkDescriptorBufferInfo bufferInfo;
    auto uboGeneration = instanceState.descriptorStates[descriptorIndex].generations[imageIndex];

    if (uboGeneration == core::invalidId || uboGeneration != material->generation) {
        bufferInfo.buffer = m_objectUniformBuffer->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range  = range;

        VkWriteDescriptorSet descriptor = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        descriptor.dstSet               = objectDescriptorSet;
        descriptor.dstBinding           = descriptorIndex;
        descriptor.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor.descriptorCount      = 1;
        descriptor.pBufferInfo          = &bufferInfo;

        descriptor_writes[descriptor_count] = descriptor;
        descriptor_count++;

        // Update the frame generation. In this case it is only needed once since this is a
        uboGeneration = material->generation;
    }
    descriptorIndex++;

    const uint32_t samplerCount = 1;
    VkDescriptorImageInfo imageInfos[1];

    for (uint32_t samplerIndex = 0; samplerIndex < samplerCount; ++samplerIndex) {
        auto use   = m_samplerUses[samplerIndex];
        Texture* t = nullptr;

        switch (use) {
            case gfx::Texture::Use::diffuseMap:
                t = static_cast<Texture*>(material->diffuseMap.texture);
                break;
            case gfx::Texture::Use::unknown:
                LOG_FATAL("Unable to bind sampler to unknown use");
                return;
        }

        auto& descriptorState = instanceState.descriptorStates[descriptorIndex];

        uint32_t* generation = &descriptorState.generations[imageIndex];
        uint32_t* id         = &descriptorState.ids[imageIndex];

        // Check if the descriptor needs updating first.
        if (t && (*id != t->id || *generation != t->generation || *generation == core::invalidId)) {
            // Assign view and sampler.
            imageInfos[samplerIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[samplerIndex].imageView   = t->getImage()->getView();
            imageInfos[samplerIndex].sampler     = t->getSampler();

            VkWriteDescriptorSet descriptor = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
            descriptor.dstSet               = objectDescriptorSet;
            descriptor.dstBinding           = descriptorIndex;
            descriptor.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor.descriptorCount      = 1;
            descriptor.pImageInfo           = &imageInfos[samplerIndex];

            descriptor_writes[descriptor_count] = descriptor;
            descriptor_count++;

            // Sync frame generation if not using a default texture.
            if (t->generation != core::invalidId) {
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
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 1, 1,
        &objectDescriptorSet, 0, 0
    );
}

void MaterialShader::updateGlobalState(
    VkCommandBuffer commandBuffer, uint32_t imageIndex, Pipeline& pipeline
) {
    auto globalDescriptor = m_globalDescriptorSets[imageIndex];

    const uint32_t range = sizeof(gfx::GlobalUniformObject);
    uint64_t offset      = 0;

    m_globalUniformBuffer->loadData(offset, range, 0, (void*)&m_globalUBO);

    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_globalUniformBuffer->getHandle();
    bufferInfo.offset = offset;
    bufferInfo.range  = range;

    // Update descriptor sets.
    VkWriteDescriptorSet descriptor_write = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    descriptor_write.dstSet               = globalDescriptor;
    descriptor_write.dstBinding           = 0;
    descriptor_write.dstArrayElement      = 0;
    descriptor_write.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount      = 1;
    descriptor_write.pBufferInfo          = &bufferInfo;

    vkUpdateDescriptorSets(m_device->getLogicalDevice(), 1, &descriptor_write, 0, 0);

    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1,
        &globalDescriptor, 0, 0
    );
}

void MaterialShader::createUniformBuffer() {
    Buffer::Properties props;

    props.size       = sizeof(gfx::GlobalUniformObject);
    props.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    props.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    props.bindOnCreate = true;

    m_globalUniformBuffer = core::createUniqPtr<Buffer>(m_context, m_device, props);

    props.size = sizeof(gfx::MaterialUniformObject) * VULKAN_MAX_MATERIAL_COUNT;

    m_objectUniformBuffer = core::createUniqPtr<Buffer>(m_context, m_device, props);

    std::array<VkDescriptorSetLayout, 3> globalLayouts = {
        m_globalDescriptorSetLayout, m_globalDescriptorSetLayout, m_globalDescriptorSetLayout};

    VkDescriptorSetAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    alloc_info.descriptorPool              = m_globalDescriptorPool;
    alloc_info.descriptorSetCount          = 3;
    alloc_info.pSetLayouts                 = globalLayouts.data();

    VK_ASSERT(
        vkAllocateDescriptorSets(m_device->getLogicalDevice(), &alloc_info, m_globalDescriptorSets)
    );
}

VkDescriptorSetLayout MaterialShader::getGlobalDescriptorSetLayout() const {
    return m_globalDescriptorSetLayout;
}

const std::vector<ShaderStage>& MaterialShader::getStages() const { return m_stages; }

gfx::GlobalUniformObject& MaterialShader::getGlobalUBO() { return m_globalUBO; }

void MaterialShader::acquireResources(gfx::Material& material) {  // TODO: free list
    material.internalId = m_objectUniformBufferIndex;
    m_objectUniformBufferIndex++;

    auto& instanceState = m_instanceStates[material.internalId];

    for (uint32_t i = 0; i < VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT; ++i) {
        for (uint32_t j = 0; j < 3; ++j) {
            instanceState.descriptorStates[i].generations[j] = core::invalidId;
            instanceState.descriptorStates[i].ids[j]         = core::invalidId;
        }
    }

    // Allocate descriptor sets.
    VkDescriptorSetLayout layouts[3] = {
        m_objectDescriptorSetLayout, m_objectDescriptorSetLayout, m_objectDescriptorSetLayout};

    VkDescriptorSetAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    alloc_info.descriptorPool              = m_objectDescriptorPool;
    alloc_info.descriptorSetCount          = 3;  // one per frame
    alloc_info.pSetLayouts                 = layouts;

    VK_ASSERT(vkAllocateDescriptorSets(
        m_device->getLogicalDevice(), &alloc_info, instanceState.descriptorSets
    ));
}

void MaterialShader::releaseResources(gfx::Material& material) {
    auto& instanceState = m_instanceStates[material.internalId];

    const uint32_t descriptor_set_count = 3;
    // Release object descriptor sets.
    VkResult result = vkFreeDescriptorSets(
        m_device->getLogicalDevice(), m_objectDescriptorPool, descriptor_set_count,
        instanceState.descriptorSets
    );

    if (result != VK_SUCCESS) LOG_ERROR("Error freeing object shader descriptor sets!");

    for (uint32_t i = 0; i < VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT; ++i) {
        for (uint32_t j = 0; j < 3; ++j) {
            instanceState.descriptorStates[i].generations[j] = core::invalidId;
            instanceState.descriptorStates[i].ids[j]         = core::invalidId;
        }
    }

    material.internalId = core::invalidId;
}

}  // namespace nova::platform::vulkan
