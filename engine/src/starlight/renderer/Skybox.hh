#pragma once

#include "gpu/Texture.hh"
#include "Mesh.hh"
#include "starlight/renderer/gpu/Shader.hh"

namespace sl {

class Skybox
    : public kstd::NonMovable,
      public kstd::NamedResource<Skybox, "Skybox"> {
public:
    explicit Skybox(kstd::SharedPtr<Texture> cubeMap, OptStr name = {});

    Texture* getCubeMap();

private:
    kstd::SharedPtr<Texture> m_cubeMap;
};

}  // namespace sl
