#pragma once

#include <string>
#include <unordered_map>

#include <imgui/imgui.h>

#include <kc/core/Singleton.hpp>

namespace sl::gui {

class GuiHelper : public kc::core::Singleton<GuiHelper> {
    using MergedFontsRanges = std::vector<std::array<ImWchar, 3>>;

    struct Font {
        ImFont* handle;
        MergedFontsRanges mergedFontsRanges;
    };

    struct FontWrapper {
    public:
        explicit FontWrapper(const int size, MergedFontsRanges& parentMergedFontsRanges);

        FontWrapper&& mergeWith(const std::string& path,
            const unsigned short minCode, const unsigned short maxCode) &&;

    private:
        const int m_size;
        MergedFontsRanges& m_parentMergedFontsRanges;
    };

public:
    FontWrapper addFont(const std::string& name, const std::string& path, const int size);

    ImFont* getFontHandle(const std::string& name) const;

private:
    std::unordered_map<std::string, Font> m_fonts;
};

}