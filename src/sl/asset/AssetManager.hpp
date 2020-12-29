#pragma once

#include <memory>

#include "sl/asset/loaders/CubemapLoader.hpp"
#include "sl/asset/loaders/ModelLoaderWrapper.hpp"
#include "sl/asset/loaders/ShaderLoader.hpp"
#include "sl/asset/loaders/TextureLoader.hpp"
#include "sl/async/AsyncEngine.hpp"
#include "sl/core/error/ErrorCode.h"
#include "sl/core/error/Errors.hpp"

namespace sl::asset {

class AssetManager {
public:
    template <typename T, typename... Args>
    static std::shared_ptr<T> loadGlobalPath(Args&&... args) {
        return load<T>(true, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    static std::shared_ptr<T> loadLocalPath(Args&&... args) {
        return load<T>(false, std::forward<Args>(args)...);
    }

private:
    template <typename T, typename... Args>
    static std::shared_ptr<T> load(bool globalPath, Args&&... args) {
        if (loaders::IsSpecializedFor<loaders::AssetLoader<T>>::value)
            return loaders::AssetLoader<T>::load(globalPath,
                loaders::AssetLoaderArgs<T>{ std::forward<Args>(args)... });
        throw core::error::AssetError{ core::error::ErrorCode::CouldNotFindAssetLoader };
    }
};
}
