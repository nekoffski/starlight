#pragma once

#include <string>
#include <vector>

namespace sl::core::fs {

using Path = std::string;

class FileSystemImpl {
public:
	virtual bool isFile(const Path&) = 0;
    virtual bool isDirectory(const Path&) = 0;
    virtual std::vector<std::string> listDirectory(const Path& paths) = 0;
};
}
