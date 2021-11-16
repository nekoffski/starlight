#include "TextureManager.h"

namespace sl::gfx {

TextureManager::TextureManager(Texture::Factory* textureFactory, Cubemap::Factory* cubemapFactory, Image::Factory* imageFactory)
    : m_textureFactory(textureFactory)
    , m_cubemapFactory(cubemapFactory)
    , m_imageFactory(imageFactory) {
}

TextureManager::CubemapBuilder::CubemapBuilder(TextureManager* textureManager)
    : m_textureManager(textureManager) {
}

std::unique_ptr<Cubemap> TextureManager::CubemapBuilder::get() && {
    auto cubemap = std::invoke(m_buildFunction, this);
    cubemap->name = m_name;

    if (m_facesView.has_value()) {
        CubemapArgs cubemapArgs;

        std::ranges::transform(m_facesView.value(), cubemapArgs.begin(), [](auto& face) -> std::string {
            return face->getPath();
        });

        cubemap->m_facesPaths = std::move(cubemapArgs);
    }
    return cubemap;
}

TextureManager::CubemapBuilder&& TextureManager::CubemapBuilder::fromFaces(const CubemapFacesView& faces) && {
    m_facesView = faces;
    m_buildFunction = &CubemapBuilder::buildFromFaces;
    return std::move(*this);
}

TextureManager::CubemapBuilder&& TextureManager::CubemapBuilder::fromPaths(const CubemapArgs& args) && {
    m_cubemapArgs = args;
    m_buildFunction = &CubemapBuilder::buildFromPaths;
    return std::move(*this);
}

TextureManager::CubemapBuilder&& TextureManager::CubemapBuilder::withWidth(const int width) && {
    m_width = width;
    m_buildFunction = &CubemapBuilder::buildFromDimensions;
    return std::move(*this);
}

TextureManager::CubemapBuilder&& TextureManager::CubemapBuilder::withHeight(const int height) && {
    m_height = height;
    m_buildFunction = &CubemapBuilder::buildFromDimensions;
    return std::move(*this);
}

TextureManager::CubemapBuilder&& TextureManager::CubemapBuilder::withName(const std::string& name) && {
    m_name = name;
    return std::move(*this);
}

std::unique_ptr<Cubemap> TextureManager::CubemapBuilder::buildFromDimensions() {
    return m_textureManager->m_cubemapFactory->create(m_width.value(), m_height.value());
}

std::unique_ptr<Cubemap> TextureManager::CubemapBuilder::buildFromFaces() {
    return m_textureManager->m_cubemapFactory->create(m_facesView.value());
}

std::unique_ptr<Cubemap> TextureManager::CubemapBuilder::buildFromPaths() {
    m_faces = m_textureManager->loadCubemapFaces(m_cubemapArgs.value());
    m_facesView = m_textureManager->createCubemapFacesView(m_faces.value());

    return buildFromFaces();
}

TextureManager::CubemapFaces TextureManager::loadCubemapFaces(const CubemapArgs& facesPaths) {
    CubemapFaces faces;
    std::ranges::transform(facesPaths, faces.begin(),
        [](const auto& facePath) -> std::unique_ptr<gfx::Image> { return gfx::TextureManager::get().loadImage(facePath); });

    return faces;
}

TextureManager::CubemapFacesView TextureManager::createCubemapFacesView(const CubemapFaces& cubemapFaces) {
    CubemapFacesView facesView;
    std::ranges::transform(cubemapFaces, facesView.begin(),
        [](const auto& face) -> gfx::Image* { return face.get(); });

    return facesView;
}
TextureManager::TextureBuilder::TextureBuilder(TextureManager* textureManager)
    : m_textureManager(textureManager) {
}

std::unique_ptr<Texture> TextureManager::TextureBuilder::get() && {
    auto texture = std::invoke(m_buildFunction, this);
    texture->name = m_name;

    if (m_imageView != nullptr)
        texture->path = m_imageView->getPath();

    return texture;
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::fromImage(Image* imageView) && {
    m_buildFunction = &TextureBuilder::buildFromImage;
    m_imageView = imageView;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::fromPath(const std::string& path) && {
    m_buildFunction = &TextureBuilder::buildFromPath;
    m_path = path;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::withWidth(const int width) && {
    m_buildFunction = &TextureBuilder::buildFromDimension;
    m_width = width;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::withHeight(const int height) && {
    m_buildFunction = &TextureBuilder::buildFromDimension;
    m_height = height;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::withName(const std::string& name) && {
    m_name = name;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::withColorComponents(int colorComponents) && {
    m_colorComponents = colorComponents;
    return std::move(*this);
}

TextureManager::TextureBuilder&& TextureManager::TextureBuilder::withFormat(int format) && {
    m_format = format;
    return std::move(*this);
}

std::unique_ptr<Texture> TextureManager::TextureBuilder::buildFromImage() {
    return m_textureManager->m_textureFactory->create(*m_imageView);
}

std::unique_ptr<Texture> TextureManager::TextureBuilder::buildFromPath() {
    m_image = m_textureManager->loadImage(m_path);
    m_imageView = m_image.get();

    return buildFromImage();
}

std::unique_ptr<Texture> TextureManager::TextureBuilder::buildFromDimension() {
    return m_textureManager->m_textureFactory->create(
        m_width.value(), m_height.value(), m_colorComponents, m_format);
}

std::unique_ptr<Cubemap> TextureManager::loadCubemap(const CubemapFaces& faces) {
    std::array<gfx::Image*, facesCount> facesView;
    std::ranges::transform(faces, facesView.begin(),
        [](const auto& face) -> gfx::Image* { return face.get(); });

    return m_cubemapFactory->create(facesView);
}

std::unique_ptr<Image> TextureManager::loadImage(const std::string& path, int desiredChannels) {
    auto image = m_imageFactory->create(path, desiredChannels);
    image->m_path = path;

    return image;
}

TextureManager::CubemapBuilder TextureManager::createCubemap() {
    return CubemapBuilder { this };
}

std::unique_ptr<Cubemap> TextureManager::createOmniShadowMap() {
    static const auto shadowMapSize = Texture::shadowMapSize;

    return createCubemap().withWidth(shadowMapSize).withHeight(shadowMapSize).get();
}

TextureManager::TextureBuilder TextureManager::createTexture() {
    return TextureBuilder { this };
}

std::unique_ptr<Texture> TextureManager::createShadowMap() {
    static const auto shadowMapSize = Texture::shadowMapSize;

    return createTexture()
        .withHeight(shadowMapSize)
        .withWidth(shadowMapSize)
        .withColorComponents(STARL_DEPTH_COMPONENT)
        .withFormat(STARL_DEPTH_COMPONENT)
        .get();
}

}