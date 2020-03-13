#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <numeric>

namespace starl::platform::gpu {

struct BufferMemoryOffset {
    uint32_t index;
    uint32_t elementsCount;
    uint32_t type;
    uint32_t typeSize;
    uint32_t totalLength;
    bool normalized;
    void* begin;
};

class BufferMemoryScheme {
public:
    const auto begin() const {
        return m_offsets.cbegin();
    }

    const auto end() const {
        return m_offsets.cend();
    }

    void addScheme(uint32_t elementsCount, uint32_t type, uint32_t typeSize, bool normalized) {
        // clang-format off
        auto stride = calculateStride();
        m_offsets.push_back({ 
            static_cast<uint32_t>(m_offsets.size()),
            elementsCount,
            type,
            typeSize,
            elementsCount * typeSize,
            normalized,
            (void*)stride
        });
        // clang-format on
    }

    uint64_t calculateStride() const {
        auto lengthAdder = [](const auto& previous, const auto& element) {
            return previous + element.totalLength;
        };
        return std::accumulate(m_offsets.cbegin(), m_offsets.cend(), 0u, lengthAdder);
    }

private:
    std::vector<BufferMemoryOffset> m_offsets;
    unsigned int m_stride;
};
}