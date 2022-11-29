#include "ShaderObject.h"

#include <glm/gtc/type_ptr.hpp>

#include "ShaderStage.h"
#include "Pipeline.h"
#include "Buffer.h"

namespace nova::platform::vulkan {

ShaderObject::ShaderObject(const Context* context, const Device* device, int swapchainImageCount)
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
}

ShaderObject::~ShaderObject() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    if (m_globalDescriptorPool)
        vkDestroyDescriptorPool(logicalDevice, m_globalDescriptorPool, allocator);

    if (m_globalDescriptorSetLayout)
        vkDestroyDescriptorSetLayout(logicalDevice, m_globalDescriptorSetLayout, allocator);
}

void ShaderObject::updateObject(
    Pipeline& pipeline, VkCommandBuffer commandBuffer, const glm::mat4& modelMatrix
) {
    vkCmdPushConstants(
        commandBuffer, pipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(modelMatrix),
        glm::value_ptr(modelMatrix)
    );
}

void ShaderObject::updateGlobalState(
    const gfx::GlobalState& globalState, VkCommandBuffer commandBuffer, uint32_t imageIndex,
    Pipeline& pipeline
) {
    auto globalDescriptor = m_globalDescriptorSets[imageIndex];

    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1,
        &globalDescriptor, 0, 0
    );

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
}

void ShaderObject::createUniformBuffer() {
    Buffer::Properties props;

    props.size       = sizeof(gfx::GlobalUniformObject);
    props.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    props.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    props.bindOnCreate = true;

    m_globalUniformBuffer = core::createUniqPtr<Buffer>(m_context, m_device, props);

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
