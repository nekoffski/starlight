#pragma once

#include <string>

#include "starlight/core/memory/Memory.hpp"

#include "Vulkan.h"
#include "VKRenderPass.h"
#include "VKBuffer.h"
#include "VKDevice.h"
#include "VKPipeline.h"
#include "VKRendererBackendProxy.h"

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class VKShaderStage {
public:
    struct Properties {
        const std::string& source;
        Shader::Stage::Type type;
    };

    explicit VKShaderStage(
      VKDevice* device, const VKContext* context, const Properties& properties
    );

    VkPipelineShaderStageCreateInfo getStageCreateInfo() const {
        return m_stageCreateInfo;
    }

    ~VKShaderStage();

private:
    VKDevice* m_device;
    const VKContext* m_context;

    VkShaderModuleCreateInfo m_moduleCreateInfo;
    VkPipelineShaderStageCreateInfo m_stageCreateInfo;
    VkShaderModule m_handle;
};

class VKShader final : public Shader {
    static constexpr u32 maxBindings = 2;

    struct DescriptorSetConfig {
        u8 bindingCount = 0;
        std::array<VkDescriptorSetLayoutBinding, 32> bindings;
    };

    struct DescriptorState {
        // 1 per frame
        std::array<Id8, 3> generations;
        std::array<Id32, 3> ids;
    };

    struct DescriptorSetState {
        std::array<VkDescriptorSet, 3> descriptorSets;
        std::vector<DescriptorState> descriptorStates;
    };

    struct InstanceState {
        Id32 id;
        Id64 offset;
        DescriptorSetState descriptorSetState;
        std::vector<const TextureMap*> instanceTextureMaps;
    };

public:
    explicit VKShader(
      u32 id, VKDevice* device, const VKContext* context, VKRenderPass* renderPass,
      VKRendererBackendProxy& backendProxy, const Shader::Properties& props
    );
    ~VKShader() override;

    // clang-format off
    void use() override;
    u32 acquireInstanceResources(
        const std::vector<TextureMap*>& textureMaps
    ) override;
    void releaseInstanceResources(u32 instanceId) override;
    // clang-format on

private:
    void setUniform(const std::string& uniform, const void* value) override;
    void bindInstance(u32 instanceId) override;

    void bindGlobals() override;
    void applyGlobals() override;
    void applyInstance() override;

    void createModules(std::span<const Stage> stages);
    void processAttributes();
    void processUniforms();
    void createDescriptorPool();
    void createDescriptorSetLayouts();
    void createPipeline();
    void createUniformBuffer();

    void addAttributes(std::span<const Shader::Attribute> attributes);
    void addUniforms(
      std::span<const Shader::Uniform::Properties> uniforms, Texture* defaultTexture
    );
    void addUniform(
      const std::string& name, u32 size, Uniform::Type type, Scope scope,
      u32 setLocation, bool isSampler
    );
    void addSampler(
      const Shader::Uniform::Properties& props, Texture* defaultTexture
    );

    VKDevice* m_device;
    const VKContext* m_context;
    VKRenderPass* m_renderPass;
    VKRendererBackendProxy& m_backendProxy;

    void* m_mappedUniformBufferBlock;
    Config m_config;
    VkDescriptorPool m_descriptorPool;
    std::array<VkDescriptorSetLayout, 2> m_descriptorSetLayouts;
    std::array<VkDescriptorSet, 3> m_globalDescriptorSets;

    LocalPtr<VKPipeline> m_pipeline;
    LocalPtr<VKBuffer> m_uniformBuffer;

    std::vector<InstanceState> m_instanceStates;
    std::vector<VKShaderStage> m_stages;

    u64 m_requiredUboAlignment;

    u64 m_globalUboSize;
    u64 m_globalUboStride;
    u64 m_globalUboOffset;

    u64 m_uboSize;
    u64 m_uboStride;

    u64 m_pushConstantSize;
    u64 m_pushConstantStride;

    std::vector<const TextureMap*> m_globalTextureMaps;
    u8 m_instanceTextureCount;

    u32 m_boundInstanceId;
    u32 m_boundUboOffset;

    std::unordered_map<std::string, Shader::Uniform> m_uniforms;
    std::vector<Shader::Attribute> m_attributes;

    u8 m_pushConstantRangeCount;
    std::array<Range, 32> m_pushConstantRanges;

    u16 m_attributeStride;

    std::array<VkDescriptorPoolSize, 2> m_poolSizes;
    u16 m_maxDescriptorSetCount;
    u8 m_descriptorSetCount;
    std::array<DescriptorSetConfig, 2> m_descriptorSets;
    std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
};

}  // namespace sl::vk