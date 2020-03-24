#pragma once

#include <memory>

namespace starl::platform::img {

struct ImageSize {
    int width;
    int height;
};

class Image {
public:
    static std::shared_ptr<Image> create(const std::string&);

    virtual ~Image() = default;

    virtual unsigned char* getRaw() = 0;
    virtual ImageSize getSize() = 0;
    virtual int getChannels() = 0;
};
}