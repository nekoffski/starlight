#pragma once

#include <span>
#include <vector>

#include "starlight/core/Log.hh"
#include "starlight/core/Core.hh"

#include "Vulkan.hh"

#include "VKPhysicalDevice.hh"
#include "VKContext.hh"

#include "VKContext.hh"
#include "VKCommandBuffer.hh"
#include "VKRenderPass.hh"

#include "starlight/core/math/Vertex.hh"

#include "starlight/renderer/CullMode.hh"

namespace sl::vk {

// TODO: REFACTOR

class VKPipeline {
public:
    struct Properties {
        uint32_t stride;
        std::span<VkVertexInputAttributeDescription> vertexAttributes;
        std::span<VkDescriptorSetLayout> descriptorSetLayouts;
        std::span<VkPipelineShaderStageCreateInfo> stages;
        std::span<Range> pushConstantRanges;
        VkViewport viewport;
        VkRect2D scissor;
        VkPolygonMode polygonMode;
        bool depthTestEnabled;
        CullMode cullMode;
    };

    explicit VKPipeline(
      VKContext& context, VKLogicalDevice& device, VKRenderPass& renderPass,
      Properties props
    ) :
        m_context(context),
        m_device(device) {
        VkPipelineViewportStateCreateInfo viewportState = {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
        };

        viewportState.viewportCount = 1;
        viewportState.pViewports    = &props.viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &props.scissor;

        // rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
        };

        static const auto getCullMode = [](CullMode mode) {
            switch (mode) {
                case CullMode::none:
                    return VK_CULL_MODE_NONE;
                case CullMode::front:
                    return VK_CULL_MODE_FRONT_BIT;
                case CullMode::frontAndBack:
                    return VK_CULL_MODE_FRONT_AND_BACK;
                case CullMode::back:
                default:
                    return VK_CULL_MODE_BACK_BIT;
            }
        };

        rasterizerCreateInfo.depthClampEnable        = false;
        rasterizerCreateInfo.rasterizerDiscardEnable = false;
        rasterizerCreateInfo.polygonMode             = props.polygonMode;
        rasterizerCreateInfo.lineWidth               = 1.0f;
        rasterizerCreateInfo.cullMode                = getCullMode(props.cullMode);
        rasterizerCreateInfo.frontFace       = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerCreateInfo.depthBiasEnable = false;
        rasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
        rasterizerCreateInfo.depthBiasClamp          = 0.0f;
        rasterizerCreateInfo.depthBiasSlopeFactor    = 0.0f;

        // Multisampling.
        VkPipelineMultisampleStateCreateInfo multisampling_create_info = {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
        };
        multisampling_create_info.sampleShadingEnable   = VK_FALSE;
        multisampling_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
        multisampling_create_info.minSampleShading      = 1.0f;
        multisampling_create_info.pSampleMask           = 0;
        multisampling_create_info.alphaToCoverageEnable = VK_FALSE;
        multisampling_create_info.alphaToOneEnable      = VK_FALSE;

        // Depth and stencil testing.
        VkPipelineDepthStencilStateCreateInfo depth_stencil = {
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
        };

        if (props.depthTestEnabled) {
            depth_stencil.depthTestEnable       = VK_TRUE;
            depth_stencil.depthWriteEnable      = VK_TRUE;
            depth_stencil.depthCompareOp        = VK_COMPARE_OP_LESS;
            depth_stencil.depthBoundsTestEnable = VK_FALSE;
            depth_stencil.stencilTestEnable     = VK_FALSE;
        }

        VkPipelineColorBlendAttachmentState color_blend_attachment_state;
        std::memset(
          &color_blend_attachment_state, 0,
          sizeof(VkPipelineColorBlendAttachmentState)
        );
        color_blend_attachment_state.blendEnable         = VK_TRUE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_state.dstColorBlendFactor =
          VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_state.dstAlphaBlendFactor =
          VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

        color_blend_attachment_state.colorWriteMask =
          VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
          | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
        };
        color_blend_state_create_info.logicOpEnable   = VK_FALSE;
        color_blend_state_create_info.logicOp         = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments = &color_blend_attachment_state;

        // Dynamic state
        const uint32_t dynamic_state_count                 = 3;
        VkDynamicState dynamic_states[dynamic_state_count] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_LINE_WIDTH,
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
        };
        dynamic_state_create_info.dynamicStateCount = dynamic_state_count;
        dynamic_state_create_info.pDynamicStates    = dynamic_states;

        // Vertex input
        VkVertexInputBindingDescription binding_description;
        binding_description.binding = 0;  // Binding index
        binding_description.stride  = props.stride;
        binding_description.inputRate =
          VK_VERTEX_INPUT_RATE_VERTEX;  // Move to next data entry for each vertex.

        // Attributes
        VkPipelineVertexInputStateCreateInfo vertex_input_info = {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };
        vertex_input_info.vertexBindingDescriptionCount = 1;
        vertex_input_info.pVertexBindingDescriptions    = &binding_description;
        vertex_input_info.vertexAttributeDescriptionCount =
          props.vertexAttributes.size();
        vertex_input_info.pVertexAttributeDescriptions =
          props.vertexAttributes.data();

        // Input assembly
        VkPipelineInputAssemblyStateCreateInfo input_assembly = {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
        };
        input_assembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;

        // VKPipeline layout
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };

        std::vector<VkPushConstantRange> ranges;
        VkPushConstantRange push_constant;

        if (const auto pushConstantRangeCount = props.pushConstantRanges.size();
            pushConstantRangeCount > 0) {
            ASSERT(
              pushConstantRangeCount <= 32,
              "Push constant range count {} exceeds maximum 32",
              pushConstantRangeCount
            );

            ranges.reserve(pushConstantRangeCount);

            for (const auto& range : props.pushConstantRanges) {
                LOG_TRACE("Push constant range: {}-{}", range.offset, range.size);
                ranges.push_back(VkPushConstantRange{
                  .stageFlags =
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                  .offset = static_cast<u32>(range.offset),
                  .size   = static_cast<u32>(range.size) });
            }

            LOG_TRACE(
              "Total push constants for pipeline: {}", pushConstantRangeCount
            );

            pipeline_layout_create_info.pushConstantRangeCount =
              pushConstantRangeCount;
            pipeline_layout_create_info.pPushConstantRanges = ranges.data();

        } else {
            // TEMPORARY: todo remove
            push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            push_constant.offset     = sizeof(glm::mat4) * 0;
            push_constant.size       = sizeof(glm::mat4) * 2;
            pipeline_layout_create_info.pushConstantRangeCount = 1;
            pipeline_layout_create_info.pPushConstantRanges    = &push_constant;

            pipeline_layout_create_info.pPushConstantRanges    = &push_constant;
            pipeline_layout_create_info.pushConstantRangeCount = 1;

            // pipeline_layout_create_info.pushConstantRangeCount = 0;
            // pipeline_layout_create_info.pPushConstantRanges    = nullptr;
        }

        // Descriptor set layouts
        pipeline_layout_create_info.setLayoutCount =
          props.descriptorSetLayouts.size();
        pipeline_layout_create_info.pSetLayouts = props.descriptorSetLayouts.data();

        LOG_TRACE(
          "Descriptor set layouts in pipeline: {}",
          pipeline_layout_create_info.setLayoutCount
        );

        for (int i = 0; i < pipeline_layout_create_info.setLayoutCount; ++i) {
        }

        auto logicalDevice = m_device.getHandle();
        auto allocator     = m_context.getAllocator();

        // Create the pipeline layout.
        VK_ASSERT(vkCreatePipelineLayout(
          logicalDevice, &pipeline_layout_create_info, allocator, &m_layout
        ));

        // VKPipeline create
        VkGraphicsPipelineCreateInfo pipeline_create_info = {
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO
        };
        pipeline_create_info.stageCount          = props.stages.size();
        pipeline_create_info.pStages             = props.stages.data();
        pipeline_create_info.pVertexInputState   = &vertex_input_info;
        pipeline_create_info.pInputAssemblyState = &input_assembly;

        pipeline_create_info.pViewportState      = &viewportState;
        pipeline_create_info.pRasterizationState = &rasterizerCreateInfo;
        pipeline_create_info.pMultisampleState   = &multisampling_create_info;
        pipeline_create_info.pDepthStencilState =
          props.depthTestEnabled ? &depth_stencil : nullptr;
        pipeline_create_info.pColorBlendState   = &color_blend_state_create_info;
        pipeline_create_info.pDynamicState      = &dynamic_state_create_info;
        pipeline_create_info.pTessellationState = 0;

        pipeline_create_info.layout = m_layout;

        pipeline_create_info.renderPass         = renderPass.getHandle();
        pipeline_create_info.subpass            = 0;
        pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_create_info.basePipelineIndex  = -1;

        VK_ASSERT(vkCreateGraphicsPipelines(
          logicalDevice, VK_NULL_HANDLE, 1, &pipeline_create_info, allocator,
          &m_handle
        ));
    }

    ~VKPipeline() {
        auto logicalDeviceHandle = m_device.getHandle();
        auto allocator           = m_context.getAllocator();

        if (m_handle) vkDestroyPipeline(logicalDeviceHandle, m_handle, allocator);

        if (m_layout)
            vkDestroyPipelineLayout(logicalDeviceHandle, m_layout, allocator);
    }

    void bind(VKCommandBuffer& commandBuffer, VkPipelineBindPoint bindPoint) {
        vkCmdBindPipeline(commandBuffer.getHandle(), bindPoint, m_handle);
    }

    VkPipelineLayout getLayout() const { return m_layout; }

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;

    VkPipeline m_handle = VK_NULL_HANDLE;
    VkPipelineLayout m_layout;
};

}  // namespace sl::vk
