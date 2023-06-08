#pragma once

#include <vector>
#include <optional>
#include <memory>

#include "starlight/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::platform::vulkan {

class Swapchain {
   public:
    explicit Swapchain(Device* device, Context* context, const math::Size2u32& size);
    ~Swapchain();

    void changeSize(const math::Size2u32& size);

    std::optional<uint32_t> acquireNextImageIndex(
        Nanoseconds timeout, VkSemaphore imageSemaphore, VkFence fence
    );

    void present(
        VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderSemaphore,
        uint32_t presentImageIndex
    );

    void recreate();

    uint64_t getImagesSize() const;

    std::vector<Framebuffer>* getFramebuffers();

    Image* getDepthBuffer();

    std::vector<VkImageView>* getImageViews();

    VkSurfaceFormatKHR getSurfaceFormat() const;

   private:
    void create();
    void destroy();

    void createSwapchain();
    void createImages();

    Device* m_device;
    Context* m_context;

    math::Size2u32 m_viewportSize;

    VkSurfaceFormatKHR m_imageFormat;
    VkSwapchainKHR m_handle;
    VkExtent2D m_swapchainExtent;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_views;

    std::unique_ptr<Image> m_depthBuffer;

    std::vector<Framebuffer> m_framebuffers;
};

}  // namespace sl::platform::vulkan
