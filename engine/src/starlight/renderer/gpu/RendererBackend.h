#pragma once

#include <array>
#include <span>
#include <functional>

#include "starlight/core/math/Glm.h"
#include "starlight/core/math/Vertex2.h"
#include "starlight/core/math/Vertex3.h"

#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Shader.h"
#include "starlight/renderer/fwd.h"

#include "GeometryRenderData.h"
#include "GlobalState.h"

namespace sl {

// TODO: change to objects or something
constexpr int builtinRenderPassWorld = 1;
constexpr int builtinRenderPassUI    = 2;

struct RendererBackend {
    enum class BultinRenderPass { world, ui };

    virtual ~RendererBackend() = default;

    virtual u32 getRenderPassId(const std::string& renderPass) const = 0;

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    bool renderPass(uint8_t id, auto&& callback) {
        if (beginRenderPass(id)) {
            callback();
            return endRenderPass(id);
        }
        return false;
    }

    virtual bool beginRenderPass(uint8_t id) = 0;
    virtual bool endRenderPass(uint8_t id)   = 0;

    virtual void renderUI(std::function<void()>&&) = 0;

    virtual void drawGeometry(const GeometryRenderData& modelMatrix) = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;

    // resources
    virtual Texture* createTexture(
      const Texture::Properties& props, const void* pixels
    )                                             = 0;
    virtual void destroyTexture(Texture& texture) = 0;

    virtual Geometry* createGeometry(
      const Geometry::Properties& props, const Geometry::Data& data
    )                                                = 0;
    virtual void destroyGeometry(Geometry& geometry) = 0;

    virtual Shader* createShader(const Shader::Properties& props) = 0;
    virtual void destroyShader(Shader& shader)                    = 0;
};

}  // namespace sl
