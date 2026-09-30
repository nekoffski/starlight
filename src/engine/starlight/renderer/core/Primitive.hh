#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Vertex.hh"
#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/core/Shader.hh"

namespace sl {

struct PrimitiveUploadData {
    Bytes vertexData;
    Bytes indexData;
    u32 vertexCount;
    u32 indexCount;
};

template <typename T>
class PrimitiveUploadDataBuilder {
   public:
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    PrimitiveUploadDataBuilder& addVertex(Args&&... args) {
        m_vertices.emplace_back(std::forward<Args>(args)...);
        return *this;
    }

    PrimitiveUploadDataBuilder& addVertex(const T& vertex) {
        m_vertices.push_back(vertex);
        return *this;
    }

    PrimitiveUploadDataBuilder& addIndex(u32 index) {
        m_indices.push_back(index);
        return *this;
    }

    PrimitiveUploadDataBuilder& addIndices(const std::vector<u32>& indices) {
        m_indices.insert(m_indices.end(), indices.begin(), indices.end());
        return *this;
    }

    PrimitiveUploadData build() {
        PrimitiveUploadData data;

        data.indexCount = m_indices.size();
        data.vertexCount = m_vertices.size();

        data.indexData.resize(data.indexCount * sizeof(u32));
        std::memcpy(
            data.indexData.data(), m_indices.data(), data.indexData.size()
        );

        data.vertexData.resize(data.vertexCount * sizeof(T));
        std::memcpy(
            data.vertexData.data(), m_vertices.data(), data.vertexData.size()
        );

        return data;
    };

   private:
    std::vector<T> m_vertices;
    std::vector<u32> m_indices;
};

}  // namespace sl

// namespace sl
