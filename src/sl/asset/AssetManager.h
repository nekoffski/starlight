#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "AssetContainer.h"
#include "sl/core/Logger.h"
#include "sl/geom/Mesh.h"
#include "sl/geom/Model.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Shader.h"

namespace sl::asset {

class AssetManager {
public:
    template <typename T>
    class Output : public core::Output<T> {
    public:
        explicit Output(AssetManager& assetManager)
            : m_assetManager(assetManager) { }

        void set(std::shared_ptr<T>& result) override {
            m_assetManager.add(result);
        }

    private:
        AssetManager& m_assetManager;
    };

    void clear() {
        m_textures.clear();
        m_cubemaps.clear();
        m_meshes.clear();
        m_shaders.clear();
    }

    void add(std::shared_ptr<gfx::Cubemap> cubemap, const std::string& name) {
        cubemap->name = name;
        m_cubemaps.insert(cubemap, name);
    }

    void add(std::shared_ptr<geom::Mesh> mesh) {
        m_meshes.insert(mesh, mesh->name);
    }

    void add(std::shared_ptr<gfx::Texture> texture) {
        m_textures.insert(texture, texture->name);
    }

    void add(std::vector<std::shared_ptr<geom::Mesh>> meshes) {
        for (auto& mesh : meshes)
            add(mesh);
    }

    AssetContainer<gfx::Shader>& getShaders() {
        return m_shaders;
    }

    AssetContainer<gfx::Cubemap>& getCubemaps() {
        return m_cubemaps;
    }

    AssetContainer<geom::Mesh>& getMeshes() {
        return m_meshes;
    }

    AssetContainer<gfx::Texture>& getTextures() {
        return m_textures;
    }

private:
    AssetContainer<gfx::Texture> m_textures;
    AssetContainer<gfx::Cubemap> m_cubemaps;
    AssetContainer<geom::Mesh> m_meshes;
    AssetContainer<gfx::Shader> m_shaders;
};

}
