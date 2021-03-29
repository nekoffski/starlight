#pragma once

#include "Loader.h"

#include "sl/core/Logger.h"
#include "sl/geometry/Model.h"
#include "sl/geometry/ModelLoader.hpp"
#include "sl/utils/Globals.h"

namespace sl::asset::loaders {

using geometry::Model;

template <>
struct LoaderArgs<Model> {
    std::string path;
};

template <>
class Loader<Model> {
public:
    static std::shared_ptr<Model> load(bool globalPath, LoaderArgs<Model> args) {
        std::string path = "";
        if (not globalPath)
            path += GLOBALS().config.paths.models;

        path += args.path;
        SL_DEBUG("loading model: {}", path);
        return geometry::ModelLoader::loadModel(path);
    }
};
}
