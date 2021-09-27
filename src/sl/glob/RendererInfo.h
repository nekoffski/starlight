#pragma once

#include <string>

namespace sl::glob {

struct RendererInfo {
    std::string gpuApiVendor;
    std::string gpuApiRelease;
    std::string rendererName;
    std::string shadingLanguageVersion;
};

}