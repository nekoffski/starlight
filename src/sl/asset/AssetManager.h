#pragma once

#include <kc/core/Log.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "AssetContainer.h"
#include "AssetContainer2.h"
#include "sl/geom/Mesh.h"
#include "sl/geom/Model.h"
#include "sl/gpu/Cubemap.h"
#include "sl/gpu/Shader.h"

namespace sl::asset {

class AssetManager {
   public:
    void clear() {
        m_textures.clear();
        m_cubemaps.clear();
        m_meshes.clear();
        m_shaders.clear();
    }

    void add(std::unique_ptr<gpu::Cubemap> cubemap) {
        auto name = cubemap->name;
        m_cubemaps.insert(std::move(cubemap), name);
    }

    void add(std::shared_ptr<geom::Mesh> mesh) { m_meshes.insert(mesh, mesh->name); }

    void add(std::unique_ptr<gpu::Texture> texture) {
        auto name = texture->name;
        m_textures.insert(std::move(texture), name);
    }

    void add(std::vector<std::shared_ptr<geom::Mesh>> meshes) {
        for (auto& mesh : meshes) add(mesh);
    }

    AssetContainer<gpu::Shader>& getShaders() { return m_shaders; }

    AssetContainer2<gpu::Cubemap>& getCubemaps() { return m_cubemaps; }

    AssetContainer<geom::Mesh>& getMeshes() { return m_meshes; }

    AssetContainer2<gpu::Texture>& getTextures() { return m_textures; }

   private:
    AssetContainer2<gpu::Texture> m_textures;
    AssetContainer2<gpu::Cubemap> m_cubemaps;
    AssetContainer<geom::Mesh> m_meshes;
    AssetContainer<gpu::Shader> m_shaders;
};

}  // namespace sl::asset
