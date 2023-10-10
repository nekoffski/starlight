#pragma once

#include <string>

#include "starlight/core/memory/Memory.hpp"

#include "Vulkan.h"
#include "VKRenderPass.h"
#include "VKBuffer.h"
#include "VKDevice.h"
#include "VKShaderStage.h"
#include "VKPipeline.h"

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class VKShaderImpl final : public Shader::Impl {
    struct StageConfig {
        ShaderStage::Type type;
        std::string source;
    };

    struct DescriptorSetConfig {
        u8 bindingCount;
        std::array<VkDescriptorSetLayoutBinding, 32> bindings;
    };

    struct Config {
        std::vector<StageConfig> stages;
        std::array<VkDescriptorPoolSize, 2> poolSizes;
        u16 maxDescriptorSetCount;
        u8 descriptorSetCount;
        std::array<DescriptorSetConfig, 2> descriptorSets;
        std::vector<VkVertexInputAttributeDescription> attributes;
    };

    struct VKDescriptorState {
        // 1 per frame
        std::array<u8, 3> generations;
        std::array<u32, 3> ids;
    };

    struct DescriptorSetState {
        std::array<VkDescriptorSet, 3> descriptorSets;
        std::vector<VKDescriptorState> descriptorStates;
    };

    struct InstanceState {
        Id32 id;
        u64 offset;
        DescriptorSetState descriptorSetState;
        std::vector<Texture*> instanceTextures;
    };

public:
    explicit VKShaderImpl(
      sl::Shader& self, VKDevice* device, const VKContext* context,
      VKRenderPass* renderPass
    );

    void initialize() override;

private:
    void createModules();
    void processAttributes();
    void processUniforms();
    void createDescriptorPool();
    void createDescriptorSetLayouts();

    sl::Shader& m_self;
    VKDevice* m_device;
    const VKContext* m_context;
    VKRenderPass* m_renderPass;

    void* m_mappedUniformBufferBlock;
    Id32 m_id;
    Config m_config;
    VkDescriptorPool m_descriptorPool;
    std::array<VkDescriptorSetLayout, 2> m_descriptorSetLayouts;
    std::array<VkDescriptorSet, 3> m_globalDescriptorSets;

    LocalPtr<VKPipeline> m_pipeline;
    LocalPtr<VKBuffer> m_uniformBuffer;

    std::vector<InstanceState> m_instanceStates;
    std::vector<VKShaderStage> m_stages;
};

}  // namespace sl::vk