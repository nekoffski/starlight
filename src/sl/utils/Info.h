#pragma once

#include <string>

namespace sl::utils {

struct Info {
    std::string gpuApiVendor;
    std::string gpuApiRelease;
    std::string rendererName;
    std::string shadingLanguageVersion;
};

}