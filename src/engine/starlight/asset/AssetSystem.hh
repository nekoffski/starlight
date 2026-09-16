#pragma once

#include "Shader.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/renderer/RendererProxy.hh"
#include "starlight/renderer/rhi/Shader.hh"
#include "starlight/runtime/EventLoop.hh"

namespace sl {

class AssetSystem : public NonCopyable, public NonMovable {
   public:
    explicit AssetSystem(const Config& cfg, RendererProxy proxy);

    Result<ShaderRef> loadShader(const Path& path);

   private:
    Config m_cfg;
    RendererProxy m_proxy;
    EventLoop m_eventLoop{"AssetSystemEventLoop"};
};

}  // namespace sl
