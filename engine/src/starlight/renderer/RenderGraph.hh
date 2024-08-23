#pragma once

#include <vector>

#include "starlight/core/memory/Memory.hh"
#include "gpu/RendererBackend.hh"
#include "views/RenderView.hh"

namespace sl {

class RenderGraph {
public:
    using Views = std::vector<OwningPtr<RenderView>>;

    class Builder {
    public:
        explicit Builder(RendererBackend& renderer, const Vec2<u32>& viewportSize);

        OwningPtr<RenderGraph> build() &&;

        template <typename T, typename... Args>
        Builder&& addView(Args&&... args) && {
            m_renderGraph->getViews().push_back(
              createOwningPtr<T>(std::forward<Args>(args)...)
            );

            return std::move(*this);
        }

    private:
        RendererBackend& m_renderer;
        const Vec2<u32>& m_viewportSize;
        OwningPtr<RenderGraph> m_renderGraph;
    };

    Views& getViews();
    const Views& getViews() const;

private:
    Views m_views;
};

}  // namespace sl
