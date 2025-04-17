#pragma once

#include "starlight/core/Factory.hh"
#include "starlight/renderer/Skybox.hh"

namespace sl {

class SkyboxFactory : public Factory<SkyboxFactory, Skybox> {
public:
    explicit SkyboxFactory();

    kstd::SharedPtr<Skybox> load(const std::string& name);
    kstd::SharedPtr<Shader> getDefaultShader();

private:
    kstd::SharedPtr<Shader> m_defaultSkyboxShader;
};

}  // namespace sl
