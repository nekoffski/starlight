#pragma once

#include <memory>
#include <string>

#include "FileSystemImpl.h"

namespace sl::core::fs {

class FileSystem {
public:
    static void init();

    static bool isFile(const Path& path) {
        return m_fileSystemImpl->isFile(path);
    }

    static bool isDirectory(const Path& path) {
        return m_fileSystemImpl->isDirectory(path);
    }

    static std::vector<std::string> listDirectory(const Path& path) {
        return m_fileSystemImpl->listDirectory(path);
    }

private:
    inline static std::unique_ptr<FileSystemImpl> m_fileSystemImpl;
};

using FS = FileSystem;
}
