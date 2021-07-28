#pragma once

#include <memory>

namespace sl::gfx {

struct ImageSize {
    int width;
    int height;
};

class TextureManager;

class Image {
    friend class TextureManager;

public:
    struct Factory {
        virtual std::unique_ptr<Image> create(const std::string& path, int desiredChannels = 0) = 0;
    };

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
