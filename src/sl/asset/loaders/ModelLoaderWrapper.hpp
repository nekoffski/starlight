#pragma once

#include "sl/asset/loaders/AssetLoader.h"

#include "sl/core/log/Logger.h"
#include "sl/core/path/PathManager.hpp"
#include "sl/geometry/Model.h"
#include "sl/platform/model/ModelLoader.h"

namespace sl::asset::loaders {

using geometry::Model;
using platform::model::ModelLoader;

template <>
struct AssetLoaderArgs<Model> {
    std::string path;
};

template <>
class AssetLoader<Model> {
public:
    static std::shared_ptr<Model> load(bool globalPath, AssetLoaderArgs<Model> args) {
        std::string path = "";
        if (not globalPath)
            path += core::path::PathManager::get<Model>();

        path += args.path;
        SL_DEBUG("loading model: {}", path);
        return m_modelLoader->loadModel(path);
    }

private:
    inline static auto m_modelLoader{ ModelLoader::create() };
};
}
