#pragma once

#include <concepts>
#include <cstring>
#include <type_traits>
#include <utility>
#include <vector>

#include "GeometryData.hh"

namespace sl {

template <typename T>
concept GeometryVertex = std::is_trivially_copyable_v<T> && requires {
    { T::layout() } -> std::same_as<VertexLayout>;
};

template <GeometryVertex T>
class GeometryDataBuilder {
   public:
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    GeometryDataBuilder& addVertex(Args&&... args) {
        m_vertices.emplace_back(std::forward<Args>(args)...);
        return *this;
    }

    GeometryDataBuilder& addVertex(const T& vertex) {
        m_vertices.push_back(vertex);
        return *this;
    }

    GeometryDataBuilder& addIndex(u32 index) {
        m_indices.push_back(index);
        return *this;
    }

    GeometryDataBuilder& addIndices(const std::vector<u32>& indices) {
        m_indices.insert(m_indices.end(), indices.begin(), indices.end());
        return *this;
    }

    GeometryData build() {
        GeometryData data;

        data.vertexLayout = T::layout();
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
