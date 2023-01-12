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
#include "ShaderObject.h"

namespace nova::platform::vulkan {

RendererBackend::RendererBackend(core::Window& window, const core::Config& config)
    : m_framebufferSize(window.getSize()) {
    createCoreComponents(window, config);
    regenerateFramebuffers();
    createCommandBuffers();
    createSemaphoresAndFences();

    m_simpleShader = core::createUniqPtr<ShaderObject>(
        m_context.get(), m_device.get(), m_swapchain->getImagesSize()
    );

    LOG_DEBUG("Basic shader created");

    createPipeline();
    createBuffers();

    // temporary texture

    // ---

    m_simpleShader->createUniformBuffer();

    const uint32_t vert_count = 4;
    math::Vertex3 verts[vert_count];

    verts[0].position.x         = -0.5;
    verts[0].position.y         = -0.5;
    verts[0].textureCoordinates = {0.0f, 0.0f};

    verts[1].position.x         = 0.5;
    verts[1].position.y         = 0.5;
    verts[1].textureCoordinates = {1.0f, 1.0f};

    verts[2].position.x         = -0.5;
    verts[2].position.y         = 0.5;
    verts[2].textureCoordinates = {0.0f, 1.0f};

    verts[3].position.x         = 0.5;
    verts[3].position.y         = -0.5;
    verts[3].textureCoordinates = {1.0f, 0.0f};

    const uint32_t index_count    = 6;
    uint32_t indices[index_count] = {0, 1, 2, 0, 3, 1};

    LOG_TRACE("Initializing test vertex and index buffers");

    const auto& graphicsQueue = m_device->getQueues().graphics;

    uploadDataRange(
        m_device->getGraphicsCommandPool(), 0, graphicsQueue, *m_objectVertexBuffer, 0,
        sizeof(math::Vertex3) * vert_count, verts
    );

    uploadDataRange(
        m_device->getGraphicsCommandPool(), 0, graphicsQueue, *m_objectIndexBuffer, 0,
        sizeof(uint32_t) * index_count, indices
    );

    core::Id objectId = m_simpleShader->acquireResources();
    ASSERT(objectId != core::invalidId, "Could not acquire shader resources");

    // create temp texture
    static uint32_t dim      = 256;
    static uint32_t channels = 4;

    static std::vector<uint8_t> pixels(dim * dim * channels, 255);

    // Each pixel.
    for (uint64_t row = 0; row < dim; ++row) {
        for (uint64_t col = 0; col < dim; ++col) {
            uint64_t index     = (row * dim) + col;
            uint64_t index_bpp = index * channels;
            if (row % 2) {
                if (col % 2) {
                    pixels[index_bpp + 0] = 0;
                    pixels[index_bpp + 1] = 0;
                }
            } else {
                if (!(col % 2)) {
                    pixels[index_bpp + 0] = 0;
                    pixels[index_bpp + 1] = 0;
                }
            }
        }
    }

    m_testTexture.emplace(
        m_context.get(), m_device.get(), "test-texture", false, dim, dim, channels, pixels.data(),
        false
    );
}

void RendererBackend::uploadDataRange(
    VkCommandPool pool, VkFence fence, VkQueue queue, Buffer& outBuffer, uint64_t offset,
    uint64_t size, void* data
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

// TODO: pass as const ref
void RendererBackend::updateObject(const gfx::GeometryRenderData& geometryRenderData) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    // TODO: ITS REALLY TEMPORARY
    const_cast<gfx::GeometryRenderData&>(geometryRenderData).textures[0] = m_testTexture.get();

    m_simpleShader->updateObject(
        *m_pipeline, commandBuffer.getHandle(), geometryRenderData, m_frameInfo.imageIndex
    );

    m_pipeline->bind(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

    // Bind vertex buffer at offset.
    VkDeviceSize offsets[1] = {0};

    vkCmdBindVertexBuffers(
        commandBuffer.getHandle(), 0, 1, m_objectVertexBuffer->getHandlePointer(),
        (VkDeviceSize*)offsets
    );

    // Bind index buffer at offset.
    vkCmdBindIndexBuffer(
        commandBuffer.getHandle(), m_objectIndexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32
    );

    // Issue the draw.
    vkCmdDrawIndexed(commandBuffer.getHandle(), 6, 1, 0, 0, 0);
    // TODO: end temporary test code
}

void RendererBackend::updateGlobalState(const gfx::GlobalState& globalState) {
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];

    m_renderPass->setAmbient(globalState.ambientColor);

    // temp test code
    m_pipeline->bind(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);

    m_simpleShader->getGlobalUBO().projection = globalState.projectionMatrix;
    m_simpleShader->getGlobalUBO().view       = globalState.viewMatrix;

    m_simpleShader->updateGlobalState(
        globalState, commandBuffer.getHandle(), m_frameInfo.imageIndex, *m_pipeline
    );
}

void RendererBackend::createBuffers() {
    LOG_DEBUG("Creating buffers");

    m_objectVertexBuffer = core::createUniqPtr<Buffer>(
        m_context.get(), m_device.get(),
        Buffer::Properties{
            sizeof(math::Vertex3) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            true}
    );

    m_objectIndexBuffer = core::createUniqPtr<Buffer>(
        m_context.get(), m_device.get(),
        Buffer::Properties{
            sizeof(uint32_t) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            true}
    );
}

void RendererBackend::createPipeline() {
    // Pipeline creation
    const auto& [w, h] = m_framebufferSize;

    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = (float)w;
    viewport.width    = (float)w;
    viewport.height   = -(float)h;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = w;
    scissor.extent.height               = h;

    // Attributes
    uint32_t offset               = 0;
    const int32_t attribute_count = 2;
    std::vector<VkVertexInputAttributeDescription> attribute_descriptions(attribute_count);
    // Position

    VkFormat formats[attribute_count] = {VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32_SFLOAT};
    uint64_t sizes[attribute_count]   = {sizeof(math::Vec3f), sizeof(math::Vec2f)};

    for (uint32_t i = 0; i < attribute_count; ++i) {
        attribute_descriptions[i].binding  = 0;  // binding index - should match binding desc
        attribute_descriptions[i].location = i;  // attrib location
        attribute_descriptions[i].format   = formats[i];
        attribute_descriptions[i].offset   = offset;
        offset += sizes[i];
    }

    // TODO: Desciptor set layouts.

    // Stages
    // NOTE: Should match the number of shader->stages.
    std::vector<VkPipelineShaderStageCreateInfo> stage_create_infos(2);
    const auto& shaderStages = m_simpleShader->getStages();

    for (uint32_t i = 0; i < 2; ++i) {
        auto stageCreateInfo = shaderStages[i].getStageCreateInfo();

        stage_create_infos[i].sType = stageCreateInfo.sType;
        stage_create_infos[i]       = stageCreateInfo;
    }

    Pipeline::Properties props;

    std::vector<VkDescriptorSetLayout> descriptorSetLayout = {
        m_simpleShader->getGlobalDescriptorSetLayout(),
        m_simpleShader->getObjectDescriptorSetLayout()};

    // TODO: seems like a lot of copying, consider passing a vector view?
    props.vertexAttributes     = attribute_descriptions;
    props.stages               = stage_create_infos;
    props.scissor              = scissor;
    props.viewport             = viewport;
    props.polygonMode          = VK_POLYGON_MODE_FILL;
    props.descriptorSetLayouts = descriptorSetLayout;

    m_pipeline =
        core::createUniqPtr<Pipeline>(m_context.get(), m_device.get(), *m_renderPass, props);

    LOG_DEBUG("Pipeline created");
}

void RendererBackend::createCoreComponents(core::Window& window, const core::Config& config) {
    m_context   = core::createUniqPtr<Context>(window, config);
    m_device    = core::createUniqPtr<Device>(m_context.get());
    m_swapchain = core::createUniqPtr<Swapchain>(m_device.get(), m_context.get(), window.getSize());

    const auto& [width, height] = window.getSize();

    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{117, 210, 98, 255};

    RenderPass::Properties renderPassProperties{
        .area  = glm::vec4{0.0f, 0.0f, width, height},
        .color = backgroundColor,
    };

    m_renderPass = core::createUniqPtr<RenderPass>(
        m_context.get(), m_device.get(), *m_swapchain, renderPassProperties
    );
}

void RendererBackend::createSemaphoresAndFences() {
    m_imageAvailableSemaphores.reserve(s_maxFramesInFlight);
    m_queueCompleteSemaphores.reserve(s_maxFramesInFlight);

    m_imagesInFlight.resize(s_maxFramesInFlight);
    m_inFlightFences.reserve(s_maxFramesInFlight);

    const auto contextPointer = m_context.get();
    const auto devicePointer  = m_device.get();

    REPEAT(s_maxFramesInFlight) {
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

    framebuffers->clear();
    framebuffers->reserve(swapchainImagesCount);

    for (auto& view : *m_swapchain->getImageViews()) {
        uint32_t attachment_count            = 2;
        std::vector<VkImageView> attachments = {view, depthBuffer->getView()};

        framebuffers->emplace_back(
            m_context.get(), m_device.get(), m_renderPass->getHandle(), m_framebufferSize,
            attachments
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

    m_renderPass->getArea()->z = m_framebufferSize.width;
    m_renderPass->getArea()->w = m_framebufferSize.height;

    auto& framebuffer = m_swapchain->getFramebuffers()->at(m_frameInfo.imageIndex);
    m_renderPass->begin(commandBuffer, framebuffer.getHandle());

    return true;
}

bool RendererBackend::endFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();
    auto& commandBuffer      = m_commandBuffers[m_frameInfo.imageIndex];

    m_renderPass->end(commandBuffer);
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

    m_frameInfo.currentFrame = (m_frameInfo.currentFrame + 1) % s_maxFramesInFlight;

    return true;
}

}  // namespace nova::platform::vulkan
