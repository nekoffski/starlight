/*#include "RendererProxy.h"*/

//#include "sl/asset/AssetManager.hpp"
//#include "sl/rendering/renderer/CubemapRenderer.h"
//#include "sl/rendering/renderer/ModelRenderer.h"
//#include "sl/rendering/renderer/ParticleRenderer.h"

//namespace sl::rendering {

//RendererProxy::RendererProxy(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer)
    //: m_lowLevelRenderer(lowLevelRenderer)
    //, m_cubemapRenderer(std::make_shared<renderer::CubemapRenderer>(lowLevelRenderer))
    //, m_modelRenderer(std::make_shared<renderer::ModelRenderer>(lowLevelRenderer))
    //, m_particleRenderer(std::make_shared<renderer::ParticleRenderer>(lowLevelRenderer))
    //, m_shadowMapFrameBuffer(platform::gpu::FrameBuffer::create())
    //, m_depthShader(asset::AssetManager::load<platform::shader::Shader>("/depth_capture.vert", "/depth_capture.frag")) {
//}

//void RendererProxy::renderCubemap(std::shared_ptr<platform::texture::Cubemap> cubemap,
    //std::shared_ptr<platform::shader::Shader> cubemapShader,
    //std::shared_ptr<rendering::camera::Camera> camera) {
    //m_cubemapRenderer->render(cubemap, cubemapShader, camera);
//}

//void RendererProxy::renderModel(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
    //const math::Mat4& transform) {
    //m_modelRenderer->render(shader, modelData, transform);
//}

//void RendererProxy::beginParticleEffect(std::shared_ptr<platform::shader::Shader> shader) {
    //m_particleRenderer->begin(shader);
//}

//void RendererProxy::renderParticle() {
    //m_particleRenderer->render();
//}

//void RendererProxy::endParticleEffect() {
    //m_particleRenderer->end();
//}

//std::shared_ptr<RendererProxy> RendererProxy::create(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer) {
    //return std::make_shared<RendererProxy>(lowLevelRenderer);
//}
/*}*/
