#pragma once

#include <string>

#include "starlight/core/memory/Memory.hpp"

#include "Vulkan.h"
#include "RenderPass.h"
#include "Buffer.h"
#include "Device.h"

#include "starlight/renderer/Shader.h"

namespace sl::vk {

class Shader final : public sl::Shader::Impl {
    struct StageConfig {
        VkShaderStageFlagBits stage;
        std::string source;
    };

    struct DescriptorSetConfig {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
    };

    struct Config {
        std::vector<StageConfig> stages;
        std::array<VkDescriptorPoolSize, 2> poolSizes;
        u16 maxDescriptorSetCount;
        u8 descriptorSetCount;
        std::array<DescriptorSetConfig, 2> descriptorSets;
    };

    struct DescriptorState {
        // 1 per frame
        std::array<u8, 3> generations;
        std::array<u32, 3> ids;
    };

    struct DescriptorSetState {
        std::array<VkDescriptorSet, 3> descriptorSets;
        std::vector<DescriptorState> descriptorStates;
    };

    struct InstanceState {
        Id32 id;
        u64 offset;
        DescriptorSetState descriptorSetState;
        std::vector<Texture*> instanceTextures;
    };

public:
    explicit Shader(sl::Shader& self, Device* device, const Context* context);

    void initialize() override;

private:
    void createModule(const StageConfig& cfg);

    sl::Shader& m_self;
    Device* m_device;
    const Context* m_context;

    void* m_mappedUniformBufferBlock;
    Id32 m_id;
    Config m_config;
    RenderPass* m_renderPass;
    VkDescriptorPool m_descriptorPool;
    std::array<VkDescriptorSetLayout, 2> m_descriptorSetLayouts;
    std::array<VkDescriptorSet, 3> m_globalDescriptorSets;
    LocalPtr<Buffer> m_uniformBuffer;

    std::vector<InstanceState> m_instanceStates;
};

}  // namespace sl::vk