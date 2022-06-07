#include "Context.h"

#include <kc/core/Utils.hpp>

// TODO: make it more generic
#include "nova/platform/glfw/Vulkan.h"

#include "nova/core/Window.h"

namespace nova::platform::vulkan {

namespace details {

DispatcherLoader::DispatcherLoader()
    : m_vkGetInstanceProcAddr(m_dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"
      )) {
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_vkGetInstanceProcAddr);
}

}  // namespace details

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* pUserData
);

static vk::raii::Instance createInstance(
    vk::AllocationCallbacks* allocator, vk::raii::Context& context
);

static vk::raii::DebugUtilsMessengerEXT createDebugMessenger(vk::raii::Instance& instance);

Context::Context(core::Window& window)
    : m_allocator(nullptr)
    , m_instance(createInstance(m_allocator, m_context))
#ifdef DEBUG
    , m_debugMessenger(createDebugMessenger(m_instance))
#endif
    , m_surface(glfw::createVulkanSurface(m_instance, window.getHandle(), m_allocator))
    , m_device(m_instance, m_surface)
    , m_swapchain(m_device, m_surface, window.getSize())
    , m_mainRenderPass(
          m_device, m_swapchain,
          math::Vec4f{0.0f, 0.0f, m_framebufferSize.width, m_framebufferSize.height},
          math::Vec4f{0.0f, 0.0f, 0.2, 1.0f}, 1.0f, 0
      ) {
    LOG_TRACE("Vulkan context initialized");
}

Context::~Context() { LOG_TRACE("Destroying vulkan context"); }

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, [[maybe_unused]] void* pUserData
) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LOG_WARN("{}", pCallbackData->pMessage);

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LOG_ERROR("{}", pCallbackData->pMessage);

        default:
            LOG_INFO("{}", pCallbackData->pMessage);
    }

    return false;
}

static vk::raii::Instance createInstance(
    vk::AllocationCallbacks* allocator, vk::raii::Context& context
) {
    // set app info
    vk::ApplicationInfo applicationInfo{};
    applicationInfo.pApplicationName   = "Vulkan";
    applicationInfo.applicationVersion = 1;
    applicationInfo.pApplicationName   = "Nova";
    applicationInfo.engineVersion      = 1;
    applicationInfo.apiVersion         = VK_API_VERSION_1_2;

    vk::InstanceCreateInfo createInfo{};
    createInfo.pApplicationInfo = &applicationInfo;

    // get extensions info
    const auto platformRequiredExtensions = glfw::getRequiredExtensions();

    std::vector<const char*> requiredExtensions = {
#ifdef DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif

    };

    requiredExtensions.insert(
        requiredExtensions.end(), platformRequiredExtensions.begin(),
        platformRequiredExtensions.end()
    );

    LOG_INFO("Vulkan extensions to enable:");
    for (const auto& extension : requiredExtensions) LOG_INFO("\t{}", extension);

    createInfo.enabledExtensionCount   = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    createInfo.enabledLayerCount       = 0;
    createInfo.ppEnabledLayerNames     = 0;

// validation layers
#ifdef DEBUG
    // get the list of available validation layers
    auto layersAvailable = vk::enumerateInstanceLayerProperties();
    std::vector<const char*> layerNames{layersAvailable.size(), nullptr};
    std::ranges::transform(layersAvailable, std::begin(layerNames), [](auto& layer) -> const char* {
        return layer.layerName;
    });

    std::vector<const char*> requiredValidationLayersNames = {
        "VK_LAYER_KHRONOS_validation",
    };

    LOG_TRACE("Available Vulkan layers:");
    for (const auto& layerName : layerNames) LOG_TRACE("\t{}", layerName);

    for (const auto& requiredLayer : requiredValidationLayersNames) {
        ASSERT(
            kc::core::contains(layerNames, requiredLayer), "Required layer {} not found",
            requiredLayer
        );
        LOG_DEBUG("Layer {} found", requiredLayer);
    }

    LOG_DEBUG("All required validation layers found");

    createInfo.enabledLayerCount   = requiredValidationLayersNames.size();
    createInfo.ppEnabledLayerNames = requiredValidationLayersNames.data();
#endif

    vk::raii::Instance instance(context, createInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance);

    return instance;
}

static vk::raii::DebugUtilsMessengerEXT createDebugMessenger(vk::raii::Instance& instance) {
    LOG_TRACE("Creating Vulkan debugger");

    vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
    );

    vk::DebugUtilsMessageTypeFlagsEXT messageType(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    );

    vk::DebugUtilsMessengerCreateInfoEXT createInfo(
        {}, messageSeverity, messageType, &debugMessengerCallback
    );

    return vk::raii::DebugUtilsMessengerEXT{instance, createInfo};
}

}  // namespace nova::platform::vulkan
