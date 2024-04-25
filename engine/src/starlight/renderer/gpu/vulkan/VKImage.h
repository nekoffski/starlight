#pragma once

#include <optional>
#include <span>

#include "starlight/core/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"
#include "VKBuffer.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

class VKImage {
public:
    enum class Type : u8 { flat, cubemap };

    struct Properties {
        Type type;
        u32 width;
        u32 height;
        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags memoryFlags;
        bool createView;
        VkImageAspectFlags viewAspectFlags;
        u8 channels;
    };

    explicit VKImage(
      VKBackendAccessor& backendAccesor, const Properties& properties
    );
    explicit VKImage(
      VKBackendAccessor& backendAccesor, const Properties& properties,
      const std::span<u8> pixels
    );
    explicit VKImage(
      VKBackendAccessor& backendAccesor, const Properties& properties, VkImage handle
    );

    ~VKImage();

    void recreate(const Properties& properties, VkImage handle);
    void recreate(const Properties& properties);

    const Properties& getProperties() const;

    VKImage(const VKImage& oth)            = delete;
    VKImage& operator=(const VKImage& oth) = delete;
    VKImage& operator=(VKImage&& oth)      = delete;
    VKImage(VKImage&&)                     = delete;

    void write(u32 offset, std::span<u8> pixels);

    VkImageView getView() const;

private:
    void copyFromBuffer(VKBuffer& buffer, VKCommandBuffer& commandBuffer);

    void transitionLayout(
      VKCommandBuffer& commandBuffer, VkFormat format, VkImageLayout oldLayout,
      VkImageLayout newLayout
    );

    void create();
    void destroy();

    void createImage();
    void allocateAndBindMemory();
    void createView();

    VkImage m_handle;
    VkDeviceMemory m_memory;
    VkImageView m_view;

    Properties m_props;

    VKBackendAccessor& m_backendAccesor;
    VKContext& m_context;
    VKLogicalDevice& m_device;

    // sometimes we want to just wrap an image owned by the swapchain
    bool m_destroyImage;
};

}  // namespace sl::vk
