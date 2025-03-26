#include "Transform.hh"

#include <glm/gtx/euler_angles.hpp>

namespace sl {

Transform::Transform() :
    Transform(Vec3<f32>{ 0.0f }, Vec3<f32>{ 1.0f }, identityMatrix) {}

Transform::Transform(
  const Vec3<f32>& position, const Vec3<f32>& scale, const Quat& orientation
) :
    m_model(identityMatrix), m_position(position), m_scale(scale),
    m_orientation(orientation), m_updated(false), m_parent(nullptr) {}

Transform* Transform::getParent() const { return m_parent; }

void Transform::setParent(Transform& parent) { m_parent = &parent; }

Transform Transform::fromScale(const Vec3<f32>& scale) {
    return Transform(Vec3<f32>{ 0.0f }, scale, identityMatrix);
}

Transform Transform::fromPosition(const Vec3<f32>& position) {
    return Transform(position);
}

Transform Transform::fromRotation(const Vec3<f32>& axis, const float angle) {
    return Transform::fromOrientation(math::angleAxis(angle, axis));
}

Transform Transform::fromOrientation(const Quat& orientation) {
    return Transform(Vec3<f32>{ 0.0f }, Vec3<f32>{ 1.0f }, orientation);
}

const Vec3<f32>& Transform::getPosition() const { return m_position; }

const Vec3<f32>& Transform::getScale() const { return m_scale; }

const Quat& Transform::getOrientation() const { return m_orientation; }

Transform& Transform::translate(const Vec3<f32>& position) {
    m_position += position;
    m_updated = true;
    return *this;
}

Transform& Transform::rotate(const Quat& rotation) {
    m_orientation = rotation * m_orientation;
    m_updated     = true;
    return *this;
}

Transform& Transform::rotate(const Vec3<f32>& axis, const float angle) {
    return rotate(math::angleAxis(angle, axis));
}

Transform& Transform::scale(float scale) { return this->scale(Vec3<f32>{ scale }); }

Vec3<f32> Transform::getEuler() const { return math::eulerAngles(m_orientation); }

Transform& Transform::scale(const Vec3<f32>& scale) {
    m_scale *= scale;
    m_updated = true;
    return *this;
}

Transform& Transform::setPosition(const Vec3<f32>& position) {
    m_position = position;
    m_updated  = true;
    return *this;
}

Transform& Transform::setScale(const Vec3<f32>& scale) {
    m_scale   = scale;
    m_updated = true;
    return *this;
}

Transform& Transform::setOrientation(const Quat& orientation) {
    m_orientation = orientation;
    m_updated     = true;
    return *this;
}

Transform& Transform::setOrientation(const Vec3<f32>& euler) {
    return setOrientation(Quat{ euler });
}

const Mat4<f32>& Transform::getLocal() {
    if (m_updated) {
        calculateModelMatrix();
        m_updated = false;
    }
    return m_model;
}

Mat4<f32> Transform::getWorld() {
    auto model = getLocal();
    if (m_parent) model = m_parent->getWorld() * model;
    return model;
}

void Transform::setAsDirty() { m_updated = true; }

void Transform::calculateModelMatrix() {
    m_model =
      math::translate(identityMatrix, m_position) * math::mat4_cast(m_orientation)
      * math::scale(identityMatrix, m_scale);
}

}  // namespace sl
