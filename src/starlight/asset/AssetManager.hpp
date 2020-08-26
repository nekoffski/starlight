#pragma once

#include <memory>

#include "starlight/asset/Error.h"
#include "starlight/asset/loaders/CubemapLoader.hpp"
#include "starlight/asset/loaders/ModelLoaderWrapper.hpp"
#include "starlight/asset/loaders/ShaderLoader.hpp"
#include "starlight/asset/loaders/TextureLoader.hpp"

namespace sl::asset {

class AssetManager {
public:
    template <typename T, typename... Args>
    static std::shared_ptr<T> load(Args&&... args) {
        if (loaders::IsSpecializedFor<loaders::AssetLoader<T>>::value)
            return loaders::AssetLoader<T>::load(
                loaders::AssetLoaderArgs<T>{ std::forward<Args>(args)... });
        throw AssetException(ErrorCode::LOADER_NOT_FOUND);
    }
};
}
