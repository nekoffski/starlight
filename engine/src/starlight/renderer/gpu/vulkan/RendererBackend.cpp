#include "RendererBackend.h"

#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "Fence.h"
#include "Image.h"
#include "Framebuffer.h"
#include "Buffer.h"
#include "ShaderStage.h"
#include "MaterialShader.h"
#include "TextureLoader.h"

// #include "starlight/renderer/MaterialManager.h"

namespace sl::vk {

RendererBackend::RendererBackend(sl::Window& window, const Config& config)
    : m_framebufferSize(window.getSize()) {
    createCoreComponents(window, config);
    regenerateFramebuffers();
    createCommandBuffers();
    createSemaphoresAndFences();

    m_materialShader = createUniqPtr<MaterialShader>(
        m_context.get(), m_device.get(), m_swapchain->getImagesSize(), m_framebufferSize,
        *m_mainRenderPass
    );

    m_uiShader = createUniqPtr<UIShader>(
        m_context.get(), m_device.get(), m_swapchain->getImagesSize(), m_framebufferSize,
        *m_uiRenderPass
    );

    LOG_DEBUG("Basic shader created");

    createBuffers();

    m_textureLoader = createUniqPtr<TextureLoader>(m_context.get(), m_device.get());

    for (auto& geometry : m_geometries) geometry.id = invalidId;
}

void RendererBackend::acquireGeometryResources(
    Geometry& geometry, uint32_t vertexSize, uint32_t vertexCount, void* vertexData,
    std::span<uint32_t> indices
) {
    LOG_TRACE(
        "Acquiring geometry resources vertexSize={}, vertexCount={}, indicesCount={}, vertexDataPtr={}",
        vertexSize, vertexCount, indices.size(), vertexData
    );

    bool isReupload = geometry.internalId != invalidId;

    GeometryData oldRange;
    GeometryData* internalData = nullptr;

    if (isReupload) {
        LOG_TRACE("Reuploading geometry");
        internalData = &m_geometries[geometry.internalId];
        oldRange     = *internalData;
    } else {
        for (int i = 0; auto& geometryData : m_geometries) {
            if (geometryData.id == invalidId) {
                LOG_DEBUG("Found free geometry id={}", i);
                geometry.internalId = i;
                geometryData.id     = i;
                internalData        = &geometryData;
                break;
            }
            ++i;
        }
    }

    ASSERT(internalData != nullptr, "Could not find geometry slot");

    auto pool  = m_device->getGraphicsCommandPool();
    auto queue = m_device->getQueues().graphics;

    internalData->vertexBufferOffset = m_geometryVertexOffset;
    internalData->vertexCount        = vertexCount;
    internalData->vertexElementSize  = vertexSize;

    const auto verticesTotalSize = internalData->getVerticesTotalSize();

    uploadDataRange(
        pool, nullptr, queue, *m_objectVertexBuffer, internalData->vertexBufferOffset,
        verticesTotalSize, vertexData
    );

    m_geometryVertexOffset += verticesTotalSize;

    if (indices.size() > 0) {
        // TODO: allow for indices to be optional
        internalData->indexBufferOffset = m_geometryIndexOffset;
        internalData->indexCount        = indices.size();
        internalData->indexElementSize  = sizeof(uint32_t);

        const auto indicesTotalSize = internalData->getIndicesTotalSize();

        uploadDataRange(
            pool, nullptr, queue, *m_objectIndexBuffer, internalData->indexBufferOffset,
            indicesTotalSize, indices.data()
        );

        m_geometryIndexOffset += indicesTotalSize;
    }

    if (internalData->generation == invalidId)
        internalData->generation = 0;
    else
        internalData->generation++;

    if (isReupload) {
        // TODO: free data range

        if (oldRange.indexElementSize > 0) {
            // freeDataRange
        }
    }
}

void RendererBackend::releaseGeometryResources(Geometry& geometry) {
    if (geometry.internalId != invalidId) {
        vkDeviceWaitIdle(m_device->getLogicalDevice());

        auto internalData = &m_geometries[geometry.internalId];

        // TODO: free_data_range

        if (internalData->indexElementSize > 0) {
            // TODO: free
        }

        internalData->id         = invalidId;
        internalData->generation = invalidId;
        // TODO: is that required to reset also other properties?
    }
}

void RendererBackend::acquireMaterialResources(Material& material) {
    switch (material.type) {
        case Material::Type::world:
            m_materialShader->acquireResources(material);
            break;
        case Material::Type::ui:
            m_uiShader->acquireResources(material);
            break;
        default:
            LOG_ERROR("Unknown material type, could not determine shader to acquire resources");
    }
}

void RendererBackend::releaseMaterialResources(Material& material) {
    switch (material.type) {
        case Material::Type::world:
            m_materialShader->releaseResources(material);
            break;
        case Material::Type::ui:
            m_uiShader->releaseResources(material);
            break;
        default:
            LOG_ERROR("Unknown material type, could not determine shader to release resources");
    }
}

void RendererBackend::uploadDataRange(
    VkCommandPool pool, VkFence fence, VkQueue queue, Buffer& outBuffer, uint64_t offset,
    uint64_t size, const void* data
) {
    VkMemoryPropertyFlags flags =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    Buffer staging(
        m_context.get(), m_device.get(),
        Buffer::Properties{size, flags, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, true}
    );

    staging.loadData(0, size, 0, data);

    staging.copyTo(
        pool, fence, queue, outBuffer.getHandle(),
        VkBufferCopy{.srcOffset = 0, .dstOffset = offset, .size = size}
    );
}

TextureLoader* RendererBackend::getTextureLoader() const { return m_textureLoader.get(); }

void RendererBackend::drawGeometry(const GeometryRenderData& geometryRenderData) {
    if (geometryRenderData.geometry->internalId == invalidId) {
        LOG_ERROR("Could not draw Geometry with invalid id");
        return;
    }

    auto& bufferData    = m_geometries[geometryRenderData.geometry->internalId];
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    Material* material = geometryRenderData.geometry->material;
    ASSERT(material, "Invalid material handle");

    // TODO
    // if (not material) material = MaterialManager::get().getDefaultMaterial();

    switch (material->type) {
        case Material::Type::ui:
            m_uiShader->setModel(commandBuffer.getHandle(), geometryRenderData.model);
            m_uiShader->applyMaterial(commandBuffer.getHandle(), m_frameInfo.imageIndex, *material);
            m_uiShader->use(commandBuffer);
            break;

        case Material::Type::world:
            m_materialShader->setModel(commandBuffer.getHandle(), geometryRenderData.model);
            m_materialShader->applyMaterial(
                commandBuffer.getHandle(), m_frameInfo.imageIndex, *material
            );
            m_materialShader->use(commandBuffer);
            break;

        default:
            LOG_ERROR("Could not draw geometry, unknown material type");
            return;
    }

    VkDeviceSize offsets[1] = {bufferData.vertexBufferOffset};

    vkCmdBindVertexBuffers(
        commandBuffer.getHandle(), 0, 1, m_objectVertexBuffer->getHandlePointer(),
        (VkDeviceSize*)offsets
    );

    if (bufferData.indexCount > 0) {
        vkCmdBindIndexBuffer(
            commandBuffer.getHandle(), m_objectIndexBuffer->getHandle(),
            bufferData.indexBufferOffset, VK_INDEX_TYPE_UINT32
        );
        vkCmdDrawIndexed(commandBuffer.getHandle(), bufferData.indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer.getHandle(), bufferData.vertexCount, 1, 0, 0);
    }
}

void RendererBackend::updateGlobalWorldState(const GlobalState& globalState) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    m_mainRenderPass->setAmbient(globalState.ambientColor);

    // temp test code
    m_materialShader->use(commandBuffer);
    m_materialShader->getGlobalUBO().projection = globalState.projectionMatrix;
    m_materialShader->getGlobalUBO().view       = globalState.viewMatrix;

    m_materialShader->updateGlobalWorldState(commandBuffer.getHandle(), m_frameInfo.imageIndex);
}

void RendererBackend::updateGlobalUIState(Mat4f projection, Mat4f view, int32_t mode) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    m_uiShader->use(commandBuffer);
    m_uiShader->getGlobalUBO().projection = projection;
    m_uiShader->getGlobalUBO().view       = view;

    m_uiShader->updateGlobalWorldState(commandBuffer.getHandle(), m_frameInfo.imageIndex);
}

bool RendererBackend::beginRenderPass(uint8_t id) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    Framebuffer* framebuffer = nullptr;
    RenderPass* renderPass   = nullptr;

    switch (id) {
        case builtinRenderPassWorld: {
            renderPass  = m_mainRenderPass.get();
            framebuffer = &m_worldFramebuffers[m_frameInfo.imageIndex];
            break;
        }

        case builtinRenderPassUI: {
            renderPass  = m_uiRenderPass.get();
            framebuffer = &m_swapchain->getFramebuffers()->at(m_frameInfo.imageIndex);
            break;
        }
    }

    renderPass->begin(commandBuffer, framebuffer->getHandle());

    switch (id) {
        case builtinRenderPassWorld:
            m_materialShader->use(commandBuffer);
            break;

        case builtinRenderPassUI:
            m_uiShader->use(commandBuffer);
            break;
    }

    return true;
}

bool RendererBackend::endRenderPass(uint8_t id) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    RenderPass* renderPass = nullptr;

    switch (id) {
        case builtinRenderPassWorld: {
            renderPass = m_mainRenderPass.get();
            break;
        }

        case builtinRenderPassUI: {
            renderPass = m_uiRenderPass.get();
            break;
        }
    }
    renderPass->end(commandBuffer);

    return true;
}

void RendererBackend::createBuffers() {
    LOG_DEBUG("Creating buffers");

    m_objectVertexBuffer = createUniqPtr<Buffer>(
        m_context.get(), m_device.get(),
        Buffer::Properties{
            sizeof(Vertex3) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            true}
    );

    m_objectIndexBuffer = createUniqPtr<Buffer>(
        m_context.get(), m_device.get(),
        Buffer::Properties{
            sizeof(uint32_t) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            true}
    );
}

void RendererBackend::createCoreComponents(sl::Window& window, const Config& config) {
    m_context   = createUniqPtr<Context>(window, config);
    m_device    = createUniqPtr<Device>(m_context.get());
    m_swapchain = createUniqPtr<Swapchain>(m_device.get(), m_context.get(), window.getSize());

    const auto& [width, height] = window.getSize();

    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{117, 210, 98, 255};

    RenderPass::Properties renderPassProperties{
        .area  = glm::vec4{0.0f, 0.0f, width, height},
        .color = backgroundColor,
        .clearFlags =
            (RenderPass::clearFlagColorBuffer | RenderPass::clearFlagDepthBuffer |
             RenderPass::clearFlagStencilBuffer),
        .hasPreviousPass = false,
        .hasNextPass     = true
    };

    m_mainRenderPass = createUniqPtr<RenderPass>(
        m_context.get(), m_device.get(), *m_swapchain, renderPassProperties
    );

    renderPassProperties.color           = glm::vec4(0.0f);
    renderPassProperties.clearFlags      = RenderPass::clearFlagNone;
    renderPassProperties.hasPreviousPass = true;
    renderPassProperties.hasNextPass     = false;

    m_uiRenderPass = createUniqPtr<RenderPass>(
        m_context.get(), m_device.get(), *m_swapchain, renderPassProperties
    );
}

void RendererBackend::createSemaphoresAndFences() {
    m_imageAvailableSemaphores.reserve(maxFramesInFlight);
    m_queueCompleteSemaphores.reserve(maxFramesInFlight);

    m_imagesInFlight.resize(maxFramesInFlight);
    m_inFlightFences.reserve(maxFramesInFlight);

    const auto contextPointer = m_context.get();
    const auto devicePointer  = m_device.get();

    REPEAT(maxFramesInFlight) {
        m_imageAvailableSemaphores.emplace_back(contextPointer, devicePointer);
        m_queueCompleteSemaphores.emplace_back(contextPointer, devicePointer);
        m_inFlightFences.emplace_back(m_context.get(), m_device.get(), Fence::State::signaled);
    }
}

void RendererBackend::onViewportResize(uint32_t width, uint32_t height) {
    m_framebufferSize.width = width;
    m_framebufferSize.width = height;

    m_frameInfo.lastFramebufferSizeGeneration = m_frameInfo.framebufferSizeGeneration;
    m_frameInfo.framebufferSizeGeneration++;

    LOG_TRACE(
        "Vulkan renderer backend onViewportResize {}/{}/{}", width, height,
        m_frameInfo.framebufferSizeGeneration
    );
}

void RendererBackend::createCommandBuffers() {
    const auto swapchainImagesCount = m_swapchain->getImagesSize();
    LOG_TRACE("Creating {} command buffers", swapchainImagesCount);

    m_commandBuffers.reserve(swapchainImagesCount);

    for (int i = 0; i < swapchainImagesCount; ++i) {
        m_commandBuffers.emplace_back(
            m_device.get(), m_device->getGraphicsCommandPool(), CommandBuffer::Severity::primary
        );
    }
}

void RendererBackend::regenerateFramebuffers() {
    const auto swapchainImagesCount = m_swapchain->getImagesSize();
    auto framebuffers               = m_swapchain->getFramebuffers();
    auto depthBuffer                = m_swapchain->getDepthBuffer();

    m_worldFramebuffers.clear();
    m_worldFramebuffers.reserve(swapchainImagesCount);

    framebuffers->clear();
    framebuffers->reserve(swapchainImagesCount);

    for (auto& view : *m_swapchain->getImageViews()) {
        std::vector<VkImageView> worldAttachments = {view, depthBuffer->getView()};

        m_worldFramebuffers.emplace_back(
            m_context.get(), m_device.get(), m_mainRenderPass->getHandle(), m_framebufferSize,
            worldAttachments
        );

        std::vector<VkImageView> uiAttachments = {view};

        framebuffers->emplace_back(
            m_context.get(), m_device.get(), m_uiRenderPass->getHandle(), m_framebufferSize,
            uiAttachments

        );
    }
}

RendererBackend::~RendererBackend() { vkDeviceWaitIdle(m_device->getLogicalDevice()); }

void RendererBackend::recreateSwapchain() {
    if (auto result = vkDeviceWaitIdle(m_device->getLogicalDevice()); not isGood(result)) {
        LOG_ERROR("vkDeviceWaitIdle (2) failed: {}", getResultString(result, true));
        return;
    }

    // TODO: implement case when recreation fails
    m_swapchain->recreate();
    LOG_INFO("Resized, booting.");
}

void RendererBackend::recordCommands(CommandBuffer& commandBuffer) {
    commandBuffer.reset();
    commandBuffer.begin(CommandBuffer::BeginFlags{
        .isSingleUse          = false,
        .isRenderpassContinue = false,
        .isSimultaneousUse    = false,
    });

    // Dynamic state
    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = (float)m_framebufferSize.height;
    viewport.width    = (float)m_framebufferSize.width;
    viewport.height   = -(float)m_framebufferSize.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = m_framebufferSize.width;
    scissor.extent.height               = m_framebufferSize.height;

    vkCmdSetViewport(commandBuffer.getHandle(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.getHandle(), 0, 1, &scissor);
}

bool RendererBackend::beginFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();

    if (m_recreatingSwapchain) {
        if (auto result = vkDeviceWaitIdle(logicalDevice); not isGood(result)) {
            LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
            return false;
        }

        LOG_INFO("Recreating swapchain, booting");
        return false;
    }

    // Check if the framebuffer has been resized. If so, a new swapchain must be created.
    if (m_frameInfo.framebufferSizeGeneration != m_frameInfo.lastFramebufferSizeGeneration) {
        recreateSwapchain();
        return false;
    }

    // Wait for the execution of the current frame to complete. The fence being free will allow
    // this one to move on.
    if (not m_inFlightFences[m_frameInfo.currentFrame].wait(UINT64_MAX)) {
        LOG_WARN("In-flight fence wait failure!");
        return false;
    }

    // Acquire the next image from the swap chain. Pass along the semaphore that should signaled
    // when this completes. This same semaphore will later be waited on by the queue submission
    // to ensure this image is available.
    auto nextImageIndex = m_swapchain->acquireNextImageIndex(
        UINT64_MAX, m_imageAvailableSemaphores[m_frameInfo.currentFrame].getHandle(), nullptr
    );

    if (not nextImageIndex) return false;

    m_frameInfo.imageIndex = *nextImageIndex;

    // Begin recording commands.
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];
    recordCommands(commandBuffer);

    m_mainRenderPass->getArea()->z = m_framebufferSize.width;
    m_mainRenderPass->getArea()->w = m_framebufferSize.height;

    return true;
}

bool RendererBackend::endFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();
    auto& commandBuffer      = m_commandBuffers[m_frameInfo.imageIndex];

    commandBuffer.end();

    if (m_imagesInFlight[m_frameInfo.imageIndex] != VK_NULL_HANDLE)
        m_imagesInFlight[m_frameInfo.imageIndex]->wait(UINT64_MAX);

    // Make sure the previous frame is not using this image (i.e. its fence is being waited on)
    // if (context.images_in_flight[context.image_index] != VK_NULL_HANDLE) {  // was frame
    // vulkan_fence_wait(&context, context.images_in_flight[context.image_index], UINT64_MAX);
    // }

    // Mark the image fence as in-use by this frame.
    m_imagesInFlight[m_frameInfo.imageIndex] = &m_inFlightFences[m_frameInfo.currentFrame];

    // Reset the fence for use on the next frame
    m_inFlightFences[m_frameInfo.currentFrame].reset();

    // Submit the queue and wait for the operation to complete.
    // Begin queue submission
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};

    auto commandBufferHandle = commandBuffer.getHandle();

    // Command buffer(s) to be executed.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers    = &commandBufferHandle;

    // The semaphore(s) to be signaled when the queue is complete.
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores =
        m_queueCompleteSemaphores[m_frameInfo.currentFrame].getHandlePointer();

    // Wait semaphore ensures that the operation cannot begin until the image is available.
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
        m_imageAvailableSemaphores[m_frameInfo.currentFrame].getHandlePointer();

    // Each semaphore waits on the corresponding pipeline stage to complete. 1:1 ratio.
    // VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT prevents subsequent colour attachment
    // writes from executing until the semaphore signals (i.e. one frame is presented at atime)
    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.pWaitDstStageMask = flags;

    const auto& deviceQueues = m_device->getQueues();

    VkResult result = vkQueueSubmit(
        deviceQueues.graphics, 1, &submit_info,
        m_inFlightFences[m_frameInfo.currentFrame].getHandle()
    );

    if (result != VK_SUCCESS) {
        LOG_ERROR("vkQueueSubmit failed with result: {}", getResultString(result, true));
        return false;
    }

    commandBuffer.updateSubmitted();

    m_swapchain->present(
        deviceQueues.graphics, deviceQueues.present,
        m_queueCompleteSemaphores[m_frameInfo.currentFrame].getHandle(), m_frameInfo.imageIndex
    );

    m_frameInfo.currentFrame = (m_frameInfo.currentFrame + 1) % maxFramesInFlight;

    return true;
}

}  // namespace sl::vk
