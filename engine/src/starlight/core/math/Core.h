#pragma once

#include "starlight/core/Core.h"

#include "Glm.h"

namespace sl {

template <typename T> struct Vec2 {
    explicit Vec2(T x = 0, T y = 0) : x(x), y(y), w(this->x), h(this->y) {}
    Vec2(const Vec2& oth) : Vec2(oth.x, oth.y) {}

    Vec2& operator=(const Vec2& oth) {
        x = oth.x;
        y = oth.y;
        return *this;
    }

    T x;
    T y;

    T& w;
    T& h;
};

template <typename T> struct Vec3 {
    explicit Vec3(T x = 0, T y = 0, T z = 0) :
        x(x), y(y), z(z), w(this->x), h(this->y), d(this->z) {}
    Vec3(const Vec3& oth) : Vec3(oth.x, oth.y, oth.z) {}

    Vec3& operator=(const Vec3& oth) {
        x = oth.x;
        y = oth.y;
        z = oth.z;
        return *this;
    }

    T x;
    T y;
    T z;

    T& w;
    T& h;
    T& d;
};

template <typename T> struct Vec4 {
    explicit Vec4(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}
    Vec4(const Vec4& oth) : Vec4(oth.x, oth.y, oth.z, oth.w) {}

    Vec4& operator=(const Vec4& oth) {
        x = oth.x;
        y = oth.y;
        z = oth.z;
        w = oth.w;
        return *this;
    }

    T x;
    T y;
    T z;
    T w;
};

template <typename T> struct Rect2 {
    Vec2<T> offset;
    Vec2<T> size;
};

template <typename T> struct Rect3 {
    Vec3<T> offset;
    Vec3<T> size;
};

using Vec2u32  = Vec2<u32>;
using Vec3u32  = Vec3<u32>;
using Vec4u32  = Vec4<u32>;
using Rect2u32 = Rect2<u32>;

}  // namespace sl
