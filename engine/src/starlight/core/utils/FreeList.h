#pragma once

#include <vector>
#include <cstdint>

#include <starlight/core/utils/Log.h>
#include <starlight/core/utils/Id.h>

namespace sl {

class FreeList {
    struct Node {
        explicit Node();

        void invalidate();

        uint64_t offset;
        uint64_t size;
        Node* next;
    };

public:
    explicit FreeList(uint64_t size);

    void freeBlock(uint64_t size, uint64_t offset);
    uint64_t allocateBlock(uint64_t size);
    uint64_t spaceLeft();

    void resize(uint64_t newSize);

    void clear();

private:
    Node* getFreeNode();

    uint64_t m_totalSize;
    uint64_t m_maxEntries;

    std::vector<Node> m_nodes;
    Node* m_head;
};

}  // namespace sl
