#pragma once

#include <vector>
#include <array>
#include <optional>
#include <memory>

#include "starlight/core/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class Swapchain {
   public:
    explicit Swapchain(Device* device, Context* context, const Size2u32& size);
    ~Swapchain();

    void changeSize(const Size2u32& size);

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

    Size2u32 m_viewportSize;

    VkSurfaceFormatKHR m_imageFormat;
    VkSwapchainKHR m_handle;
    VkExtent2D m_swapchainExtent;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_views;

    std::unique_ptr<Image> m_depthBuffer;

    // TODO: redefine to fixed-size array
    std::vector<Framebuffer> m_framebuffers;
};

}  // namespace sl::vk
