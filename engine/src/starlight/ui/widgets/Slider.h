#pragma once

#include "starlight/ui/Core.h"

namespace sl::ui {

class Slider3 {
    using Callback = std::function<void(const Vec3<f32>&)>;

public:
    explicit Slider3(
      const std::string& name, const StepInterval<float>& range,
      const Vec3<f32>& initialValue = Vec3<f32>{ 0.0 }
    );

    void render(Callback&& callback);

private:
    std::string m_name;
    std::string m_id;

    Vec3<f32> m_data;

    StepInterval<float> m_range;
};

}  // namespace sl::ui