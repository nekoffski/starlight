#pragma once

#include <memory>

namespace starl::platform::img {

using ImageSize = std::pair<int, int>;

class Image {
public:
    static std::shared_ptr<Image> create(const std::string&);

    virtual ~Image() = default;

    virtual unsigned char* getRaw() = 0;
    virtual ImageSize getSize() = 0;
    virtual int getChannels() = 0;
};
}