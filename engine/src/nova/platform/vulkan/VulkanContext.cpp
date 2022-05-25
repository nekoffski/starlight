#include "VulkanContext.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace nova::platform::vulkan {

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/
);

VulkanContext::VulkanContext() {
    vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

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
    std::vector<const char*> requiredExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
    };

    LOG_INFO("Vulkan extensions to enable:");
    for (auto& extension : requiredExtensions) LOG_INFO("{}", extension);

    createInfo.enabledExtensionCount   = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    createInfo.enabledLayerCount       = 0;
    createInfo.ppEnabledLayerNames     = 0;

// validation layers
#ifdef DEBUG
    std::vector<const char*> requiredValidationLayersNames = {
        "VK_LAYER_KHRONOS_validation",
    };

    // get the list of available validation layers
    auto layerProperties = vk::enumerateInstanceLayerProperties();

    for (const auto& requiredLayer : requiredValidationLayersNames) {
        LOG_DEBUG("Searching for layer: {}", requiredLayer);

        bool found = false;

        for (const auto& layer : layerProperties) {
            if (std::strcmp(requiredLayer, layer.layerName) == 0) {
                LOG_DEBUG("Found layer: {}", requiredLayer);
                found = true;
                break;
            }
        }

        ASSERT(found, "Layer {} not found", requiredLayer);
    }

    LOG_DEBUG("All required validation layers found");

    createInfo.enabledLayerCount   = requiredValidationLayersNames.size();
    createInfo.ppEnabledLayerNames = requiredValidationLayersNames.data();
#endif

    instance = vk::createInstance(createInfo, allocator);
    LOG_TRACE("Vulkan instance created");

    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

// create debugger
#ifdef DEBUG
    LOG_TRACE("Creating vulkan debugger");

    const auto messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

    const auto messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

    debugMessenger = instance.createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT(
        {}, messageSeverity, messageType, debugMessengerCallback
    ));
#endif

    LOG_TRACE("Vulkan context initialized");
}

VulkanContext::~VulkanContext() {
#ifdef DEBUG
    if (debugMessenger) instance.destroyDebugUtilsMessengerEXT(debugMessenger);
#endif
    instance.destroy();
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/
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

}  // namespace nova::platform::vulkan
