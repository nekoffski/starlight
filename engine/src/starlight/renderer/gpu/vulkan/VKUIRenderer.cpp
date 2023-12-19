#include "VKUIRenderer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

#include "starlight/core/utils/Log.h"

#include "Vulkan.h"
#include "VKDevice.h"
#include "VKContext.h"
#include "VKRenderPass.h"

namespace sl::vk {

VKUIRenderer::VKUIRenderer(
  VKContext& context, VKDevice& device, RendererBackendProxy& backendProxy,
  Window& window, RenderPass* renderPass
) :
    m_context(context),
    m_device(device) {
    ASSERT(not s_hasInstance, "Only single instance of UI renderer is allowed");
    s_hasInstance = true;

    VkDescriptorPoolSize poolSizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER,                 1000},
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
    };
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets       = 1000;
    poolInfo.poolSizeCount = std::size(poolSizes);
    poolInfo.pPoolSizes    = poolSizes;

    VK_ASSERT(vkCreateDescriptorPool(
      m_device.getLogicalDevice(), &poolInfo, m_context.getAllocator(), &m_uiPool
    ));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(window.getHandle()), true);

    auto graphicsQueue = m_device.getQueues().graphics;

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance                  = m_context.getInstance();
    initInfo.PhysicalDevice            = m_device.getGPU();
    initInfo.Device                    = m_device.getLogicalDevice();
    initInfo.Queue                     = graphicsQueue;
    initInfo.DescriptorPool            = m_uiPool;
    initInfo.MinImageCount             = 3;
    initInfo.ImageCount                = 3;
    initInfo.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;

    auto renderPassHandle = static_cast<VKRenderPass*>(renderPass)->getHandle();
    ImGui_ImplVulkan_Init(&initInfo, renderPassHandle);

    backendProxy.gpuCall([&](CommandBuffer& buffer) {
        auto bufferHandle = static_cast<VKCommandBuffer*>(&buffer)->getHandle();
        ImGui_ImplVulkan_CreateFontsTexture(bufferHandle);
    });

    ImGui_ImplVulkan_DestroyFontUploadObjects();
    LOG_TRACE("UI backend initialized successfully");
}

VKUIRenderer::~VKUIRenderer() {
    m_device.waitIdle();

    if (m_uiPool) {
        vkDestroyDescriptorPool(
          m_device.getLogicalDevice(), m_uiPool, m_context.getAllocator()
        );
    }

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void VKUIRenderer::begin([[maybe_unused]] CommandBuffer& commandBuffer) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void VKUIRenderer::end(CommandBuffer& commandBuffer) {
    ImGui::Render();
    auto bufferHandle =
      static_cast<vk::VKCommandBuffer*>(&commandBuffer)->getHandle();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), bufferHandle);
}

}  // namespace sl::vk
