#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "RenderRequest.hh"
#include "starlight/renderer/shading/Material.hh"

namespace sl {

// Only the material is borrowed; it must outlive every build using this draw.
struct MaterialDraw {
    std::reference_wrapper<const Material> material;
    std::vector<PrimitiveHandle> primitives;
    ShaderParameters parameters;
};

class RenderRequestBuilder {
   public:
    explicit RenderRequestBuilder(RenderRequest request = {})
        : m_request(std::move(request)) {}

    RenderRequestBuilder& addView(RenderView view) {
        m_request.views.push_back(std::move(view));
        return *this;
    }

    RenderRequestBuilder& setClearColor(Vec4f color) {
        m_request.clearColor = color;
        return *this;
    }

    RenderRequestBuilder& addDraw(MaterialDraw draw) {
        m_draws.push_back(std::move(draw));
        return *this;
    }

    Result<RenderRequest> build() const;

   private:
    RenderRequest m_request;
    std::vector<MaterialDraw> m_draws;
};

}  // namespace sl
