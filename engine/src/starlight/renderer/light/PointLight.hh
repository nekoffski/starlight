#pragma once

#include <span>
#include <vector>

#include "starlight/math/Core.hh"

namespace sl {

class PointLight : public kstd::NonCopyable {
public:
    struct LOD {
        f32 distance;
        f32 opacity;
    };

    struct alignas(16) ShaderData {
        Vec4<f32> color;
        Vec3<f32> position;
        Vec3<f32> attenuation;
    };

    PointLight(PointLight&& oth);
    PointLight(const PointLight& oth);

    PointLight& operator=(const PointLight& oth);
    PointLight& operator=(PointLight&& oth);

    explicit PointLight(
      const Vec4<f32>& color       = Vec4<f32>{ 1.0f },
      const Vec3<f32>& position    = Vec3<f32>{ 0.0f },
      const Vec3<f32>& attenuation = Vec3<f32>{ 0.5f, 1.0f, 1.0f }
    );

    const Vec3<f32>& getAttenuation() const;
    void setAttenuation(const Vec3<f32>& attenuation);

    std::span<const LOD> getLODs() const;
    void generateLODs();

    const ShaderData& getShaderData() const;

private:
    ShaderData m_data;
    std::vector<LOD> m_lods;

public:
    Vec4<f32>& color;
    Vec3<f32>& position;
};

std::string toString(const PointLight& l);

}  // namespace sl
