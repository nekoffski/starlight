#pragma once

#include <span>

#include "starlight/ui/Core.hh"
#include "starlight/core/Log.hh"
#include "Text.hh"

namespace sl {

template <typename T> class Combo {
    struct Record {
        std::string label;
        T value;
    };

public:
    explicit Combo(std::string_view name)
        : m_name(name)
        , m_label(fmt::format("##{}-combo", name))
        , m_selected(0u) {}

    Combo& addOption(const std::string& name, const T& value) {
        m_records.emplace_back(name, value);
        return *this;
    }

    template <typename C> void render(C&& callback) {
        log::expect(m_records.size() > 0, "Empty Combo is not allowed");

        text("{}: ", m_name);
        if (ImGui::BeginCombo(
              m_label.c_str(), m_records[m_selected].label.c_str()
            )) {
            for (u64 i = 0u; i < m_records.size(); ++i) {
                auto& [label, value] = m_records[i];
                bool selected        = i == m_selected;
                if (ImGui::Selectable(label.c_str(), selected)) {
                    callback(value);
                    m_selected = i;
                }

                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

private:
    std::vector<Record> m_records;
    std::string m_name;
    std::string m_label;
    u64 m_selected;
};

template <typename T, typename C>
requires Callable<C, void, T&>
void immediateCombo(
  const std::string& name, const std::string& preview, std::span<T> container,
  C&& onSelect
) {
    if (ImGui::BeginCombo(
          fmt::format("##{}-combo", name).c_str(), preview.c_str()
        )) {
        for (auto& option : container) {
            bool selected = option->name == preview;
            if (ImGui::Selectable(option->name.c_str(), selected)) onSelect(option);
            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

}  // namespace sl
