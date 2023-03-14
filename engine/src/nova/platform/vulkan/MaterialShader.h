#pragma once

#include <array>
#include <string>

#include "fwd.h"

#include "nova/core/Memory.hpp"
#include "nova/gfx/Texture.h"
#include "nova/gfx/RendererBackend.h"
#include "nova/gfx/Material.h"

#include "Device.h"
#include "Vulkan.h"

namespace nova::platform::vulkan {

struct DescriptorState {
    uint32_t generations[3];  // generation per frame
    uint32_t ids[3];          // generation per frame
};

#define VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT 2
#define VULKAN_MATERIAL_SHADER_SAMPLER_COUNT 1

struct MaterialShaderObjectState {
    VkDescriptorSet descriptorSets[3];
    DescriptorState descriptorStates[VULKAN_MATERIAL_SHADER_DESCRIPTOR_COUNT];
};

#define VULKAN_MAX_MATERIAL_COUNT 1024

class MaterialShader {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit MaterialShader(const Context* context, Device* device, int swapchainImageCount);

    ~MaterialShader();

    void updateObject(
        Pipeline& pipeline, VkCommandBuffer commandBuffer, const gfx::GeometryRenderData&,
        uint32_t imageIndex
    );

    void updateGlobalState(VkCommandBuffer commandBuffer, uint32_t imageIndex, Pipeline& pipeline);

    void acquireResources(gfx::Material& material);
    void releaseResources(gfx::Material& material);

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;
    VkDescriptorSetLayout getObjectDescriptorSetLayout() const;

    const std::vector<ShaderStage>& getStages() const;

    gfx::GlobalUniformObject& getGlobalUBO();

   private:
    void createShaderStages();
    void createGlobalDescriptorSetLayout();
    void createLocalDescriptorSetLayout();
    void createGlobalDescriptorPool(int swapchainImageCount);
    void createLocalDescriptorPool();

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

    std::array<gfx::Texture::Use, VULKAN_MATERIAL_SHADER_SAMPLER_COUNT> m_samplerUses;

    MaterialShaderObjectState m_instanceStates[VULKAN_MAX_MATERIAL_COUNT];
};

}  // namespace nova::platform::vulkan
