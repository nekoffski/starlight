#pragma once

#include "Glm.h"

namespace sl {

class Transform {
public:
    Transform();

    explicit Transform(
      const Vec3f& position, const Vec3f& scale = Vec3f{ 1.0f },
      const Mat4f& rotation = identityMatrix
    );

    void setParent(Transform* parent);
    Transform* getParent() const;

    static Transform fromScale(const Vec3f& scale);
    static Transform fromPosition(const Vec3f& position);
    static Transform fromRotation(const Mat4f& rotation);
    static Transform fromRotation(const Vec3f& axis, const float angle);

    Vec3f getPosition() const;
    Vec3f getScale() const;
    Mat4f getRotation() const;

    Transform& scale(float scale);
    Transform& scale(const Vec3f& scale);
    Transform& rotate(const Mat4f& rotation);
    Transform& rotate(const Vec3f& axis, const float angle);
    Transform& translate(const Vec3f& position);

    Transform& setPosition(const Vec3f& position);
    Transform& setScale(const Vec3f& scale);
    Transform& setRotation(const Mat4f& rotation);

    Mat4f getModel();
    Mat4f getWorld();

    void setAsDirty();

private:
    void calculateModelMatrix();

    Mat4f m_model;

    Vec3f m_position;
    Vec3f m_scale;
    Mat4f m_rotation;

    bool m_updated;
    Transform* m_parent;
};

}  // namespace sl