#pragma once

#include "sl/geometry/ModelLoaderImpl.h"

#include <gmock/gmock.h>

using namespace sl::geometry;

struct ModelLoaderImplMock : public ModelLoaderImpl {
    struct Factory : public ModelLoaderImpl::Factory {
        MOCK_METHOD(std::unique_ptr<ModelLoaderImpl>, create, (), (override));
    };

    MOCK_METHOD(std::shared_ptr<Model>, loadModel, (const std::string&), (override));
};
