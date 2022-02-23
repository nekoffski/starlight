#pragma once

namespace sl::rendering {

class StageBase {
   public:
    explicit StageBase() : m_enabled(true) {}

    void enable() { m_enabled = true; }

    void disable() { m_enabled = false; }

    bool isEnabled() const { return m_enabled; }

   private:
    bool m_enabled;
};

}  // namespace sl::rendering