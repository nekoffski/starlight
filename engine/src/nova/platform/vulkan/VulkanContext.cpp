#include "VulkanContext.h"

#include <kc/core/Utils.hpp>

// TODO: make it more generic
#include "nova/platform/glfw/Vulkan.h"

#include "nova/core/Window.h"

namespace nova::platform::vulkan {

namespace details {

VulkanDispatcherLoader::VulkanDispatcherLoader()
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

static vk::Instance createVulkanInstance(vk::AllocationCallbacks* allocator);

static vk::DebugUtilsMessengerEXT createDebugMessenger(vk::Instance& instance);

VulkanContext::VulkanContext(core::Window& window)
    : m_allocator(nullptr)
    , m_instance(createVulkanInstance(m_allocator))
#ifdef DEBUG
    , m_debugMessenger(createDebugMessenger(m_instance))
#endif
    , m_surface(glfw::createVulkanSurface(m_instance, window.getHandle(), m_allocator))
    , m_device(m_instance) {
    LOG_TRACE("Vulkan context initialized");
}

VulkanContext::~VulkanContext() {
    if (m_debugMessenger) m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger);
    if (m_surface) m_instance.destroySurfaceKHR(m_surface);
    m_instance.destroy();
}

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

static vk::Instance createVulkanInstance(vk::AllocationCallbacks* allocator) {
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

    auto instance = vk::createInstance(createInfo, allocator);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

    return instance;
}

static vk::DebugUtilsMessengerEXT createDebugMessenger(vk::Instance& instance) {
    LOG_TRACE("Creating Vulkan debugger");

    const auto messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

    const auto messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

    return instance.createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT(
        {}, messageSeverity, messageType, debugMessengerCallback
    ));
}

}  // namespace nova::platform::vulkan
