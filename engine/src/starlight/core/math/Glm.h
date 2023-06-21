#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <kc/math/Format.h>

namespace sl {

using namespace glm;

using Vec2f = vec2;
using Vec3f = vec3;
using Vec4f = vec4;

using Mat2f = mat2;
using Mat3f = mat3;
using Mat4f = mat4;

const Mat4f identityMatrix = Mat4f{1.0f};

}  // namespace sl
