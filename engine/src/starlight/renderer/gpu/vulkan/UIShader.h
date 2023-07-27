#pragma once

#include <vector>

#include "starlight/core/math/Glm.h"
#include "starlight/core/math/Size.hpp"
#include "starlight/core/math/Vertex2.h"
#include "starlight/core/Memory.hpp"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Material.h"

#include "Vulkan.h"
#include "DescriptorState.h"
#include "ShaderStage.h"
#include "Buffer.h"

namespace sl::vk {

class UIShader {
   public:
    static constexpr uint32_t stagesCount     = 2;
    static constexpr uint32_t descriptorCount = 2;
    static constexpr uint32_t samplerCount    = 1;
    static constexpr uint32_t maxUICount      = 1024;

    struct InstanceState {
        // one per frame
        VkDescriptorSet descriptorSets[3];
        DescriptorState descriptorStates[descriptorCount];
    };

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

    explicit UIShader(
        const Context* context, Device* device, int swapchainImageCount,
        const Size2u32& framebuffferSize, RenderPass& renderPass
    );

    ~UIShader();

    void setModel(VkCommandBuffer commandBuffer, const Mat4f& model);

    void use(CommandBuffer& buffer);

    void applyMaterial(
        VkCommandBuffer commandBuffer, uint32_t imageIndex, const Material& material
    );

    void updateGlobalWorldState(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void acquireResources(Material& material);
    void releaseResources(Material& material);

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;
    VkDescriptorSetLayout getObjectDescriptorSetLayout() const;

    const std::vector<ShaderStage>& getStages() const;

    GlobalUBO& getGlobalUBO();

   private:
    void createPipeline(const Size2u32& framebuffferSize, RenderPass& renderPass);
    void createShaderStages();
    void createGlobalDescriptorSetLayout();
    void createLocalDescriptorSetLayout();
    void createGlobalDescriptorPool(int swapchainImageCount);
    void createLocalDescriptorPool();

    const Context* m_context;
    Device* m_device;

    std::vector<ShaderStage> m_stages;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSets[3];

    GlobalUBO m_globalUBO;
    UniqPtr<Buffer> m_globalUniformBuffer;

    VkDescriptorPool m_objectDescriptorPool;
    VkDescriptorSetLayout m_objectDescriptorSetLayout;

    UniqPtr<Pipeline> m_pipeline;
    UniqPtr<Buffer> m_objectUniformBuffer;
    uint32_t m_objectUniformBufferIndex;

    std::array<sl::Texture::Use, samplerCount> m_samplerUses;

    InstanceState m_instanceStates[maxUICount];
};

}  // namespace sl::vk
