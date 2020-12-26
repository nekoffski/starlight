#pragma once

#include <string>
#include <vector>

namespace sl::core {

using Path = std::string;

class FileSystem {
public:
    static bool isFile(const Path& path);
    static bool isDirectory(const Path& path);
    static std::vector<Path> listDirectory(const Path& path);
};

using FS = FileSystem;
}
