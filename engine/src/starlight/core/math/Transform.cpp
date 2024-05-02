#include "Transform.h"

namespace sl {

Transform::Transform() :
    Transform(Vec3<f32>{ 0.0f }, Vec3<f32>{ 1.0f }, identityMatrix) {}

Transform::Transform(
  const Vec3<f32>& position, const Vec3<f32>& scale, const Mat4<f32>& rotation
) :
    m_model(identityMatrix),
    m_position(position), m_scale(scale), m_rotation(rotation), m_updated(false),
    m_parent(nullptr) {}

Transform* Transform::getParent() const { return m_parent; }

void Transform::setParent(Transform* parent) { m_parent = parent; }

Transform Transform::fromScale(const Vec3<f32>& scale) {
    return Transform(Vec3<f32>{ 0.0f }, scale, identityMatrix);
}

Transform Transform::fromPosition(const Vec3<f32>& position) {
    return Transform(position);
}

Transform Transform::fromRotation(const Vec3<f32>& axis, const float angle) {
    return Transform::fromRotation(glm::rotate(identityMatrix, angle, axis));
}

Transform Transform::fromRotation(const Mat4<f32>& rotation) {
    return Transform(Vec3<f32>{ 0.0f }, Vec3<f32>{ 1.0f }, rotation);
}

Vec3<f32> Transform::getPosition() const { return m_position; }

Vec3<f32> Transform::getScale() const { return m_scale; }

Mat4<f32> Transform::getRotation() const { return m_rotation; }

Transform& Transform::translate(const Vec3<f32>& position) {
    m_position += position;
    m_updated = true;
    return *this;
}

Transform& Transform::rotate(const Mat4<f32>& rotation) {
    m_rotation = rotation * m_rotation;
    m_updated  = true;
    return *this;
}

Transform& Transform::rotate(const Vec3<f32>& axis, const float angle) {
    m_rotation = glm::rotate(m_rotation, angle, axis);
    m_updated  = true;
    return *this;
}

Transform& Transform::scale(float scale) { return this->scale(Vec3<f32>{ scale }); }

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

Transform& Transform::setRotation(const Mat4<f32>& rotation) {
    m_rotation = rotation;
    m_updated  = true;
    return *this;
}

Mat4<f32> Transform::getModel() {
    if (m_updated) {
        calculateModelMatrix();
        m_updated = false;
    }
    return m_model;
}

Mat4<f32> Transform::getWorld() {
    auto model = getModel();
    if (m_parent) model = m_parent->getWorld() * model;
    return model;
}

void Transform::setAsDirty() { m_updated = true; }

void Transform::calculateModelMatrix() {
    m_model =
      glm::scale(m_rotation * glm::translate(m_rotation, m_position), m_scale);
}

}  // namespace sl
