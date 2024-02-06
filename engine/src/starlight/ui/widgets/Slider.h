#pragma once

#include "starlight/ui/Core.h"

namespace sl::ui {

class Slider3 {
    using Callback = std::function<void(const Vec3f&)>;

public:
    explicit Slider3(
      const std::string& name, const StepInterval<float>& range,
      const Vec3f& initialValue = Vec3f{ 0.0 }
    );

    void render(Callback&& callback);

private:
    std::string m_name;
    std::string m_id;

    Vec3f m_data;

    StepInterval<float> m_range;
};

}  // namespace sl::ui