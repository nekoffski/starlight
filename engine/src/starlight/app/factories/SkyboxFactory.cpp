#include "SkyboxFactory.hh"

#include "ShaderFactory.hh"
#include "TextureFactory.hh"

namespace sl {

SkyboxFactory::SkyboxFactory()
    : m_defaultSkyboxShader(ShaderFactory::get().load("Builtin.Shader.Skybox")) {}

kstd::SharedPtr<Skybox> SkyboxFactory::load(const std::string& name) {
    log::info("Loading skybox: {}", name);

    if (auto resource = find(name); resource) [[unlikely]]
        return resource;

    auto cubemap = TextureFactory::get().loadCubemap(name);
    return save(kstd::makeShared<Skybox>(cubemap, name));
}

kstd::SharedPtr<Shader> SkyboxFactory::getDefaultShader() {
    return m_defaultSkyboxShader;
}

}  // namespace sl