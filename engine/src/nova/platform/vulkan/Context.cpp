
#include "Context.h"
#include <kc/core/Utils.hpp>

// // TODO: make it more generic
#include "nova/platform/glfw/Vulkan.h"

#include "nova/core/Window.h"

namespace nova::platform::vulkan {

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, [[maybe_unused]] void* pUserData
);

VkApplicationInfo createApplicationInfo(const core::Config& config);

std::vector<const char*> getRequiredExtensions();
std::vector<const char*> getLayers();

VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo();

VkInstanceCreateInfo createInstanceCreateInfo(
    const VkApplicationInfo& applicationInfo, std::vector<const char*>& extensions,
    std::vector<const char*>& layers
);

}  // namespace

Context::Context(core::Window& window, const core::Config& config)
    : m_allocator(nullptr)
    , m_instance(nullptr)
#ifdef NV_VK_DEBUG
    , m_debugMessenger(nullptr)
#endif
    , m_surface(nullptr) {

    auto applicationInfo    = createApplicationInfo(config);
    auto requiredExtensions = getRequiredExtensions();

    LOG_INFO("Vulkan extensions to enable:");
    for (const auto& extension : requiredExtensions) LOG_INFO("\t{}", extension);

    auto layers = getLayers();

    if (layers.size() > 0) {
        LOG_INFO("Vulkan layers to enable:");
        for (const auto& layer : layers) LOG_INFO("\t{}", layer);
    }

    auto instanceCreateInfo = createInstanceCreateInfo(applicationInfo, requiredExtensions, layers);

    VK_ASSERT(vkCreateInstance(&instanceCreateInfo, m_allocator, &m_instance));
    LOG_INFO("Vulkan Instance initialized");

#ifdef NV_VK_DEBUG
    static const auto debugFactoryFunctionName = "vkCreateDebugUtilsMessengerEXT";

    auto debugCreateInfo = createDebugMessengerCreateInfo();
    auto factory         = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(m_instance, debugFactoryFunctionName)
    );

    ASSERT(factory, "Failed to create debug messenger factory");
    VK_ASSERT(factory(m_instance, &debugCreateInfo, m_allocator, &m_debugMessenger));

    LOG_INFO("Created Vulkan Debug Messenger");
#endif

    glfw::createVulkanSurface(m_instance, window.getHandle(), m_allocator, &m_surface);
    LOG_INFO("Vulkan Surface created");
    LOG_INFO("Vulkan Context created");
}

Context::~Context() {
    if (m_surface) vkDestroySurfaceKHR(m_instance, m_surface, m_allocator);

#ifdef NV_VK_DEBUG
    if (m_debugMessenger) {
        static const auto debugDestructorFunctionName = "vkDestroyDebugUtilsMessengerEXT";

        auto destructor = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(m_instance, debugDestructorFunctionName)
        );
        destructor(m_instance, m_debugMessenger, m_allocator);
    }
#endif

    if (m_instance) vkDestroyInstance(m_instance, m_allocator);
}

VkAllocationCallbacks* Context::getAllocator() const { return m_allocator; }

VkInstance Context::getInstance() const { return m_instance; }

VkSurfaceKHR Context::getSurface() const { return m_surface; }

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, [[maybe_unused]] void* pUserData
) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LOG_WARN("VK_DEBUG_LAYER - {}", pCallbackData->pMessage);

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LOG_ERROR("VK_DEBUG_LAYER - {}", pCallbackData->pMessage);

        default:
            LOG_INFO("VK_DEBUG_LAYER - {}", pCallbackData->pMessage);
    }

    return false;
}

VkApplicationInfo createApplicationInfo(const core::Config& config) {
    VkApplicationInfo applicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};

    applicationInfo.apiVersion       = VK_API_VERSION_1_3;
    applicationInfo.pApplicationName = config.window.name.c_str();
    applicationInfo.pEngineName      = "Nova Engine";
    applicationInfo.engineVersion =
        VK_MAKE_VERSION(config.version.major, config.version.minor, config.version.build);

    return applicationInfo;
}

std::vector<const char*> getRequiredExtensions() {
    const auto platformRequiredExtensions = glfw::getRequiredExtensions();

    std::vector<const char*> requiredExtensions = {
#ifdef NV_VK_DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
    };

    requiredExtensions.insert(
        requiredExtensions.end(), platformRequiredExtensions.begin(),
        platformRequiredExtensions.end()
    );

    return requiredExtensions;
}

std::vector<const char*> getLayers() {
    std::vector<const char*> layers;

    // validation layers
#ifdef NV_VK_DEBUG
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

    std::move(
        requiredValidationLayersNames.begin(), requiredValidationLayersNames.end(),
        std::back_inserter(layers)
    );
#endif

    return layers;
}

VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo() {
    uint32_t logSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};

    debugCreateInfo.messageSeverity = logSeverity;
    debugCreateInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

    debugCreateInfo.pfnUserCallback = debugMessengerCallback;

    return debugCreateInfo;
}

VkInstanceCreateInfo createInstanceCreateInfo(
    const VkApplicationInfo& applicationInfo, std::vector<const char*>& extensions,
    std::vector<const char*>& layers
) {
    VkInstanceCreateInfo instanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};

    instanceCreateInfo.pApplicationInfo        = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount   = extensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    const auto layersSize = layers.size();

    instanceCreateInfo.enabledLayerCount   = layers.size();
    instanceCreateInfo.ppEnabledLayerNames = layersSize > 0 ? layers.data() : nullptr;

    return instanceCreateInfo;
}

}  // namespace

}  // namespace nova::platform::vulkan
