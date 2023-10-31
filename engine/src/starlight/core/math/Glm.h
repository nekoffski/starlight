#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <kc/math/Format.h>

namespace sl {

namespace math {
using namespace glm;
}

using Vec2f = math::vec2;
using Vec3f = math::vec3;
using Vec4f = math::vec4;

using Mat2f = math::mat2;
using Mat3f = math::mat3;
using Mat4f = math::mat4;

const Mat4f identityMatrix = Mat4f{ 1.0f };

const Vec3f worldUp    = Vec3f{ 0.0f, 1.0f, 0.0f };
const Vec3f worldRight = Vec3f{ 1.0f, 0.0f, 0.0 };
const Vec3f worldFront = Vec3f{ 0.0f, 0.0f, 1.0f };

}  // namespace sl
