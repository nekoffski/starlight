#include "Slider.h"

#include "starlight/ui/Utils.h"

namespace sl::ui {

Slider3::Slider3(
  const std::string& name, const StepInterval<float>& range,
  const Vec3f& initialValue
) :
    m_name(name),
    m_id(fmt::format("##{}", name)), m_data(initialValue), m_range(range) {}

void Slider3::render(Callback&& callback) {
    sl::ui::text(m_name);
    if (sl::ui::slider(m_id, m_data, m_range)) callback(m_data);
}

}  // namespace sl::ui
