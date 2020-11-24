
set(SL_AI_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_AI_SRC ${SL_AI_PATH}/*.cpp)

add_library(sl_ai STATIC ${SL_AI_SRC})
target_link_libraries(sl_ai PUBLIC ${FuzzyLiteLibrary})
include_directories(PUBLIC ${THIRDPARTY}/fuzzylite/fuzzylite)
message(${THIRDPARTY})
set_property(TARGET sl_ai PROPERTY CXX_STANDARD 20)
target_include_directories(sl_ai PUBLIC ${STARLIGHT_HEADERS})
