add_subdirectory(libkc)
add_subdirectory(expected)

# imgui

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB_RECURSE IMGUI_SRC ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/misc/*.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui_demo.cpp )
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set_property(TARGET imgui PROPERTY CXX_STANDARD 20)
target_link_libraries(imgui PUBLIC freetype)
target_include_directories(imgui PUBLIC /usr/include/freetype2) # TODO: generic way