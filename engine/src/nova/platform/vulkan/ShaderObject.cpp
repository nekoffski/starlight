#include "ShaderObject.h"

#include <glm/gtc/type_ptr.hpp>

#include "ShaderStage.h"
#include "Pipeline.h"
#include "Buffer.h"
#include "Texture.h"

namespace nova::platform::vulkan {

ShaderObject::ShaderObject(const Context* context, Device* device, int swapchainImageCount)
    : m_context(context), m_device(device) {
    static std::string defaultVertexShader = "Simple.vert";
    static std::string defaultPixelShader  = "Simple.frag";

    core::FileSystem fs;

    m_stages.reserve(s_stagesCount);

    m_stages.emplace_back(
        device, context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::vertex}
    );

    m_stages.emplace_back(
        device, context, &fs, ShaderStage::Properties{"Simple", ShaderStage::Type::fragment}
    );

    auto logicalDevice = device->getLogicalDevice();
    auto allocator     = context->getAllocator();

    VkDescriptorSetLayoutBinding global_ubo_layout_binding;
    global_ubo_layout_binding.binding            = 0;
    global_ubo_layout_binding.descriptorCount    = 1;
    global_ubo_layout_binding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    global_ubo_layout_binding.pImmutableSamplers = 0;
    global_ubo_layout_binding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo global_layout_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    global_layout_info.bindingCount = 1;
    global_layout_info.pBindings    = &global_ubo_layout_binding;

    VK_ASSERT(vkCreateDescriptorSetLayout(
        logicalDevice, &global_layout_info, allocator, &m_globalDescriptorSetLayout
    ));

    // Global descriptor pool: Used for global items such as view/projection matrix.
    VkDescriptorPoolSize global_pool_size;
    global_pool_size.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    global_pool_size.descriptorCount = swapchainImageCount;

    VkDescriptorPoolCreateInfo global_pool_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    global_pool_info.poolSizeCount              = 1;
    global_pool_info.pPoolSizes                 = &global_pool_size;
    global_pool_info.maxSets                    = swapchainImageCount;

    VK_ASSERT(
        vkCreateDescriptorPool(logicalDevice, &global_pool_info, allocator, &m_globalDescriptorPool)
    );

    // Local/Object Descriptors
    const uint32_t local_sampler_count                                       = 1;
    VkDescriptorType descriptor_types[VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // Binding 0 - uniform buffer
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // Binding 1 - Diffuse sampler layout.
    };
    VkDescriptorSetLayoutBinding bindings[VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT];
    zeroMemory(bindings);

    for (uint32_t i = 0; i < VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT; ++i) {
        bindings[i].binding         = i;
        bindings[i].descriptorCount = 1;
        bindings[i].descriptorType  = descriptor_types[i];
        bindings[i].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    VkDescriptorSetLayoutCreateInfo layout_info = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    layout_info.bindingCount = VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT;
    layout_info.pBindings    = bindings;

    VK_ASSERT(
        vkCreateDescriptorSetLayout(logicalDevice, &layout_info, 0, &m_objectDescriptorSetLayout)
    );

    // Local/Object descriptor pool: Used for object-specific items like diffuse colour
    VkDescriptorPoolSize object_pool_sizes[2];
    // The first section will be used for uniform buffers
    object_pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    object_pool_sizes[0].descriptorCount = VULKAN_OBJECT_MAX_OBJECT_COUNT;
    // The second section will be used for image samplers.
    object_pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    object_pool_sizes[1].descriptorCount = local_sampler_count * VULKAN_OBJECT_MAX_OBJECT_COUNT;

    VkDescriptorPoolCreateInfo object_pool_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    object_pool_info.poolSizeCount              = 2;
    object_pool_info.pPoolSizes                 = object_pool_sizes;
    object_pool_info.maxSets                    = VULKAN_OBJECT_MAX_OBJECT_COUNT;

    // Create object descriptor pool.
    VK_ASSERT(
        vkCreateDescriptorPool(logicalDevice, &object_pool_info, allocator, &m_objectDescriptorPool)
    );
}

ShaderObject::~ShaderObject() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    if (m_globalDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_globalDescriptorPool, allocator);

    if (m_globalDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(logicalDevice, m_globalDescriptorSetLayout, allocator);

    if (m_objectDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_objectDescriptorPool, allocator);

    if (m_objectDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(logicalDevice, m_objectDescriptorSetLayout, allocator);
}

void ShaderObject::updateObject(
    Pipeline& pipeline, VkCommandBuffer commandBuffer, const gfx::GeometryRenderData& renderData,
    uint32_t imageIndex
) {
    vkCmdPushConstants(
        commandBuffer, pipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
        sizeof(renderData.model), glm::value_ptr(renderData.model)
    );

    // Obtain material data.

    auto& objectState         = m_objectStates[renderData.objectId];
    auto& objectDescriptorSet = objectState.descriptorSets[imageIndex];

    // TODO: if needs update
    VkWriteDescriptorSet descriptor_writes[VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT];
    std::memset(
        &descriptor_writes, 0, sizeof(VkWriteDescriptorSet) * VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT
    );

    uint32_t descriptor_count = 0;
    uint32_t descriptor_index = 0;

    // Descriptor 0 - Uniform buffer
    uint32_t range = sizeof(gfx::ObjectUniformObject);
    uint64_t offset =
        sizeof(gfx::ObjectUniformObject) * renderData.objectId;  // also the index into the
    gfx::ObjectUniformObject obo;

    // TODO: get diffuse colour from a material.
    static float accumulator = 0.0f;
    // accumulator += renderData.deltaTime;
    // float s          = (std::sin(accumulator) + 1.0f) / 2.0f;  // scale from -1, 1 to 0, 1
    obo.diffuseColor = math::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Load the data into the buffer.
    m_objectUniformBuffer->loadData(offset, range, 0, &obo);

    // Only do this if the descriptor has not yet been updated.
    VkDescriptorBufferInfo buffer_info;

    if (objectState.descriptorStates[descriptor_index].generations[imageIndex] == core::invalidId) {
        buffer_info.buffer = m_objectUniformBuffer->getHandle();
        buffer_info.offset = offset;
        buffer_info.range  = range;

        VkWriteDescriptorSet descriptor = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        descriptor.dstSet               = objectDescriptorSet;
        descriptor.dstBinding           = descriptor_index;
        descriptor.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor.descriptorCount      = 1;
        descriptor.pBufferInfo          = &buffer_info;

        descriptor_writes[descriptor_count] = descriptor;
        descriptor_count++;

        // Update the frame generation. In this case it is only needed once since this is a

        objectState.descriptorStates[descriptor_index].generations[imageIndex] = 1;
    }
    descriptor_index++;

    // TODO: samplers.
    const uint32_t sampler_count = 1;
    VkDescriptorImageInfo image_infos[1];

    for (uint32_t sampler_index = 0; sampler_index < sampler_count; ++sampler_index) {
        Texture* t = static_cast<Texture*>(renderData.textures[sampler_index]);
        uint32_t* descriptor_generation =
            &objectState.descriptorStates[descriptor_index].generations[imageIndex];

        // Check if the descriptor needs updating first.
        if (t && (*descriptor_generation != t->generation ||
                  *descriptor_generation == core::invalidId)) {
            // Assign view and sampler.
            image_infos[sampler_index].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_infos[sampler_index].imageView   = t->getImage()->getView();
            image_infos[sampler_index].sampler     = t->getSampler();

            VkWriteDescriptorSet descriptor = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
            descriptor.dstSet               = objectDescriptorSet;
            descriptor.dstBinding           = descriptor_index;
            descriptor.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor.descriptorCount      = 1;
            descriptor.pImageInfo           = &image_infos[sampler_index];

            descriptor_writes[descriptor_count] = descriptor;
            descriptor_count++;

            // Sync frame generation if not using a default texture.
            if (t->generation != core::invalidId) {
                *descriptor_generation = t->generation;
            }
            descriptor_index++;
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

void ShaderObject::updateGlobalState(
    const gfx::GlobalState& globalState, VkCommandBuffer commandBuffer, uint32_t imageIndex,
    Pipeline& pipeline
) {
    auto globalDescriptor = m_globalDescriptorSets[imageIndex];

    if (not m_descriptorUpdated[imageIndex]) {
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
        m_descriptorUpdated[imageIndex] = true;
    }

    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1,
        &globalDescriptor, 0, 0
    );
}

void ShaderObject::createUniformBuffer() {
    Buffer::Properties props;

    props.size       = sizeof(gfx::GlobalUniformObject) * 3;
    props.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    props.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    props.bindOnCreate = true;

    m_globalUniformBuffer = core::createUniqPtr<Buffer>(m_context, m_device, props);

    props.size = sizeof(gfx::ObjectUniformObject);

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

VkDescriptorSetLayout ShaderObject::getGlobalDescriptorSetLayout() const {
    return m_globalDescriptorSetLayout;
}

const std::vector<ShaderStage>& ShaderObject::getStages() const { return m_stages; }

gfx::GlobalUniformObject& ShaderObject::getGlobalUBO() { return m_globalUBO; }

}  // namespace nova::platform::vulkan
