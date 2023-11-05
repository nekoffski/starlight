#include "Transform.h"

namespace sl {

Transform::Transform(
  const Vec3f& position, const Vec3f& scale, const Mat4f& rotation
) :
    m_model(identityMatrix),
    m_position(position), m_scale(scale), m_rotation(rotation), m_updated(false),
    m_parent(nullptr) {}

Transform* Transform::getParent() const { return m_parent; }

void Transform::setParent(Transform* parent) { m_parent = parent; }

Transform Transform::fromScale(const Vec3f& scale) {
    return Transform(Vec3f{ 0.0f }, scale, identityMatrix);
}

Transform Transform::fromPosition(const Vec3f& position) {
    return Transform(position);
}

Transform Transform::fromRotation(const Vec3f& axis, const float angle) {
    return Transform::fromRotation(glm::rotate(identityMatrix, angle, axis));
}

Transform Transform::fromRotation(const Mat4f& rotation) {
    return Transform(Vec3f{ 0.0f }, Vec3f{ 1.0f }, rotation);
}

Vec3f Transform::getPosition() const { return m_position; }

Vec3f Transform::getScale() const { return m_scale; }

Mat4f Transform::getRotation() const { return m_rotation; }

Transform& Transform::translate(const Vec3f& position) {
    m_position += position;
    m_updated = true;
    return *this;
}

Transform& Transform::rotate(const Mat4f& rotation) {
    m_rotation = rotation * m_rotation;
    m_updated  = true;
    return *this;
}

Transform& Transform::rotate(const Vec3f& axis, const float angle) {
    m_rotation = glm::rotate(m_rotation, angle, axis);
    m_updated  = true;
    return *this;
}

Transform& Transform::scale(float scale) { return this->scale(Vec3f{ scale }); }

Transform& Transform::scale(const Vec3f& scale) {
    m_scale *= scale;
    m_updated = true;
    return *this;
}

Transform& Transform::setPosition(const Vec3f& position) {
    m_position = position;
    m_updated  = true;
    return *this;
}

Transform& Transform::setScale(const Vec3f& scale) {
    m_scale   = scale;
    m_updated = true;
    return *this;
}

Transform& Transform::setRotation(const Mat4f& rotation) {
    m_rotation = rotation;
    m_updated  = true;
    return *this;
}

Mat4f Transform::getModel() {
    if (m_updated) {
        calculateModelMatrix();
        m_updated = false;
    }
    return m_model;
}

Mat4f Transform::getWorld() {
    auto model = getModel();
    if (m_parent) model = m_parent->getWorld() * model;
    return model;
}

void Transform::calculateModelMatrix() {
    m_model = glm::scale(glm::translate(m_rotation, m_position), m_scale);
}

}  // namespace sl
