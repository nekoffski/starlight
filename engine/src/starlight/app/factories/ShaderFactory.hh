#pragma once

#include "starlight/core/Factory.hh"
#include "kstd/FileSystem.hh"
#include "starlight/renderer/gpu/Shader.hh"

namespace sl {

class ShaderFactory : public Factory<ShaderFactory, Shader> {
public:
    kstd::SharedPtr<Shader> load(
      const std::string& name,
      const kstd::FileSystem& fs = kstd::GlobalFileSystem::get()
    );
};

}  // namespace sl
