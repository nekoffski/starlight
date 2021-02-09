#include "FileSystem.h"

#include <iostream>
#include <filesystem>

namespace sl::core {

namespace fsys = std::filesystem;

bool FileSystem::isFile(const Path& path) const {
    return fsys::is_regular_file(path);
}

bool FileSystem::isDirectory(const Path& path) const {
    return fsys::is_directory(path);
}

std::vector<std::string> FileSystem::listDirectory(const Path& path) const {
    std::vector<std::string> entries;
    for (auto& entry : fsys::directory_iterator(path))
        entries.emplace_back(entry.path());

    return entries;
}

void FileSystem::writeFile(const Path& path, const std::string& buffer) const {
    std::ofstream fs;
    fs.open(path);
	fs << buffer;
	fs.close();
}
}
