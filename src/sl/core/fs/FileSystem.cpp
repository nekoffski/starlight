#include "FileSystem.h"

#include "StdFileSystemImpl.h"

namespace sl::core::fs {

void FileSystem::init() {
    m_fileSystemImpl = std::make_unique<StdFileSystemImpl>();
}
}
