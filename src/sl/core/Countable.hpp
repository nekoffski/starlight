#pragma once

#include <sstream>
#include <vector>

#include "Logger.h"

namespace sl::core {

template <typename T>
struct Countable {
    using ulong = unsigned long;

public:
    Countable()
        : id(getNextId()) {
        s_takenIds.push_back(id);
    }

    ~Countable() {
        s_freeIds.push_back(id);
        auto position = std::find(s_takenIds.begin(), s_takenIds.end(), id);
        s_takenIds.erase(position);
    }

    void setId(ulong id) {
        if (this->id == id)
            return;

        assertNotTaken(id);

        s_freeIds.push_back(this->id);
        s_takenIds.push_back(id);
        this->id = id;

        auto position = std::find(s_freeIds.begin(), s_freeIds.end(), id);
        if (position != s_freeIds.end())
            s_freeIds.erase(position);
    }

    ulong id;

private:
    static void assertNotTaken(ulong id) {
        auto position = std::find(s_takenIds.begin(), s_takenIds.end(), id);
        SL_ASSERT(position == s_takenIds.end(), "Mismatch in Countable for id: " + std::to_string(id) + "\n" + raport());
    }

    static std::string raport() {
        std::ostringstream stream;

        stream << "Next free id: " << s_nextId << '\n';
        stream << "Free ids: ";

        for (auto& id : s_freeIds)
            stream << id << ", ";
        stream << "\nTaken ids: ";

        for (auto& id : s_takenIds)
            stream << id << ", ";
        stream << '\n';
        return stream.str();
    }

    static ulong getNextId() {
        if (not s_freeIds.empty()) {
            ulong nextId = s_freeIds.back();
            s_freeIds.pop_back();
            return nextId;
        }

        while (1) {
            auto position = std::find(s_takenIds.begin(), s_takenIds.end(), s_nextId);

            if (position != s_takenIds.end()) {
                s_nextId++;
                continue;
            }

            return s_nextId++;
        }
    }

    inline static std::vector<ulong> s_freeIds;
    inline static std::vector<ulong> s_takenIds;
    inline static ulong s_nextId = 0;
};

}
