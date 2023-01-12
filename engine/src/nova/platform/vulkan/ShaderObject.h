#pragma once

#include <array>
#include <string>

#include "fwd.h"

#include "nova/core/Memory.hpp"
#include "nova/gfx/RendererBacked.h"

#include "Device.h"
#include "Vulkan.h"

namespace nova::platform::vulkan {

struct DescriptorState {
    uint32_t generations[3];  // generation per frame
};

#define VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT 2

struct ObjectSharedObjectState {
    VkDescriptorSet descriptorSets[3];
    DescriptorState descriptorStates[VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT];
};

#define VULKAN_OBJECT_MAX_OBJECT_COUNT 1024

class ShaderObject {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit ShaderObject(const Context* context, Device* device, int swapchainImageCount);

    ~ShaderObject();

    void updateObject(
        Pipeline& pipeline, VkCommandBuffer commandBuffer, const gfx::GeometryRenderData&,
        uint32_t imageIndex
    );

    void updateGlobalState(
        const gfx::GlobalState& globalState, VkCommandBuffer commandBuffer, uint32_t imageIndex,
        Pipeline& pipeline
    );

    uint32_t acquireResources() {  // TODO: free list
        core::Id objectId = m_objectUniformBufferIndex;
        m_objectUniformBufferIndex++;

        auto& objectState = m_objectStates[objectId];

        for (uint32_t i = 0; i < VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT; ++i)
            for (uint32_t j = 0; j < 3; ++j)
                objectState.descriptorStates[i].generations[j] = core::invalidId;

        // Allocate descriptor sets.
        VkDescriptorSetLayout layouts[3] = {
            m_objectDescriptorSetLayout, m_objectDescriptorSetLayout, m_objectDescriptorSetLayout};

        VkDescriptorSetAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        alloc_info.descriptorPool              = m_objectDescriptorPool;
        alloc_info.descriptorSetCount          = 3;  // one per frame
        alloc_info.pSetLayouts                 = layouts;

        VK_ASSERT(vkAllocateDescriptorSets(
            m_device->getLogicalDevice(), &alloc_info, objectState.descriptorSets
        ));

        return objectId;
    }

    void releaseResources(uint32_t objectId) {
        auto& objectState = m_objectStates[objectId];

        const uint32_t descriptor_set_count = 3;
        // Release object descriptor sets.
        VkResult result = vkFreeDescriptorSets(
            m_device->getLogicalDevice(), m_objectDescriptorPool, descriptor_set_count,
            objectState.descriptorSets
        );

        if (result != VK_SUCCESS) {
            // KERROR("Error freeing object shader descriptor sets!");
        }

        for (uint32_t i = 0; i < VULKAN_OBJECT_SHADER_DESCRIPTOR_COUNT; ++i)
            for (uint32_t j = 0; j < 3; ++j)
                objectState.descriptorStates[i].generations[j] = core::invalidId;
    }

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;
    VkDescriptorSetLayout getObjectDescriptorSetLayout() const {
        return m_objectDescriptorSetLayout;
    }

    const std::vector<ShaderStage>& getStages() const;

    gfx::GlobalUniformObject& getGlobalUBO();

   private:
    const Context* m_context;
    Device* m_device;

    std::vector<ShaderStage> m_stages;

    gfx::GlobalUniformObject m_globalUBO;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSets[3];  // max 3 for triple-buffering

    core::UniqPtr<Buffer> m_globalUniformBuffer;

    VkDescriptorPool m_objectDescriptorPool;
    VkDescriptorSetLayout m_objectDescriptorSetLayout;

    core::UniqPtr<Buffer> m_objectUniformBuffer;
    uint32_t m_objectUniformBufferIndex;

    ObjectSharedObjectState m_objectStates[VULKAN_OBJECT_MAX_OBJECT_COUNT];
};

}  // namespace nova::platform::vulkan
