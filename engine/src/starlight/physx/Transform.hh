#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/math/Core.hh"

namespace sl {

class Transform {
public:
    Transform();

    explicit Transform(
      const Vec3<f32>& position, const Vec3<f32>& scale = Vec3<f32>{ 1.0f },
      const Quat& orientation = Quat{ 0.0f, 0.0f, 0.0f, 1.0f }
    );

    static Transform fromScale(const Vec3<f32>& scale);
    static Transform fromPosition(const Vec3<f32>& position);
    static Transform fromOrientation(const Quat& orientation);
    static Transform fromRotation(const Vec3<f32>& axis, const float angle);

    void setParent(Transform& parent);
    Transform* getParent() const;

    const Vec3<f32>& getPosition() const;
    const Vec3<f32>& getScale() const;
    const Quat& getOrientation() const;

    Vec3<f32> getEuler() const;

    Transform& scale(float scale);
    Transform& scale(const Vec3<f32>& scale);
    Transform& rotate(const Quat& rotation);
    Transform& rotate(const Vec3<f32>& axis, const float angle);
    Transform& translate(const Vec3<f32>& position);

    Transform& setPosition(const Vec3<f32>& position);
    Transform& setScale(const Vec3<f32>& scale);
    Transform& setOrientation(const Quat& orientation);
    Transform& setOrientation(const Vec3<f32>& euler);

    const Mat4<f32>& getLocal();
    Mat4<f32> getWorld();

    void setAsDirty();

private:
    void calculateModelMatrix();

    Mat4<f32> m_model;

    Vec3<f32> m_position;
    Vec3<f32> m_scale;
    Quat m_orientation;

    bool m_updated;
    Transform* m_parent;
};

class Transformable : public kstd::NonCopyable {
public:
    explicit Transformable();
    explicit Transformable(Transform& transform);

    const Mat4<f32>& getLocal() const;
    Mat4<f32> getWorld() const;

    void resetTransform();
    void setTransform(Transform& transform);

private:
    Transform* m_transform;
    inline static Transform s_emptyTransform;
};

}  // namespace sl
