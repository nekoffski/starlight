#include "RenderView.hh"

namespace sl {

RenderView::RenderView(Camera* camera) : m_renderPass(nullptr), m_camera(camera) {}

void RenderView::setCamera(Camera* camera) { m_camera = camera; }

}  // namespace sl