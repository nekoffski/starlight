#pragma once

#include <array>
#include <string>

#include "fwd.h"

#include "nova/core/Memory.hpp"
#include "nova/gfx/RendererBacked.h"

#include "Vulkan.h"

namespace nova::platform::vulkan {

class ShaderObject {
    static constexpr uint8_t s_stagesCount = 2;  // vertex + fragment
   public:
    explicit ShaderObject(const Context* context, const Device* device, int swapchainImageCount);

    ~ShaderObject();

    void updateObject(
        Pipeline& pipeline, VkCommandBuffer commandBuffer, const glm::mat4& modelMatrix
    );

    void updateGlobalState(
        const gfx::GlobalState& globalState, VkCommandBuffer commandBuffer, uint32_t imageIndex,
        Pipeline& pipeline
    );

    void createUniformBuffer();

    VkDescriptorSetLayout getGlobalDescriptorSetLayout() const;

    const std::vector<ShaderStage>& getStages() const;

    gfx::GlobalUniformObject& getGlobalUBO();

   private:
    const Context* m_context;
    const Device* m_device;

    std::vector<ShaderStage> m_stages;

    gfx::GlobalUniformObject m_globalUBO;

    VkDescriptorPool m_globalDescriptorPool;
    VkDescriptorSetLayout m_globalDescriptorSetLayout;
    VkDescriptorSet m_globalDescriptorSets[3];  // max 3 for triple-buffering

    core::UniqPtr<Buffer> m_globalUniformBuffer;
};

}  // namespace nova::platform::vulkan
