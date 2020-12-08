#pragma once

#include "FileSystemImpl.h"

#include <string>
#include <vector>

namespace sl::core::fs {

class StdFileSystemImpl : public FileSystemImpl {
public:
    bool isFile(const Path& path) override;
    bool isDirectory(const Path& path) override;
    std::vector<std::string> listDirectory(const Path& path) override;
};
}
