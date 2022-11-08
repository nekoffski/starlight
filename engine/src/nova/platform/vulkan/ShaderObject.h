#pragma once

#include <array>
#include <string>

#include "nova/gfx/RendererBacked.h"

#include "Vulkan.h"
#include "ShaderStage.h"
#include "Pipeline.h"
#include "Buffer.h"

namespace nova::platform::vulkan {

class ShaderObject {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit ShaderObject(const Context* context, const Device* device, int swapchainImageCount) {
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

        VkDescriptorPoolCreateInfo global_pool_info = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        global_pool_info.poolSizeCount = 1;
        global_pool_info.pPoolSizes    = &global_pool_size;
        global_pool_info.maxSets       = swapchainImageCount;

        VK_ASSERT(vkCreateDescriptorPool(
            logicalDevice, &global_pool_info, allocator, &m_globalDescriptorPool
        ));
    }

    const std::vector<ShaderStage>& getStages() const { return m_stages; }

   private:
    std::vector<ShaderStage> m_stages;

    gfx::GlobalUniformObject m_globalUBO;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSet[3];  // max 3 for triple-buffering

    core::UniqPtr<Buffer> m_globalUniformBuffer;
};

}  // namespace nova::platform::vulkan
