#pragma once

#include <kc/core/FileSystem.h>

#include <gmock/gmock.h>

class FileSystemMock : public kc::core::FileSystem {
public:
    MOCK_METHOD(bool, isFile, (const Path& path), (const, override));
    MOCK_METHOD(bool, isDirectory, (const Path& path), (const, override));
    MOCK_METHOD(std::vector<Path>, listDirectory, (const Path& path), (const, override));
    MOCK_METHOD(void, writeFile, (const Path& path, const std::string& buffer, kc::core::FileSystem::WritePolicy), (const, override));
    MOCK_METHOD(std::string, readFile, (const Path& path), (const override));

    inline static std::string capturedPath;
    inline static std::string capturedFileContent;
    inline static WritePolicy writePolicy;
};

inline void writeFileArgsHijacker(const kc::core::FileSystem::Path& path,
    const std::string& buffer, kc::core::FileSystem::WritePolicy writePolicy) {

    FileSystemMock::capturedPath = path;
    FileSystemMock::capturedFileContent = buffer;
    FileSystemMock::writePolicy = writePolicy;
};
