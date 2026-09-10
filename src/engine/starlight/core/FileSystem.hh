#pragma once

#include <string>
#include <vector>

#include "Core.hh"
#include "Error.hh"

namespace sl {

class Path {
   public:
    template <typename... Args>
        requires std::constructible_from<Str, Args...>
    Path(Args&&... args) : m_path(std::forward<Args>(args)...) {}

    template <typename... Args>
    static Path fmt(const Str& fmt, Args&&... args) {
        return Path(
            fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...)
        );
    }

    const Str& str() const;

    bool isFile() const;
    bool isDirectory() const;
    bool exists() const;

    Path parent() const;

    static Path join(const Path& base, const Path& relative);
    static Path cwd();

    bool endsWith(const Str& suffix) const;
    void append(const Str& suffix);

   private:
    Str m_path;
};

Path operator+(const Path& lhs, const Str& suffix);

class File {
   public:
    explicit File(const Path& path);

    const Path& path() const;

    Result<void> append(const Str& content);
    Result<void> write(const Str& content);
    Result<Str> read() const;
    Result<std::vector<Str>> readLines() const;
    Result<std::vector<Byte>> readBinary() const;

    Result<void> remove();

   private:
    Path m_path;
};

class Directory {
   public:
    explicit Directory(const Path& path);

    const Path& path() const;

    std::vector<Path> listFiles() const;
    std::vector<Path> listDirectories() const;

    Result<void> create();
    Result<void> createSubdirectory(const Path& name);
    Result<void> touch(const Path& name);
    Result<void> remove();

   private:
    Path m_path;
};

}  // namespace sl
