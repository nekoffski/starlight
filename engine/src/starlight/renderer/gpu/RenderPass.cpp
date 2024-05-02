#include "RenderPass.h"

#include "RenderTarget.h"

namespace sl {

RenderPass::RenderPass(u32 id, const Properties& props) : m_id(id), m_props(props) {}

void RenderPass::setClearColor(const Vec4<f32>& color) {
    m_props.clearColor = color;
}

void RenderPass::setRect(const Rect2u32& extent) { m_props.rect = extent; }

void RenderPass::setRectSize(const Vec2<u32>& size) { m_props.rect.size = size; }

u32 RenderPass::getId() const { return m_id; }

}  // namespace sl
