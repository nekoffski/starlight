#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "starlight/core/math/Core.hh"
#include <kstd/Id.hh>

#include "fwd.hh"
#include "starlight/renderer/gpu/Shader.hh"
#include "gpu/ShaderDataBinder.hh"
#include "gpu/Texture.hh"

namespace sl {

using namespace std::string_literals;

class Material
    : public kstd::NonMovable,
      public kstd::NamedResource<Material, "Material", false> {
public:
    struct Properties {
        kstd::SharedPtr<Texture> diffuseMap;
        kstd::SharedPtr<Texture> specularMap;
        kstd::SharedPtr<Texture> normalMap;
        Vec4<f32> diffuseColor;
        float shininess;
    };

    explicit Material(const Properties& props, OptStr name = {});

    bool isTransparent() const;

    kstd::SharedPtr<Texture> diffuseMap;
    kstd::SharedPtr<Texture> specularMap;
    kstd::SharedPtr<Texture> normalMap;
    float shininess;
    Vec4<f32> diffuseColor;
};

}  // namespace sl
