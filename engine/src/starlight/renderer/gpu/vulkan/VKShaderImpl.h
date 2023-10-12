#pragma once

#include <string>

#include "starlight/core/memory/Memory.hpp"

#include "Vulkan.h"
#include "VKRenderPass.h"
#include "VKBuffer.h"
#include "VKDevice.h"
#include "VKShaderStage.h"
#include "VKPipeline.h"
#include "VKRendererContext.h"

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class VKShaderImpl final : public Shader::Impl {
    static constexpr u32 maxBindings = 32;

    struct StageConfig {
        ShaderStage::Type type;
        std::string source;
    };

    struct DescriptorSetConfig {
        u8 bindingCount = 0;
        std::array<VkDescriptorSetLayoutBinding, 32> bindings;
    };

    struct Config {
        std::vector<StageConfig> stages;
        std::array<VkDescriptorPoolSize, 2> poolSizes;
        u16 maxDescriptorSetCount = 0;
        u8 descriptorSetCount     = 0;
        std::array<DescriptorSetConfig, 2> descriptorSets;
        std::vector<VkVertexInputAttributeDescription> attributes;
    };

    struct VKDescriptorState {
        // 1 per frame
        std::array<Id8, 3> generations;
        std::array<Id32, 3> ids;
    };

    struct DescriptorSetState {
        std::array<VkDescriptorSet, 3> descriptorSets;
        std::vector<VKDescriptorState> descriptorStates;
    };

    struct InstanceState {
        Id32 id;
        Id64 offset;
        DescriptorSetState descriptorSetState;
        std::vector<Texture*> instanceTextures;
    };

public:
    explicit VKShaderImpl(
      sl::Shader& self, VKDevice* device, const VKContext* context,
      VKRenderPass* renderPass, VKRendererContext& rendererContext
    );
    ~VKShaderImpl() override;

    void initialize() override;
    void use() override;
    void bindGlobals() override;
    void bindInstance(u32 instanceId) override;
    void applyGlobals() override;
    void applyInstance() override;
    u32 acquireInstanceResources() override;
    void releaseInstanceResources(u32 instanceId) override;
    void setUniform(const ShaderUniform& uniform, void* value) override;

private:
    void createModules();
    void processAttributes();
    void processUniforms();
    void createDescriptorPool();
    void createDescriptorSetLayouts();
    void createPipeline();
    void createUniformBuffer();

    sl::Shader& m_self;
    VKDevice* m_device;
    const VKContext* m_context;
    VKRenderPass* m_renderPass;
    VKRendererContext& m_rendererContext;

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