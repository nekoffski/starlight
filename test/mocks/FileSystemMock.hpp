#pragma once

#include "sl/core/FileSystem.h"

#include <gmock/gmock.h>

using namespace sl::core;

class FileSystemMock : public FileSystem {
public:
    MOCK_METHOD(bool, isFile, (const Path& path), (const, override));
    MOCK_METHOD(bool, isDirectory, (const Path& path), (const, override));
    MOCK_METHOD(std::vector<Path>, listDirectory, (const Path& path), (const, override));
    MOCK_METHOD(void, writeFile, (const Path& path, const std::string& buffer), (const, override));
    MOCK_METHOD(std::string, readFile, (const Path& path), (const override));

    inline static std::string capturedPath;
    inline static std::string capturedFileContent;
};

inline void writeFileArgsHijacker(const Path& path, const std::string& buffer) {
    FileSystemMock::capturedPath = path;
    FileSystemMock::capturedFileContent = buffer;
};
