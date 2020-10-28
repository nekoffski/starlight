/*#pragma once*/

//#include "data/ModelData.h"
//#include "renderer/fwd.h"
//#include "renderer/lowlevel/LowLevelRenderer.h"
//#include "sl/math/Matrix.hpp"
//#include "sl/platform/fwd.h"
//#include "sl/platform/gpu/FrameBuffer.h"
//#include "sl/platform/texture/Texture.h"

//#include "sl/rendering/renderer/ParticleRenderer.h"

//namespace sl::rendering {

//class RendererProxy {
//public:
    //explicit RendererProxy(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer);

    //void renderCubemap(std::shared_ptr<platform::texture::Cubemap> cubemap,
        //std::shared_ptr<platform::shader::Shader> cubemapShader,
        //std::shared_ptr<rendering::camera::Camera> camera);

    //void renderModel(std::shared_ptr<platform::shader::Shader> shader, const data::ModelData& modelData,
        //const math::Mat4& transform);

    //void beginParticleEffect(std::shared_ptr<platform::shader::Shader> shader);
    //void renderParticle();
    //void endParticleEffect();

    //void setShadowMap(std::shared_ptr<platform::texture::Texture> shadowMap) {
        //m_shadowMapFrameBuffer->bindTexture(shadowMap);
    //}

    //void setTemporaryViewport(unsigned int w, unsigned int h) {	
		//m_lowLevelRenderer.setTemporaryViewport(w, h);		
	//}

    //void restoreViewport() {
		//m_lowLevelRenderer.restoreViewport();
	//}

    //void beginDepthCapture() {
		//m_lowLevelRenderer.setTemporaryViewport(1024, 1024);
		//m_shadowMapFrameBuffer->bind();
		//m_lowLevelRenderer.clearBuffers(STARL_DEPTH_BUFFER_BIT);
		//m_depthShader->enable();
	//}

    //void endDepthCapture() {
		//m_shadowMapFrameBuffer->unbind();
		//m_depthShader->disable();
		//m_lowLevelRenderer.restoreViewport();
		//m_lowLevelRenderer.clearBuffers(STARL_COLOR_BUFFER_BIT | STARL_DEPTH_BUFFER_BIT);

	//}
	
	//std::shared_ptr<platform::shader::Shader> getDepthShader() {
		//return m_depthShader;
	//}

    //static std::shared_ptr<RendererProxy> create(renderer::lowlevel::LowLevelRenderer& lowLevelRenderer);

//private:
    //renderer::lowlevel::LowLevelRenderer& m_lowLevelRenderer;
    //std::shared_ptr<renderer::CubemapRenderer> m_cubemapRenderer;
    //std::shared_ptr<renderer::ModelRenderer> m_modelRenderer;
    //std::shared_ptr<renderer::ParticleRenderer> m_particleRenderer;

    //std::shared_ptr<platform::gpu::FrameBuffer> m_shadowMapFrameBuffer;
	//std::shared_ptr<platform::shader::Shader> m_depthShader;
//};
/*}*/
