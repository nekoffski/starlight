#include "FileSystem.hh"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace sl {

const Str& Path::str() const { return m_path; }

bool Path::isFile() const { return fs::is_regular_file(m_path); }

bool Path::isDirectory() const { return fs::is_directory(m_path); }

bool Path::exists() const { return fs::exists(m_path); }

Path Path::parent() const {
    auto parentPath = fs::path(m_path).parent_path();
    return Path{parentPath.string()};
}

Path Path::join(const Path& base, const Path& relative) {
    return Path{fs::path(base.str()) / fs::path(relative.str())};
}

Path Path::cwd() { return Path{fs::current_path().string()}; }

bool Path::endsWith(const Str& suffix) const {
    if (suffix.size() > m_path.size()) [[unlikely]] {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), m_path.rbegin());
}

void Path::append(const Str& suffix) { m_path += suffix; }

File::File(const Path& path) : m_path(path) {}

const Path& File::path() const { return m_path; }

Result<void> File::append(const Str& content) {
    std::ofstream file(m_path.str(), std::ios::app);
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to open file for appending: {}",
            m_path.str()
        );
    }
    file << content;
    return {};
}

Result<void> File::write(const Str& content) {
    std::ofstream file(m_path.str(), std::ios::trunc);
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to open file for writing: {}",
            m_path.str()
        );
    }
    file << content;
    return {};
}

Result<Str> File::read() const {
    std::ifstream file(m_path.str());
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to open file for reading: {}",
            m_path.str()
        );
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Result<std::vector<Str>> File::readLines() const {
    std::ifstream file(m_path.str());
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to open file for reading: {}",
            m_path.str()
        );
    }
    std::vector<Str> lines;
    Str line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

Result<std::vector<Byte>> File::readBinary() const {
    std::ifstream file(m_path.str(), std::ios::binary | std::ios::ate);
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to open file '{}' for reading",
            m_path.str()
        );
    }
    auto size = file.tellg();
    if (size % sizeof(Byte) != 0) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "File '{}' size is not a multiple of 1 byte", m_path.str()
        );
    }
    file.seekg(0);
    std::vector<Byte> buffer(static_cast<std::size_t>(size) / sizeof(Byte));
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

Result<void> File::remove() {
    std::error_code ec;
    fs::remove(m_path.str(), ec);
    if (ec) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to remove file '{}'",
            m_path.str()
        );
    }
    return {};
}

Directory::Directory(const Path& path) : m_path(path) {}

const Path& Directory::path() const { return m_path; }

std::vector<Path> Directory::listFiles() const {
    std::vector<Path> files;
    for (const auto& entry : fs::directory_iterator(m_path.str())) {
        if (entry.is_regular_file()) {
            files.emplace_back(entry.path().string());
        }
    }
    return files;
}

std::vector<Path> Directory::listDirectories() const {
    std::vector<Path> directories;
    for (const auto& entry : fs::directory_iterator(m_path.str())) {
        if (entry.is_directory()) {
            directories.emplace_back(entry.path().string());
        }
    }
    return directories;
}

Result<void> Directory::create() {
    std::error_code ec;
    fs::create_directories(m_path.str(), ec);
    if (ec) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to create directory '{}'",
            m_path.str()
        );
    }
    return {};
}

Result<void> Directory::remove() {
    std::error_code ec;
    fs::remove_all(m_path.str(), ec);
    if (ec) {
        return Error::unexpected(
            ErrorCode::fileSystemError, "Failed to remove directory '{}'",
            m_path.str()
        );
    }
    return {};
}

Result<void> Directory::createSubdirectory(const Path& name) {
    const auto subdirPath = Path::join(m_path, name);
    std::error_code ec;
    fs::create_directories(subdirPath.str(), ec);
    if (ec) {
        return Error::unexpected(
            ErrorCode::fileSystemError,
            "Failed to create subdirectory '{}' in '{}'", name.str(),
            m_path.str()
        );
    }
    return {};
}

Result<void> Directory::touch(const Path& name) {
    const auto filePath = Path::join(m_path, name);
    std::ofstream file(filePath.str(), std::ios::app);
    if (not file.is_open()) {
        return Error::unexpected(
            ErrorCode::fileSystemError,
            "Failed to create or open file '{}' in '{}'", name.str(),
            m_path.str()
        );
    }
    return {};
}

Path operator+(const Path& lhs, const Str& suffix) {
    return Path{lhs.str() + suffix};
}

}  // namespace sl
