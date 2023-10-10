#pragma once

#include <array>
#include <string>

#include "fwd.h"

#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/math/Size.hpp"

#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Material.h"

#include "VKPipeline.h"
#include "VKBuffer.h"
#include "VKDevice.h"
#include "Vulkan.h"
#include "VKDescriptorState.h"

namespace sl::vk {

class VKMaterialShader {
public:
    static constexpr uint32_t stagesCount      = 2;  // vertex + fragment
    static constexpr uint32_t descriptorCount  = 2;
    static constexpr uint32_t samplerCount     = 1;
    static constexpr uint32_t maxMaterialCount = 1024;

    struct InstanceUBO {
        Vec4f diffuseColor;  // 16 bytes
        Vec4f reserved0;     // 16 bytes, reserved for future use
        Vec4f reserved1;     // 16 bytes, reserved for future use
        Vec4f reserved2;     // 16 bytes, reserved for future use
    };

    struct GlobalUBO {
        Mat4f projection;
        Mat4f view;
        // pad to 256
        Mat4f p0, p1;
    };

    struct InstanceState {
        VkDescriptorSet descriptorSets[3];
        VKDescriptorState descriptorStates[descriptorCount];
    };

    explicit VKMaterialShader(
      const VKContext* context, VKDevice* device, int swapchainImageCount,
      const Size2u32& framebuffferSize, VKRenderPass& renderPass
    );

    ~VKMaterialShader();

    void use(VKCommandBuffer& buffer);

    void setModel(VkCommandBuffer commandBuffer, const Mat4f& model);

    void applyMaterial(
      VkCommandBuffer commandBuffer, uint32_t imageIndex, const Material& material
    );

    void updateGlobalWorldState(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void acquireResources(Material& material);
    void releaseResources(Material& material);

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;
    VkDescriptorSetLayout getObjectDescriptorSetLayout() const;

    const std::vector<VKShaderStage>& getStages() const;

    GlobalUBO& getGlobalUBO();

private:
    void createShaderStages();
    void createGlobalDescriptorSetLayout();
    void createLocalDescriptorSetLayout();
    void createGlobalDescriptorPool(int swapchainImageCount);
    void createLocalDescriptorPool();

    void createPipeline(const Size2u32& framebuffferSize, VKRenderPass& renderPass);

    const VKContext* m_context;
    VKDevice* m_device;

    std::vector<VKShaderStage> m_stages;

    GlobalUBO m_globalUBO;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSets[3];  // max 3 for triple-buffering

    UniqPtr<VKBuffer> m_globalUniformBuffer;

    VkDescriptorPool m_objectDescriptorPool;
    VkDescriptorSetLayout m_objectDescriptorSetLayout;

    UniqPtr<VKPipeline> m_pipeline;
    UniqPtr<VKBuffer> m_objectUniformBuffer;
    uint32_t m_objectUniformBufferIndex;

    std::array<Texture::Use, samplerCount> m_samplerUses;

    InstanceState m_instanceStates[maxMaterialCount];
};

}  // namespace sl::vk
