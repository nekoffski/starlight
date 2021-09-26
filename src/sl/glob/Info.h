#pragma once

#include <string>

namespace sl::glob {

struct Info {
    std::string gpuApiVendor;
    std::string gpuApiRelease;
    std::string rendererName;
    std::string shadingLanguageVersion;
};

}