#pragma once

#include <ranges>
#include <span>
#include <vector>

#include "Concepts.hh"
#include "Core.hh"

namespace sl {

template <typename Key, typename Value>
class FlatMap : public NonCopyable {
    struct Record {
        template <typename... Args>
            requires std::is_constructible_v<Value, Args...>
        explicit Record(const Key& k, Args&&... args)
            : k(k), v(std::forward<Args>(args)...) {}

        Key k;
        Value v;
    };

   public:
    explicit FlatMap(u64 initialCapacity = 0u) {
        m_records.reserve(initialCapacity);
    }

    FlatMap(FlatMap&& other) noexcept = default;
    FlatMap& operator=(FlatMap&& other) noexcept = default;

    template <typename... Args>
        requires std::constructible_from<Value, Args&&...>
    Value& emplace(const Key& key, Args&&... args) {
        return m_records.emplace_back(key, std::forward<Args>(args)...).v;
    }

    void insert(const Key& key, const Value& value)
        requires std::constructible_from<Value, const Value&>
    {
        emplace(key, value);
    }

    void insert(const Key& key, Value&& value)
        requires std::constructible_from<Value, Value&&>
    {
        emplace(key, std::move(value));
    }

    bool has(const Key& k) const { return find(k) != nullptr; }

    auto find(this auto&& self, const Key& key) {
        auto it = std::ranges::find(self.m_records, key, &Record::k);
        return it == self.m_records.end() ? nullptr : &it->v;
    }

    void remove(const Key& key) {
        std::erase_if(m_records, [&](const Record& r) { return r.k == key; });
    }

    void remove(std::span<const Key> keys) {
        for (const auto& key : keys) {
            remove(key);
        }
    }

    void forEach(this auto&& self, auto&& cb)
        requires requires {
            cb(std::as_const(self.m_records.front().k),
               self.m_records.front().v);
        }
    {
        for (auto& record : self.m_records) {
            cb(std::as_const(record.k), record.v);
        }
    }

    void forEach(this auto&& self, auto&& cb)
        requires requires { cb(self.m_records.front().v); }
    {
        for (auto& record : self.m_records) {
            cb(record.v);
        }
    }

   private:
    std::vector<Record> m_records;
};

}  // namespace sl
