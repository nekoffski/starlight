#include "RenderView.h"

namespace sl {

RenderView::RenderView(Camera* camera) : m_camera(camera) {}

void RenderView::setCamera(Camera* camera) { m_camera = camera; }

}  // namespace sl