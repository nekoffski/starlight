#pragma once

#include "sl/geom/ModelLoaderImpl.h"

#include <gmock/gmock.h>

using namespace sl::geom;

struct ModelLoaderImplMock : public ModelLoaderImpl {
    struct Factory : public ModelLoaderImpl::Factory {
        MOCK_METHOD(std::unique_ptr<ModelLoaderImpl>, create, (), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(std::shared_ptr<Model>, loadModel, (const std::string&), (override));
};
