#include "RenderPass.h"

#include "RenderTarget.h"

namespace sl {

RenderPass::RenderPass(u32 id, const Properties& props) : m_id(id), m_props(props) {}

void RenderPass::setClearColor(const Vec4f& color) { m_props.clearColor = color; }

void RenderPass::setArea(const Vec4f& area) { m_props.area = area; }

void RenderPass::setAreaSize(u32 w, u32 h) {
    m_props.area.z = w;
    m_props.area.w = h;
}

u32 RenderPass::getId() const { return m_id; }

}  // namespace sl
