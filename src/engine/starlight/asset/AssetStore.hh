#pragma once

#include <algorithm>
#include <kcore/Functional.hh>

#include "Asset.hh"
#include "starlight/renderer/RendererProxy.hh"

namespace sl {

template <typename Record, typename Reference>
class AssetStore : public kc::NonCopyable, public kc::NonMovable {
    using Handle = typename Record::Handle;
    using Entry = std::shared_ptr<Record>;
    using Destroy = Result<void> (RendererProxy::*)(Handle);

   public:
    AssetStore(RendererProxy& proxy, Destroy destroy)
        : m_proxy(proxy), m_destroy(destroy) {}

    // Register ownership before initialization can submit renderer commands.
    template <typename... Args>
    Result<Reference> create(Args&&... args) {
        Entry record{new Record(m_proxy)};
        kc::DeferCall rollback{[&] {
            if (record->empty()) {
                std::erase(m_records, record);
            }
        }};
        m_records.push_back(record);
        const auto r = record->initialize(std::forward<Args>(args)...);
        if (not r) {
            return Error::unexpected(r.error());
        }
        rollback.dismiss();
        return reference(std::move(record));
    }

    static Reference reference(Entry record) noexcept {
        return Reference{std::move(record)};
    }

    static std::weak_ptr<Record> weakReference(
        const Reference& reference
    ) noexcept {
        return reference.m_record;
    }

    bool hasReferences() const {
        return std::ranges::any_of(m_records, [](const auto& record) {
            return record.use_count() > 1;
        });
    }

    // Retires records when the store holds their last shared reference.
    Result<void> update() {
        Opt<Error> firstError;
        for (const auto& record : m_records) {
            record->poll();
            if (record.use_count() == 1) {
                record->retireCurrent();
            }
            while (const auto handle = record->nextRetiredHandle()) {
                const auto r = (m_proxy.*m_destroy)(*handle);
                if (not r) {
                    if (not firstError) {
                        firstError = r.error();
                    }
                    break;
                }
                record->confirmReleaseSubmitted();
            }
        }
        std::erase_if(m_records, [](const auto& record) {
            return record.use_count() == 1 && record->empty();
        });
        if (firstError) {
            return Error::unexpected(*firstError);
        }
        return {};
    }

   private:
    RendererProxy& m_proxy;
    Destroy m_destroy;
    std::vector<Entry> m_records;
};

}  // namespace sl
