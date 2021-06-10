#include "FileSystem.h"

#include <filesystem>
#include <iostream>
#include <sstream>

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

void FileSystem::writeFile(const Path& path, const std::string& buffer, bool override) const {
    std::ofstream fs;
    fs.open(path, override ? std::ios::trunc : std::ios::app);
    fs << buffer;
    fs.close();
}

std::string FileSystem::readFile(const Path& path) const {
    std::ostringstream fileContentStream;
    std::ifstream fs;

    fs.open(path);
    fileContentStream << fs.rdbuf();
    fs.close();

    return fileContentStream.str();
}
}
