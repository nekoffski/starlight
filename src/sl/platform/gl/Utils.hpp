#pragma once

#include <unordered_map>

#include <glad/glad.h>

namespace sl::platform::gl {

inline const std::unordered_map<int, GLuint> channelsToFormat {
    { 1, GL_RED },
    { 3, GL_RGB },
    { 4, GL_RGBA }
};
}
