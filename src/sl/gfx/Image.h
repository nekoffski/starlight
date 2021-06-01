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
        return factory->create(path, desiredChannels);
    }

    virtual ~Image() = default;

    virtual unsigned char* getRaw() = 0;
    virtual ImageSize getSize() = 0;
    virtual int getChannels() = 0;
};
}
