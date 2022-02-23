#include "GuiHelper.h"

#include <kc/core/Log.h>

namespace sl::gui {

GuiHelper::FontWrapper::FontWrapper(const int size, MergedFontsRanges& parentMergedFontsRanges)
    : m_size(size), m_parentMergedFontsRanges(parentMergedFontsRanges) {}

GuiHelper::FontWrapper&& GuiHelper::FontWrapper::mergeWith(const std::string& path,
                                                           const unsigned short minCode,
                                                           const unsigned short maxCode) && {
    m_parentMergedFontsRanges.push_back({minCode, maxCode, 0});

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 11.0f;
    config.GlyphMaxAdvanceX = 15.0f;

    ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), m_size, &config,
                                             m_parentMergedFontsRanges.back().data());

    return std::move(*this);
}

GuiHelper::FontWrapper GuiHelper::addFont(const std::string& name, const std::string& path,
                                          const int size) {
    ASSERT(not m_fonts.contains(name), "Font with name " + name + " is already loaded");

    m_fonts[name] = Font{.handle = ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size)};

    return FontWrapper{size, m_fonts.at(name).mergedFontsRanges};
}

ImFont* GuiHelper::getFontHandle(const std::string& name) const {
    ASSERT(m_fonts.contains(name), "Font with name " + name + " not found");

    return m_fonts.at(name).handle;
}
}  // namespace sl::gui