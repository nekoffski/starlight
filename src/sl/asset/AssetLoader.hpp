#pragma once

#include <memory>

#include "sl/asset/loaders/CubemapLoader.hpp"
#include "sl/asset/loaders/ModelLoaderWrapper.hpp"
#include "sl/asset/loaders/ShaderLoader.hpp"
#include "sl/asset/loaders/TextureLoader.hpp"
#include "sl/async/AsyncEngine.hpp"
#include "sl/core/ErrorCode.h"
#include "sl/core/Errors.hpp"

namespace sl::asset {

class AssetLoader {
public:
    template <class T, typename... Args>
    static std::shared_ptr<T> loadGlobalPath(Args&&... args) {
        return load<T>(true, std::forward<Args>(args)...);
    }

    template <class T, typename... Args>
    static std::shared_ptr<T> loadLocalPath(Args&&... args) {
        return load<T>(false, std::forward<Args>(args)...);
    }

private:
    template <class T, typename... Args>
    static std::shared_ptr<T> load(bool globalPath, Args&&... args) {
        try {
            if (loaders::IsSpecializedFor<loaders::Loader<T>>::value)
                return loaders::Loader<T>::load(globalPath,
                    loaders::LoaderArgs<T>{ std::forward<Args>(args)... });
        } catch (core::Error& e) {
            throw e.as<core::AssetError>();
        }

        throw core::AssetError{ core::ErrorCode::CouldNotFindAssetLoader };
    }
};
}
