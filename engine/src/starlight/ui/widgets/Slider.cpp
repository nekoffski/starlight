#include "Slider.hh"

#include "starlight/ui/widgets/Text.hh"

namespace sl {

Slider3::Slider3(
  const std::string& name, const StepInterval<float>& range,
  const Vec3<f32>& initialValue
)
    : m_name(name)
    , m_id(fmt::format("##{}", name))
    , m_data(initialValue)
    , m_range(range) {}

void Slider3::render(Callback&& callback) {
    text("{}", m_name);
    if (immediateSlider(m_id, m_data, m_range)) callback(m_data);
}

bool immediateSlider(
  const std::string& name, Vec3<f32>& data, const StepInterval<float>& range
) {
    return ImGui::DragFloat3(
      name.c_str(), &data[0], range.step, range.min, range.max
    );
}

}  // namespace sl
