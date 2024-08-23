#include "RenderGraph.hh"

namespace sl {

RenderGraph::Builder::Builder(
  RendererBackend& renderer, const Vec2<u32>& viewportSize
) :
    m_renderer(renderer),
    m_viewportSize(viewportSize), m_renderGraph(createOwningPtr<RenderGraph>()) {}

OwningPtr<RenderGraph> RenderGraph::Builder::build() && {
    auto backendProxy  = m_renderer.getProxy();
    auto resourcePools = m_renderer.getResourcePools();
    auto& views        = m_renderGraph->getViews();

    const auto viewCount = views.size();

    RenderView::InitProperties initProperties{ .viewportSize = m_viewportSize };

    for (u32 i = 0; i < viewCount; ++i) {
        initProperties.hasPreviousView = (i != 0);
        initProperties.hasNextView     = (i != viewCount - 1);
        views[i]->init(*backendProxy, *resourcePools, initProperties);
    }

    return std::move(m_renderGraph);
}

RenderGraph::Views& RenderGraph::getViews() { return m_views; }

const RenderGraph::Views& RenderGraph::getViews() const { return m_views; }

}  // namespace sl
