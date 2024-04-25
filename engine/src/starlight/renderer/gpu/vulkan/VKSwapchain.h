#pragma once

#include <vector>
#include <array>
#include <optional>
#include <memory>

#include "starlight/core/math/Size.hpp"

#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"
#include "Vulkan.h"
#include "VKTexture.h"

#include "fwd.h"

namespace sl::vk {

class VKSwapchain {
public:
    explicit VKSwapchain(
      VKBackendAccessor& backendAccessor, u32 viewportWidth, u32 viewportHeight
    );
    ~VKSwapchain();

    std::optional<uint32_t> acquireNextImageIndex(
      Nanoseconds timeout, VkSemaphore imageSemaphore, VkFence fence
    );

    void present(
      VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderSemaphore,
      uint32_t presentImageIndex
    );

    void recreate(u32 viewportWidth, u32 viewportHeight);

    u32 getImageCount() const;

    VKTexture* getFramebuffer(u64 id);
    VKTexture* getDepthBuffer();
    VkSurfaceFormatKHR getSurfaceFormat() const;

    std::span<LocalPtr<VKTexture>> getTextures();

private:
    void create();
    void destroy();

    void createSwapchain();
    void createImages();

    VKBackendAccessor& m_backendAccessor;
    VKContext& m_context;
    VKLogicalDevice& m_device;

    u32 m_viewportWidth;
    u32 m_viewportHeight;

    VkSurfaceFormatKHR m_imageFormat;
    VkSwapchainKHR m_handle;
    VkExtent2D m_swapchainExtent;

    LocalPtr<VKTexture> m_depthTexture;
    std::vector<LocalPtr<VKTexture>> m_textures;

    u32 m_imageCount;
};

}  // namespace sl::vk
