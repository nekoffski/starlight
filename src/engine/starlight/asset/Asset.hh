#pragma once

#include <future>
#include <memory>
#include <thread>
#include <vector>

#include "starlight/core/Error.hh"

namespace sl {

// Engine-thread lifecycle shared by resource-specific asset types.
template <typename ResourceHandle>
class AssetRecord {
   public:
    using Handle = ResourceHandle;

    Result<Handle> handle() const {
        checkThread();
        if (m_current) {
            return *m_current;
        }
        if (m_error.valid()) {
            return Error::unexpected(m_error);
        }
        return Error::unexpected(
            ErrorCode::assetNotReady, "Asset creation is pending"
        );
    }

    bool pending() const {
        checkThread();
        return m_completion.valid();
    }

    u64 version() const {
        checkThread();
        return m_version;
    }

    Error error() const {
        checkThread();
        return m_error;
    }

    void poll() {
        using namespace std::chrono_literals;
        if (not pending() ||
            m_completion.wait_for(0s) != std::future_status::ready) {
            return;
        }
        // Keep the completed handle in its future until retirement can succeed.
        if (m_current) {
            m_retired.reserve(m_retired.size() + 1);
        }
        try {
            auto resource = m_completion.get();
            if (not resource) {
                m_error = std::move(resource.error());
                return;
            }
            retireCurrent();
            m_current = *resource;
            ++m_version;
            m_error.reset();
        } catch (const std::future_error& e) {
            m_completion = {};
            m_error = Error{
                ErrorCode::invalidFuture, "Asset creation failed: {}", e.what()
            };
        }
    }

    void retireCurrent() {
        checkThread();
        if (m_current) {
            m_retired.push_back(*m_current);
            m_current.reset();
        }
    }

    Opt<Handle> nextRetiredHandle() const {
        checkThread();
        if (m_retired.empty()) {
            return {};
        }
        return m_retired.back();
    }

    void confirmReleaseSubmitted() {
        checkThread();
        log::expect(
            not m_retired.empty(),
            "Asset has no retired handle awaiting release submission"
        );
        m_retired.pop_back();
    }

    bool empty() const {
        checkThread();
        return not m_completion.valid() && not m_current && m_retired.empty();
    }

   protected:
    AssetRecord() = default;
    ~AssetRecord() = default;

    void checkThread() const {
        log::expect(
            m_thread == std::this_thread::get_id(),
            "Asset access requires its engine thread"
        );
    }

    void adoptCreationFuture(std::future<Result<Handle>> future) {
        log::expect(not pending(), "Asset operation already pending");
        m_completion = std::move(future);
        m_error.reset();
    }

    Result<void> fail(const Error& error) {
        checkThread();
        m_error = error;
        return Error::unexpected(error);
    }

   private:
    Opt<Handle> m_current;
    std::future<Result<Handle>> m_completion;
    std::vector<Handle> m_retired;
    Error m_error;
    u64 m_version{0};
    const std::thread::id m_thread{std::this_thread::get_id()};
};

// Keep the asset registry alive through every reference's lifetime. Reference
// use, including copying and destruction, belongs to its engine thread.
template <typename Record>
class AssetRef {
   public:
    Result<typename Record::Handle> handle() const {
        if (not m_record) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument, "Empty asset reference"
            );
        }
        return m_record->handle();
    }

    bool pending() const { return m_record && m_record->pending(); }

    u64 version() const { return m_record ? m_record->version() : 0; }

    Error error() const {
        if (not m_record) {
            return Error{
                kc::ErrorCode::invalidArgument, "Empty asset reference"
            };
        }
        return m_record->error();
    }

    bool operator==(const AssetRef&) const = default;

   protected:
    AssetRef() = default;
    explicit AssetRef(std::shared_ptr<Record> record) noexcept
        : m_record(std::move(record)) {}
    AssetRef(const AssetRef&) = default;
    AssetRef(AssetRef&&) noexcept = default;
    AssetRef& operator=(const AssetRef&) = default;
    AssetRef& operator=(AssetRef&&) noexcept = default;
    ~AssetRef() = default;

    std::shared_ptr<Record> m_record;
};

}  // namespace sl
