#pragma once

#include "sl/geom/ModelLoader.h"

#include <gmock/gmock.h>

using namespace sl::geom;

struct ModelLoaderMock : ModelLoader {
    MOCK_METHOD(std::shared_ptr<Model>, load, (const std::string&));

    inline static ModelLoaderMock* instance = nullptr;
};