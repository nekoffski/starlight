#pragma once

#include <array>
#include <string>

#include "fwd.h"

#include "starlight/core/Memory.hpp"

#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/gpu/RendererBackend.h"

#include "Device.h"
#include "Vulkan.h"

namespace sl::vk {

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

    void setModel(Pipeline& pipeline, VkCommandBuffer commandBuffer, const Mat4f& model);

    void applyMaterial(
        Pipeline& pipeline, VkCommandBuffer commandBuffer, uint32_t imageIndex,
        const Material& material
    );

    void updateGlobalState(VkCommandBuffer commandBuffer, uint32_t imageIndex, Pipeline& pipeline);

    void acquireResources(Material& material);
    void releaseResources(Material& material);

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;
    VkDescriptorSetLayout getObjectDescriptorSetLayout() const;

    const std::vector<ShaderStage>& getStages() const;

    GlobalUniformObject& getGlobalUBO();

   private:
    void createShaderStages();
    void createGlobalDescriptorSetLayout();
    void createLocalDescriptorSetLayout();
    void createGlobalDescriptorPool(int swapchainImageCount);
    void createLocalDescriptorPool();

    const Context* m_context;
    Device* m_device;

    std::vector<ShaderStage> m_stages;

    GlobalUniformObject m_globalUBO;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSets[3];  // max 3 for triple-buffering

    UniqPtr<Buffer> m_globalUniformBuffer;

    VkDescriptorPool m_objectDescriptorPool;
    VkDescriptorSetLayout m_objectDescriptorSetLayout;

    UniqPtr<Buffer> m_objectUniformBuffer;
    uint32_t m_objectUniformBufferIndex;

    std::array<sl::Texture::Use, VULKAN_MATERIAL_SHADER_SAMPLER_COUNT> m_samplerUses;

    MaterialShaderObjectState m_instanceStates[VULKAN_MAX_MATERIAL_COUNT];
};

}  // namespace sl::vk
