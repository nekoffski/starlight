#pragma once

namespace sl::gui::assets {

struct AssetGuiProvider {
    virtual void render() = 0;
};

}  // namespace sl::gui::assets