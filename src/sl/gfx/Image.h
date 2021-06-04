#pragma once

#include <memory>

namespace sl::gfx {

struct ImageSize {
    int width;
    int height;
};

class Image {
public:
    struct Factory {
        virtual std::unique_ptr<Image> create(const std::string& path, int desiredChannels = 0) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    static std::unique_ptr<Image> load(const std::string& path, int desiredChannels = 0) {
        auto image = factory->create(path, desiredChannels);
        image->m_path = path;

        return image;
    }

    virtual ~Image() = default;

    virtual unsigned char* getRaw() = 0;
    virtual ImageSize getSize() = 0;
    virtual int getChannels() = 0;

    std::string getPath() const {
        return m_path;
    }

private:
    std::string m_path;
};
}
