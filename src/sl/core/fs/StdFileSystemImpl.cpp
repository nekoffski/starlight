
#include "StdFileSystemImpl.h"

#include <filesystem>

namespace sl::core::fs {

namespace fsys = std::filesystem;

bool StdFileSystemImpl::isFile(const Path& path) {
    return fsys::is_regular_file(path);
}

bool StdFileSystemImpl::isDirectory(const Path& path) {
    return fsys::is_directory(path);
}

std::vector<std::string> StdFileSystemImpl::listDirectory(const Path& path) {
    std::vector<std::string> entries;
    for (auto& entry : fsys::directory_iterator(path))
        entries.emplace_back(entry.path());

    return entries;
}
}
