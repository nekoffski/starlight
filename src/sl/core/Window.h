#pragma once

#include <functional>
#include <memory>

namespace sl::core {

class Window {
public:
    struct Size {
        int width;
        int height;
    };

    using ResizeCallback = std::function<void(int, int)>;

    struct Factory {
        virtual std::shared_ptr<Window> create(Size windowSize, const std::string& title) = 0;
        virtual ~Factory() = default;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    explicit Window(Size windowSize, const std::string& title)
        : m_windowSize(std::move(windowSize))
        , m_title(title) { }

    virtual ~Window() = default;

    virtual void setResizeCallback(ResizeCallback) { }

    virtual void init() = 0;
    virtual void update(float) = 0;

    virtual bool getShouldClose() const = 0;
    virtual void setShouldClose(bool) = 0;

    virtual void changeCursorState(bool) = 0;
    virtual void enableCursor() = 0;
    virtual void disableCursor() = 0;

    virtual void* getHandle() const = 0;

    const Size& getSize() const {
        return m_windowSize;
    }

protected:
    Size m_windowSize;
    std::string m_title;
};
}
