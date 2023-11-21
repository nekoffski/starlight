#include "VKRendererBackend.h"

#include "VKBuffer.h"
#include "VKCommandBuffer.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKFence.h"
#include "VKFramebuffer.h"
#include "VKImage.h"
#include "VKRenderPass.h"
#include "VKSwapchain.h"
#include "VKShader.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

namespace sl::vk {

VKRendererBackend::VKRendererBackend(Window& window, const Config& config) :
    m_rendererContext(maxFramesInFlight, window.getSize()), m_renderedVertices(0u) {
    createCoreComponents(window, config);
    regenerateFramebuffers();
    createCommandBuffers();
    createSemaphoresAndFences();
    createBuffers();

    m_resourcePools.emplace(
      *m_context, *m_device, *m_objectVertexBuffer, *m_objectIndexBuffer,
      m_rendererContext, this
    );

    initUI(window);
}

VKRendererBackend::~VKRendererBackend() {
    m_device->waitIdle();

    if (m_uiPool) {
        vkDestroyDescriptorPool(
          m_device->getLogicalDevice(), m_uiPool, m_context->getAllocator()
        );
    }

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void VKRendererBackend::initUI(Window& window) {
    // GUI: temporary
    LOG_TRACE("Initializing UI backend");
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
      m_device->getLogicalDevice(), &poolInfo, m_context->getAllocator(), &m_uiPool
    ));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;                 // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(window.getHandle()), true);

    auto graphicsQueue = m_device->getQueues().graphics;

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance                  = m_context->getInstance();
    init_info.PhysicalDevice            = m_device->getGPU();
    init_info.Device                    = m_device->getLogicalDevice();
    init_info.Queue                     = graphicsQueue;
    init_info.DescriptorPool            = m_uiPool;
    init_info.MinImageCount             = 3;
    init_info.ImageCount                = 3;
    init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, m_uiRenderPass->getHandle());

    executeNow(graphicsQueue, [&](VKCommandBuffer& buffer) {
        ImGui_ImplVulkan_CreateFontsTexture(buffer.getHandle());
    });

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    LOG_TRACE("UI backend initialized successfully");
}

void VKRendererBackend::executeNow(
  VkQueue queue, std::function<void(VKCommandBuffer& buffer)>&& callback
) {
    vkQueueWaitIdle(queue);
    VKCommandBuffer commandBuffer(
      m_device.get(), m_device->getGraphicsCommandPool(),
      VKCommandBuffer::Severity::primary
    );
    commandBuffer.createAndBeginSingleUse();
    callback(commandBuffer);
    commandBuffer.endSingleUse(queue);
    m_device->waitIdle();
}

void VKRendererBackend::renderUI(std::function<void()>&& callback) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    callback();
    ImGui::Render();
}

void VKRendererBackend::freeDataRange(
  VKBuffer& buffer, uint64_t offset, uint64_t size
) {
    buffer.free(size, offset);
}

void VKRendererBackend::drawGeometry(const GeometryRenderData& geometryRenderData) {
    const auto& dataDescription = geometryRenderData.geometry->getDataDescription();
    auto& commandBuffer         = *m_rendererContext.getCommandBuffer();

    VkDeviceSize offsets[1] = { dataDescription.vertexBufferOffset };

    vkCmdBindVertexBuffers(
      commandBuffer.getHandle(), 0, 1, m_objectVertexBuffer->getHandlePointer(),
      (VkDeviceSize*)offsets
    );
    if (dataDescription.indexCount > 0) {
        vkCmdBindIndexBuffer(
          commandBuffer.getHandle(), m_objectIndexBuffer->getHandle(),
          dataDescription.indexBufferOffset, VK_INDEX_TYPE_UINT32
        );
        vkCmdDrawIndexed(
          commandBuffer.getHandle(), dataDescription.indexCount, 1, 0, 0, 0
        );

        m_renderedVertices += dataDescription.indexCount;
    } else {
        vkCmdDraw(commandBuffer.getHandle(), dataDescription.vertexCount, 1, 0, 0);
        m_renderedVertices += dataDescription.vertexCount;
    }
}

bool VKRendererBackend::beginRenderPass(uint8_t id) {
    auto& commandBuffer = *m_rendererContext.getCommandBuffer();

    VKFramebuffer* framebuffer = nullptr;
    VKRenderPass* renderPass   = nullptr;

    switch (id) {
        case builtinRenderPassWorld: {
            renderPass  = m_mainRenderPass.get();
            framebuffer = &m_worldFramebuffers[m_rendererContext.getImageIndex()];
            break;
        }

        case builtinRenderPassUI: {
            renderPass = m_uiRenderPass.get();
            framebuffer =
              &m_swapchain->getFramebuffers()->at(m_rendererContext.getImageIndex());
            break;
        }
    }

    renderPass->begin(commandBuffer, framebuffer->getHandle());
    m_renderedVertices = 0u;
    return true;
}

u64 VKRendererBackend::endRenderPass(uint8_t id) {
    auto& commandBuffer = *m_rendererContext.getCommandBuffer();

    VKRenderPass* renderPass = nullptr;

    switch (id) {
        case builtinRenderPassWorld: {
            renderPass = m_mainRenderPass.get();
            break;
        }

        case builtinRenderPassUI: {
            ImGui_ImplVulkan_RenderDrawData(
              ImGui::GetDrawData(), commandBuffer.getHandle()
            );
            renderPass = m_uiRenderPass.get();
            break;
        }
    }
    renderPass->end(commandBuffer);
    return m_renderedVertices;
}

void VKRendererBackend::createBuffers() {
    LOG_DEBUG("Creating buffers");

    m_objectVertexBuffer = createUniqPtr<VKBuffer>(
      m_context.get(), m_device.get(),
      VKBuffer::Properties{
        sizeof(Vertex3) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
          | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        true }
    );

    m_objectIndexBuffer = createUniqPtr<VKBuffer>(
      m_context.get(), m_device.get(),
      VKBuffer::Properties{
        sizeof(uint32_t) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
          | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        true }
    );
}

void VKRendererBackend::createCoreComponents(
  sl::Window& window, const Config& config
) {
    m_context = createUniqPtr<VKContext>(window, config);
    m_device  = createUniqPtr<VKDevice>(m_context.get());
    m_swapchain =
      createUniqPtr<VKSwapchain>(m_device.get(), m_context.get(), window.getSize());

    const auto& [width, height] = window.getSize();

    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{ 11, 16, 47, 255 };

    VKRenderPass::Properties renderPassProperties{
        .area  = glm::vec4{0.0f, 0.0f, width, height},
        .color = backgroundColor,
        .clearFlags =
          (VKRenderPass::clearFlagColorBuffer | VKRenderPass::clearFlagDepthBuffer
           | VKRenderPass::clearFlagStencilBuffer),
        .hasPreviousPass = false,
        .hasNextPass     = true
    };

    m_mainRenderPass = createUniqPtr<VKRenderPass>(
      m_context.get(), m_device.get(), *m_swapchain, renderPassProperties
    );

    renderPassProperties.color           = glm::vec4(0.0f);
    renderPassProperties.clearFlags      = VKRenderPass::clearFlagNone;
    renderPassProperties.hasPreviousPass = true;
    renderPassProperties.hasNextPass     = false;

    m_uiRenderPass = createUniqPtr<VKRenderPass>(
      m_context.get(), m_device.get(), *m_swapchain, renderPassProperties
    );
}

void VKRendererBackend::createSemaphoresAndFences() {
    m_rendererContext.createSemaphoresAndFences(m_context.get(), m_device.get());
}

void VKRendererBackend::onViewportResize(uint32_t width, uint32_t height) {
    m_rendererContext.changeFramebufferSize(width, height);
}

void VKRendererBackend::createCommandBuffers() {
    const auto swapchainImagesCount = m_swapchain->getImageCount();
    m_rendererContext.createCommandBuffers(m_device.get(), swapchainImagesCount);
}

void VKRendererBackend::regenerateFramebuffers() {
    const auto swapchainImagesCount = m_swapchain->getImageCount();
    auto framebuffers               = m_swapchain->getFramebuffers();
    auto depthBuffer                = m_swapchain->getDepthBuffer();

    m_worldFramebuffers.clear();
    m_worldFramebuffers.reserve(swapchainImagesCount);

    framebuffers->clear();
    framebuffers->reserve(swapchainImagesCount);

    for (auto& texture : m_swapchain->getTextures()) {
        auto view = texture->getImage()->getView();

        std::vector<VkImageView> worldAttachments = { view, depthBuffer->getView() };

        m_worldFramebuffers.emplace_back(
          m_context.get(), m_device.get(), m_mainRenderPass->getHandle(),
          m_rendererContext.getFramebufferSize(), worldAttachments
        );

        std::vector<VkImageView> uiAttachments = { view };

        framebuffers->emplace_back(
          m_context.get(), m_device.get(), m_uiRenderPass->getHandle(),
          m_rendererContext.getFramebufferSize(), uiAttachments
        );
    }
}

u32 VKRendererBackend::getRenderPassId(const std::string& renderPass) const {
    if (renderPass == "Builtin.RenderPass.World")
        return builtinRenderPassWorld;
    else if (renderPass == "Builtin.RenderPass.UI")
        return builtinRenderPassUI;
    FAIL("Could not find render pass: {}", renderPass);
}

void VKRendererBackend::recreateSwapchain() {
    if (auto result = m_device->waitIdle(); not isGood(result)) {
        LOG_ERROR("vkDeviceWaitIdle (2) failed: {}", getResultString(result, true));
        return;
    }

    // TODO: implement case when recreation fails
    m_swapchain->recreate();
    LOG_INFO("Resized, booting.");
}

void VKRendererBackend::recordCommands(VKCommandBuffer& commandBuffer) {
    commandBuffer.reset();
    commandBuffer.begin(VKCommandBuffer::BeginFlags{
      .isSingleUse          = false,
      .isRenderpassContinue = false,
      .isSimultaneousUse    = false,
    });

    const auto& [w, h] = m_rendererContext.getFramebufferSize();

    // Dynamic state
    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = (float)h;
    viewport.width    = (float)w;
    viewport.height   = -(float)h;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = w;
    scissor.extent.height               = h;

    vkCmdSetViewport(commandBuffer.getHandle(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.getHandle(), 0, 1, &scissor);
}

VKRenderPass* VKRendererBackend::getRenderPass(u32 id) {
    if (id == builtinRenderPassWorld)
        return m_mainRenderPass.get();
    else if (id == builtinRenderPassUI)
        return m_uiRenderPass.get();
    FAIL("Could not find render pass with id {}", id);
}

bool VKRendererBackend::beginFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();

    if (m_recreatingSwapchain) {
        if (auto result = vkDeviceWaitIdle(logicalDevice); not isGood(result)) {
            LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
            return false;
        }

        LOG_INFO("Recreating swapchain, booting");
        return false;
    }

    // Check if the framebuffer has been resized. If so, a new swapchain must be
    // created.
    if (m_rendererContext.wasFramebufferResized()) {
        recreateSwapchain();
        return false;
    }

    // Wait for the execution of the current frame to complete. The fence being
    // free will allow this one to move on.
    if (not m_rendererContext.getCurrentFence()->wait(UINT64_MAX)) {
        LOG_WARN("In-flight fence wait failure!");
        return false;
    }

    // Acquire the next image from the swap chain. Pass along the semaphore that
    // should signaled when this completes. This same semaphore will later be
    // waited on by the queue submission to ensure this image is available.
    auto nextImageIndex = m_swapchain->acquireNextImageIndex(
      UINT64_MAX, m_rendererContext.getCurrentImageSemaphore()->getHandle(), nullptr
    );

    if (not nextImageIndex) return false;

    m_rendererContext.setImageIndex(*nextImageIndex);

    // Begin recording commands.
    auto& commandBuffer = *m_rendererContext.getCommandBuffer();
    recordCommands(commandBuffer);

    const auto& [w, h] = m_rendererContext.getFramebufferSize();

    m_mainRenderPass->getArea()->z = w;
    m_mainRenderPass->getArea()->w = h;

    return true;
}

bool VKRendererBackend::endFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();
    auto& commandBuffer      = *m_rendererContext.getCommandBuffer();

    commandBuffer.end();

    // Make sure the previous frame is not using this image (i.e. its fence is
    // being waited on) if (context.images_in_flight[context.image_index] !=
    // VK_NULL_HANDLE) {  // was frame vulkan_fence_wait(&context,
    // context.images_in_flight[context.image_index], UINT64_MAX);
    // }
    auto fence = m_rendererContext.acquireImageFence();

    // Submit the queue and wait for the operation to complete.
    // Begin queue submission
    VkSubmitInfo submit_info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    auto commandBufferHandle = commandBuffer.getHandle();

    // Command buffer(s) to be executed.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers    = &commandBufferHandle;

    // The semaphore(s) to be signaled when the queue is complete.
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores =
      m_rendererContext.getCurrentQueueSemaphore()->getHandlePointer();

    // Wait semaphore ensures that the operation cannot begin until the image is
    // available.
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
      m_rendererContext.getCurrentImageSemaphore()->getHandlePointer();

    // Each semaphore waits on the corresponding pipeline stage to complete. 1:1
    // ratio. VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT prevents subsequent
    // colour attachment writes from executing until the semaphore signals (i.e.
    // one frame is presented at atime)
    VkPipelineStageFlags flags[1] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submit_info.pWaitDstStageMask = flags;

    const auto& deviceQueues = m_device->getQueues();

    VkResult result =
      vkQueueSubmit(deviceQueues.graphics, 1, &submit_info, fence->getHandle());

    if (result != VK_SUCCESS) {
        LOG_ERROR(
          "vkQueueSubmit failed with result: {}", getResultString(result, true)
        );
        return false;
    }

    commandBuffer.updateSubmitted();

    m_swapchain->present(
      deviceQueues.graphics, deviceQueues.present,
      m_rendererContext.getCurrentQueueSemaphore()->getHandle(),
      m_rendererContext.getImageIndex()
    );

    m_rendererContext.bumpFrameCounter();

    return true;
}

ResourcePools* VKRendererBackend::getResourcePools() {
    return m_resourcePools.get();
}

// void VKRendererBackend::destroyGeometry(Geometry& geometry) {
//     m_geometries[geometry.getId()].clear();
// }

// Shader* VKRendererBackend::createShader(const Shader::Properties& props) {
//     // TODO: unify with other create* methods?
//     LOG_DEBUG("Backend looking for free shader slot");
//     for (int i = 0; i < m_shaders.size(); ++i) {
//         if (not m_shaders[i]) {
//             LOG_DEBUG("Slot {} found, will create shader", i);
//             m_shaders[i].emplace(
//               m_device.get(), m_context.get(),
//               getRenderPass(getRenderPassId(props.renderPassName)),
//               m_rendererContext, props, i
//             );
//             return m_shaders[i].get();
//         }
//     }
//     LOG_WARN(
//       "Couldn't find slot for a new shader, consider changing configuration to
//       allow more"
//     );
//     return nullptr;
// }

// void VKRendererBackend::destroyShader(Shader& shader) {
//     m_shaders[shader.getId()].clear();
// }

// TextureMap* VKRendererBackend::createTextureMap(
//   const TextureMap::Properties& props, Texture& texture
// ) {
//     LOG_DEBUG("Backend looking for free texture map slot");
//     for (int i = 0; i < m_textureMaps.size(); ++i) {
//         if (not m_textureMaps[i]) {
//             LOG_DEBUG("Slot {} found, will create texture map", i);
//             m_textureMaps[i].emplace(
//               *m_context, *m_device, props, i, *m_textures[texture.getId()]
//             );
//             return m_textureMaps[i].get();
//         }
//     }
//     LOG_WARN(
//       "Couldn't find slot for a new texture map, consider changing configuration
//       to allow more"
//     );
//     return nullptr;
// }

// void VKRendererBackend::destroyTextureMap(TextureMap& textureMap) {
//     m_textureMaps[textureMap.getId()].clear();
// }

}  // namespace sl::vk
