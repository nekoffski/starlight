#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace sl::core {

using Path = std::string;

class FileSystem {
public:
    static bool isFile(const Path& path);
    static bool isDirectory(const Path& path);
    static std::vector<Path> listDirectory(const Path& path);
    static void writeFile(const Path& path, const std::string& buffer);
};

using FS = FileSystem;
}
