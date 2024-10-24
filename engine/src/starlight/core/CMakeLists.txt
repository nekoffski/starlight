find_package(VulkanLoader)
find_package(VulkanHeaders REQUIRED)
find_package(glfw3)
find_package(spdlog)
find_package(fmt)

set(SL_LIBS stb dl kc glfw spdlog::spdlog fmt::fmt Vulkan::Loader vulkan-headers::vulkan-headers)
set(SL_TARGET starlight-core)
file(GLOB_RECURSE SL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${SL_TARGET} STATIC ${SL_SRC})
target_include_directories(${SL_TARGET} PRIVATE ${SL_INCLUDE})
set_property(TARGET ${SL_TARGET} PROPERTY CXX_STANDARD 20)
target_link_libraries(${SL_TARGET} PUBLIC ${SL_LIBS})
