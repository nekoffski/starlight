#pragma once

#include <kc/core/Log.h>
#include <kc/core/Singleton.hpp>

#include <algorithm>

#include "Cubemap.h"
#include "Image.h"
#include "Texture.h"

#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Texture.h"

namespace sl::gfx {

class TextureManager : public kc::core::Singleton<TextureManager> {
    using CubemapFaces = std::array<std::unique_ptr<gfx::Image>, facesCount>;
    using CubemapFacesView = std::array<gfx::Image*, facesCount>;

    friend class CubemapBuilder;
    friend class TextureBuilder;

    template <typename Callback>
    friend class LoadCubemapAsync;

    template <typename Callback>
    class LoadCubemapAsync : public async::AsyncTask {
    public:
        explicit LoadCubemapAsync(TextureManager* textureManager, const CubemapArgs& paths, const std::string& name, Callback&& callback)
            : m_textureManager(textureManager)
            , m_paths(paths)
            , m_name(name)
            , m_callback(std::forward<Callback>(callback)) {
        }

        void executeAsync() override {
            m_faces = m_textureManager->loadCubemapFaces(m_paths);
        }

        void finalize() override {
            auto cubemap = m_textureManager->m_cubemapFactory->create(
                m_textureManager->createCubemapFacesView(m_faces));

            cubemap->name = m_name;
            cubemap->m_facesPaths = m_paths;

            std::invoke(m_callback, std::move(cubemap));
        }

        std::string asString() const override {
            return fmt::format("AsyncLoadCubemap");
        }

    private:
        CubemapArgs m_paths;
        std::string m_name;

        Callback m_callback;

        std::array<std::unique_ptr<gfx::Image>, facesCount> m_faces;

        TextureManager* m_textureManager;
    };

    class TextureBuilder {
    public:
    private:
    };

    class CubemapBuilder {
        typedef std::unique_ptr<Cubemap> (CubemapBuilder::*BuildFunction)();

    public:
        explicit CubemapBuilder(TextureManager* textureManager);

        template <typename Callback>
        void getAsync(Callback&& callback) {
            ASSERT(m_cubemapArgs.has_value(), "Could not load cubemap async without args specified");

            async::AsyncManager::get()
                .callAsync<LoadCubemapAsync<Callback>>(
                    m_textureManager, m_cubemapArgs.value(), m_name, std::forward<Callback>(callback));
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

public:
    explicit TextureManager(Texture::Factory* textureFactory, Cubemap::Factory* cubemapFactory, Image::Factory* imageFactory);

    std::unique_ptr<Image> loadImage(const std::string& path, int desiredChannels = 0);

    CubemapBuilder createCubemap();

    std::unique_ptr<Cubemap> createOmniShadowMap();

    std::unique_ptr<Texture> createShadowMap();
    std::unique_ptr<Texture> createTexture(const std::string& path, const std::string& name);
    std::unique_ptr<Texture> createTexture(gfx::Image& image, const std::string& name);
    std::unique_ptr<Texture> createTexture(unsigned int width, unsigned int height,
        int internalFormat = STARL_DEPTH_COMPONENT, int format = STARL_DEPTH_COMPONENT);

private:
    CubemapFaces loadCubemapFaces(const CubemapArgs& args);
    CubemapFacesView createCubemapFacesView(const CubemapFaces& cubemapFaces);
    std::unique_ptr<Cubemap> loadCubemap(const CubemapFaces& faces);

    Texture::Factory* m_textureFactory;
    Cubemap::Factory* m_cubemapFactory;
    Image::Factory* m_imageFactory;
};

}