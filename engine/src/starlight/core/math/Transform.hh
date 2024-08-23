#pragma once

#include "Core.hh"

namespace sl {

class Transform {
public:
    Transform();

    explicit Transform(
      const Vec3<f32>& position, const Vec3<f32>& scale = Vec3<f32>{ 1.0f },
      const Mat4<f32>& rotation = identityMatrix
    );

    void setParent(Transform* parent);
    Transform* getParent() const;

    static Transform fromScale(const Vec3<f32>& scale);
    static Transform fromPosition(const Vec3<f32>& position);
    static Transform fromRotation(const Mat4<f32>& rotation);
    static Transform fromRotation(const Vec3<f32>& axis, const float angle);

    Vec3<f32> getPosition() const;
    Vec3<f32> getScale() const;
    Mat4<f32> getRotation() const;

    Transform& scale(float scale);
    Transform& scale(const Vec3<f32>& scale);
    Transform& rotate(const Mat4<f32>& rotation);
    Transform& rotate(const Vec3<f32>& axis, const float angle);
    Transform& translate(const Vec3<f32>& position);

    Transform& setPosition(const Vec3<f32>& position);
    Transform& setScale(const Vec3<f32>& scale);
    Transform& setRotation(const Mat4<f32>& rotation);

    Mat4<f32> getModel();
    Mat4<f32> getWorld();

    void setAsDirty();

private:
    void calculateModelMatrix();

    Mat4<f32> m_model;

    Vec3<f32> m_position;
    Vec3<f32> m_scale;
    Mat4<f32> m_rotation;

    bool m_updated;
    Transform* m_parent;
};

}  // namespace sl