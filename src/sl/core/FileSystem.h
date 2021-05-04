#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <filesystem>

namespace sl::core {

using Path = std::string;

class FileSystem {
public:
    virtual bool isFile(const Path& path) const;
    virtual bool isDirectory(const Path& path) const;
    virtual std::vector<Path> listDirectory(const Path& path) const;
    virtual void writeFile(const Path& path, const std::string& buffer) const;
    virtual std::string readFile(const Path& path) const;

    auto lastWriteTime(const Path& path) const {
        return std::filesystem::last_write_time(path);
    }
};

using FS = FileSystem;
}
