file(GLOB_RECURSE EDITOR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(EDITOR_TARGET starlight-editor)
set(EDITOR_LIBS asan starlight-core starlight-renderer starlight-resource starlight-scene starlight-ui)

add_executable(${EDITOR_TARGET} ${EDITOR_SRC})
set_property(TARGET ${EDITOR_TARGET} PROPERTY CXX_STANDARD 20)
target_include_directories(${EDITOR_TARGET} PUBLIC ${SL_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${EDITOR_TARGET} PUBLIC ${EDITOR_LIBS})
