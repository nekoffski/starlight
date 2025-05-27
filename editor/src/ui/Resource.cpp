#include "Resource.hh"

#include <string_view>
#include <kstd/String.hh>

#include <starlight/ui/fonts/FontAwesome.hh>
#include <starlight/core/Globals.hh>

namespace sle {

static Resource::Type extensionToResourceType(const std::string& extension) {
    if (extension == ".tga" || extension == ".jpg" || extension == ".png")
        return Resource::Type::texture;
    else if (extension == ".obj")
        return Resource::Type::wavefrontObject;
    else if (extension == ".mtl")
        return Resource::Type::wavefrontMaterial;
    else if (extension == ".vert" || extension == ".spv" || extension == ".frag")
        return Resource::Type::shader;
    else if (extension == ".starmtl")
        return Resource::Type::material;
    return Resource::Type::unknown;
}

static std::string_view getResourceThumbnail(Resource::Type type) {
    switch (type) {
        case Resource::Type::directory:
            return ICON_FA_FOLDER;
        case Resource::Type::texture:
            return ICON_FA_IMAGE;
        case Resource::Type::shader:
            return ICON_FA_LIGHTBULB;
        case Resource::Type::material:
        case Resource::Type::wavefrontMaterial:
            return ICON_FA_FEATHER;
        case Resource::Type::wavefrontObject:
            return ICON_FA_CUBE;
        case Resource::Type::unknown:
        default:
            return ICON_FA_FILE;
    }
}

Resource::Resource(const std::string& fullPath, const std::string& assetPath)
    : fullPath(fullPath) {
    name = kstd::nameFromPath(
      fullPath, kstd::NameExtractionMode::withoutLastExtensionChunk
    );
    extension =
      kstd::extensionFromPath(fullPath, kstd::ExtensionExtractionMode::lastChunk)
        .value_or("");
    type =
      kstd::isDirectory(fullPath)
        ? Resource::Type::directory
        : extensionToResourceType(extension);
    thumbnail       = getResourceThumbnail(type);
    this->assetPath = fmt::format("{}/{}", assetPath, name);
}

bool Resource::isDirectory() const { return type == Type::directory; }

}  // namespace sle
