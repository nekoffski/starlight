#pragma once

#include <kcore/Concepts.hh>
#include <kcore/io/FileSystem.hh>

#include "Shader.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/RendererProxy.hh"
#include "starlight/renderer/core/Shader.hh"
#include "starlight/runtime/EventLoop.hh"

namespace sl {

class AssetSystem : public kc::NonCopyable, public kc::NonMovable {
   public:
    explicit AssetSystem(const Config& cfg, RendererProxy proxy);

    Result<ShaderRef> loadShader(const kc::Path& path);

   private:
    Config m_cfg;
    RendererProxy m_proxy;
    EventLoop m_eventLoop{"AssetSystemEventLoop"};
};

}  // namespace sl
