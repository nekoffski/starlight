
#pragma once
#include <kc/core/Log.h>

#include <algorithm>
#include <kc/core/Singleton.hpp>

#include "Cubemap.h"
#include "Image.h"
#include "Texture.h"
#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Texture.h"

namespace sl::gfx {

class TextureManager : public kc::core::Singleton<TextureManager> {
    using CubemapFaces     = std::array<std::unique_ptr<gfx::Image>, facesCount>;
    using CubemapFacesView = std::array<gfx::Image*, facesCount>;

    friend class CubemapBuilder;
    friend class TextureBuilder;

    template <typename Callback> friend class LoadCubemapAsync;

    template <typename Callback> friend class LoadTextureAsync;

    template <typename Callback> class LoadCubemapAsync : public async::AsyncTask {
       public:
        explicit LoadCubemapAsync(
            TextureManager* textureManager, const CubemapArgs& paths, const std::string& name,
            Callback&& callback
        )
            : m_textureManager(textureManager)
            , m_paths(paths)
            , m_name(name)
            , m_callback(std::forward<Callback>(callback)) {}

        void executeAsync() override { m_faces = m_textureManager->loadCubemapFaces(m_paths); }

        void finalize() override {
            auto cubemap = m_textureManager->m_cubemapFactory->create(
                m_textureManager->createCubemapFacesView(m_faces)
            );

            cubemap->name         = m_name;
            cubemap->m_facesPaths = m_paths;

            std::invoke(m_callback, std::move(cubemap));
        }

        std::string asString() const override { return fmt::format("AsyncLoadCubemap"); }

       private:
        CubemapArgs m_paths;
        std::string m_name;

        Callback m_callback;

        std::array<std::unique_ptr<gfx::Image>, facesCount> m_faces;

        TextureManager* m_textureManager;
    };

    template <typename Callback> class LoadTextureAsync : public async::AsyncTask {
       public:
        explicit LoadTextureAsync(
            TextureManager* textureManager, const std::string& path, const std::string& name,
            Callback&& callback
        )
            : m_textureManager(textureManager)
            , m_path(path)
            , m_name(name)
            , m_callback(std::forward<Callback>(callback)) {}

        void executeAsync() override { m_image = TextureManager::get().loadImage(m_path); }

        std::string asString() const override { return fmt::format("AsyncLoadTexture_{}", m_path); }

        void finalize() override {
            if (not m_image) return;

            auto texture = m_textureManager->m_textureFactory->create(*m_image);

            texture->path = m_path;
            texture->name = m_name;

            std::invoke(m_callback, std::move(texture));
        }

       private:
        TextureManager* m_textureManager;

        std::string m_path;
        std::string m_name;
        std::unique_ptr<Image> m_image;

        Callback m_callback;
    };

    class CubemapBuilder {
        typedef std::unique_ptr<Cubemap> (CubemapBuilder::*BuildFunction)();

       public:
        explicit CubemapBuilder(TextureManager* textureManager);

        template <typename Callback> void getAsync(Callback&& callback) {
            ASSERT(
                m_cubemapArgs.has_value(), "Could not load cubemap async without args specified"
            );

            async::AsyncManager::get().callAsync<LoadCubemapAsync<Callback>>(
                m_textureManager, m_cubemapArgs.value(), m_name, std::forward<Callback>(callback)
            );
        }

        std::unique_ptr<Cubemap> get() &&;

        CubemapBuilder&& fromFaces(const CubemapFacesView& faces) &&;
        CubemapBuilder&& fromPaths(const CubemapArgs& args) &&;
        CubemapBuilder&& withWidth(const int width) &&;
        CubemapBuilder&& withHeight(const int height) &&;
        CubemapBuilder&& withName(const std::string& name) &&;

       private:
        std::unique_ptr<Cubemap> buildFromDimensions();
        std::unique_ptr<Cubemap> buildFromFaces();
        std::unique_ptr<Cubemap> buildFromPaths();

        TextureManager* m_textureManager;

        std::optional<CubemapFacesView> m_facesView;
        std::optional<CubemapFaces> m_faces;
        std::optional<CubemapArgs> m_cubemapArgs;
        std::optional<int> m_width;
        std::optional<int> m_height;

        std::string m_name;

        BuildFunction m_buildFunction;
    };

    class TextureBuilder {
        typedef std::unique_ptr<Texture> (TextureBuilder::*BuildFunction)();

       public:
        explicit TextureBuilder(TextureManager* textureManager);
        std::unique_ptr<Texture> get() &&;

        template <typename Callback> void getAsync(Callback&& callback) {
            ASSERT(not m_path.empty(), "Could not load texture async without path specified");

            async::AsyncManager::get().callAsync<LoadTextureAsync<Callback>>(
                m_textureManager, m_path, m_name, std::forward<Callback>(callback)
            );
        }

        TextureBuilder&& fromImage(Image* imageView) &&;
        TextureBuilder&& fromPath(const std::string& path) &&;
        TextureBuilder&& withWidth(const int width) &&;
        TextureBuilder&& withHeight(const int height) &&;
        TextureBuilder&& withName(const std::string& name) &&;
        TextureBuilder&& withColorComponents(int colorComponents) &&;
        TextureBuilder&& withFormat(int format) &&;

       private:
        std::unique_ptr<Texture> buildFromImage();
        std::unique_ptr<Texture> buildFromPath();
        std::unique_ptr<Texture> buildFromDimension();

        TextureManager* m_textureManager;

        std::unique_ptr<gfx::Image> m_image;
        Image* m_imageView = nullptr;

        std::string m_path;
        std::string m_name;

        std::optional<int> m_width;
        std::optional<int> m_height;

        BuildFunction m_buildFunction;

        int m_colorComponents = STARL_DEPTH_COMPONENT;
        int m_format          = STARL_DEPTH_COMPONENT;
    };

   public:
    explicit TextureManager(
        Texture::Factory* textureFactory, Cubemap::Factory* cubemapFactory,
        Image::Factory* imageFactory
    );

    std::unique_ptr<Image> loadImage(const std::string& path, int desiredChannels = 0);

    CubemapBuilder createCubemap();
    TextureBuilder createTexture();

    std::unique_ptr<Cubemap> createOmniShadowMap();
    std::unique_ptr<Texture> createShadowMap();

   private:
    CubemapFaces loadCubemapFaces(const CubemapArgs& args);
    CubemapFacesView createCubemapFacesView(const CubemapFaces& cubemapFaces);
    std::unique_ptr<Cubemap> loadCubemap(const CubemapFaces& faces);

    Texture::Factory* m_textureFactory;
    Cubemap::Factory* m_cubemapFactory;
    Image::Factory* m_imageFactory;
};

}  // namespace sl::gfx