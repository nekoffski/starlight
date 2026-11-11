#include <gtest/gtest.h>

#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <kcore/Random.hh>
#include <kcore/runtime/ThreadSafeQueue.hh>
#include <new>
#include <optional>
#include <thread>
#include <type_traits>

#include "starlight/asset/AssetSystem.hh"

namespace {

thread_local std::optional<size_t> allocationsBeforeFailure;
thread_local bool allocationFailed{false};

class AllocationFailure {
   public:
    explicit AllocationFailure(size_t allowedAllocations) {
        allocationsBeforeFailure = allowedAllocations;
        allocationFailed = false;
    }
    ~AllocationFailure() { allocationsBeforeFailure.reset(); }

    bool triggered() const { return allocationFailed; }
};

}  // namespace

// This executable isolates allocation injection from the other test suites.
void* operator new(size_t size) {
    if (allocationsBeforeFailure) {
        if (*allocationsBeforeFailure == 0) {
            allocationsBeforeFailure.reset();
            allocationFailed = true;
            throw std::bad_alloc{};
        }
        --*allocationsBeforeFailure;
    }
    if (void* memory = std::malloc(size == 0 ? 1 : size)) {
        return memory;
    }
    throw std::bad_alloc{};
}

void* operator new[](size_t size) { return ::operator new(size); }
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, size_t) noexcept { std::free(memory); }

namespace {

using namespace sl;

// Complete real proxy commands without involving native GPU allocations.
class CommandRenderer {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;

   public:
    CommandRenderer() : m_worker([this] { run(); }) {}
    ~CommandRenderer() {
        m_queue->close();
        m_worker.join();
    }

    RendererProxy proxy() {
        return RendererProxy{RenderExecutor::Submitter{m_queue}};
    }
    u32 created() const { return m_created; }
    u32 destroyed() const { return m_destroyed; }

   private:
    void run() {
        while (auto command = m_queue->pop()) {
            std::visit(
                [this](auto& value) {
                    using Command = std::decay_t<decltype(value)>;
                    if constexpr (
                        std::is_same_v<Command, RendererCreateShader>
                    ) {
                        value.completion.set_value(ShaderHandle{++m_created});
                    } else if constexpr (
                        std::is_same_v<Command, RendererCreateTexture>
                    ) {
                        value.completion.set_value(TextureHandle{++m_created});
                    } else if constexpr (
                        std::is_same_v<Command, RendererCreateSampler>
                    ) {
                        value.completion.set_value(SamplerHandle{++m_created});
                    } else if constexpr (
                        std::is_same_v<Command, RendererDestroyShader> ||
                        std::is_same_v<Command, RendererDestroyTexture> ||
                        std::is_same_v<Command, RendererDestroySampler>
                    ) {
                        ++m_destroyed;
                    } else if constexpr (
                        std::is_same_v<Command, RendererFlush>
                    ) {
                        value.completion.set_value();
                    } else {
                        log::panic("Unexpected command in allocation test");
                    }
                },
                *command
            );
        }
    }

    std::shared_ptr<Queue> m_queue{std::make_shared<Queue>()};
    std::atomic<u32> m_created{0};
    std::atomic<u32> m_destroyed{0};
    std::thread m_worker;
};

void verifyCreationHandoff(const Config& cfg, auto&& create) {
    constexpr size_t maxAllocationPoints = 256;
    for (size_t allowed = 0; allowed < maxAllocationPoints; ++allowed) {
        CommandRenderer renderer;
        AssetSystem assets{cfg, renderer.proxy()};
        bool succeeded = false;
        bool injected = false;
        {
            AllocationFailure failure{allowed};
            try {
                auto asset = create(assets);
                succeeded = bool(asset);
            } catch (const std::bad_alloc&) {
            }
            injected = failure.triggered();
        }
        if (injected) {
            auto retry = create(assets);
            ASSERT_TRUE(retry);
        }
        ASSERT_TRUE(assets.shutdown());
        EXPECT_EQ(renderer.created(), renderer.destroyed())
            << "Allocation failure after " << allowed
            << " successful allocations";
        if (not injected) {
            EXPECT_TRUE(succeeded);
            EXPECT_EQ(renderer.created(), 1);
            return;
        }
    }
    FAIL() << "Never reached a creation without an injected allocation failure";
}

struct Record : AssetRecord<TextureHandle> {
    using AssetRecord::adoptCreationFuture;
};

TEST(AssetAllocation, ReplacementKeepsItsFutureUntilPublicationCanSucceed) {
    const TextureHandle original{1};
    const TextureHandle replacement{2};
    Record record;
    std::promise<Result<TextureHandle>> first;
    record.adoptCreationFuture(first.get_future());
    first.set_value(original);
    record.poll();

    std::promise<Result<TextureHandle>> next;
    record.adoptCreationFuture(next.get_future());
    next.set_value(replacement);
    bool threw = false;
    {
        AllocationFailure failure{0};
        try {
            record.poll();
        } catch (const std::bad_alloc&) {
            threw = true;
        }
    }
    ASSERT_TRUE(threw);
    EXPECT_TRUE(record.pending());
    EXPECT_EQ(*record.handle(), original);
    EXPECT_EQ(record.version(), 1);
    EXPECT_FALSE(record.nextRetiredHandle());

    record.poll();
    EXPECT_FALSE(record.pending());
    EXPECT_EQ(*record.handle(), replacement);
    EXPECT_EQ(record.version(), 2);
    ASSERT_TRUE(record.nextRetiredHandle());
    EXPECT_EQ(*record.nextRetiredHandle(), original);
    record.confirmReleaseSubmitted();
    record.retireCurrent();
    ASSERT_TRUE(record.nextRetiredHandle());
    EXPECT_EQ(*record.nextRetiredHandle(), replacement);
    record.confirmReleaseSubmitted();
    EXPECT_TRUE(record.empty());
}

TEST(AssetAllocation, ShaderCacheAllocationFailureCanRetryCreation) {
    const auto directory =
        std::filesystem::temp_directory_path() /
        ("starlight-asset-allocation-" + kc::RandomEngine::get().uuid());
    std::filesystem::create_directory(directory);
    const auto originalDirectory = std::filesystem::current_path();
    kc::DeferCall cleanup{[&] {
        std::filesystem::current_path(originalDirectory);
        std::filesystem::remove_all(directory);
    }};
    std::ofstream(directory / "shader.bin", std::ios::binary).put('A');
    std::ofstream(directory / "shader.bin.json") << R"({"entryPoints":[]})";
    std::filesystem::current_path(directory);
    Config cfg;
    cfg.paths.assets = kc::Path{""};
    CommandRenderer renderer;
    AssetSystem assets{cfg, renderer.proxy()};
    bool threw = false;
    {
        AllocationFailure failure{0};
        try {
            auto shader = assets.loadShader("shader.bin");
        } catch (const std::bad_alloc&) {
            threw = true;
        }
    }
    ASSERT_TRUE(threw);
    EXPECT_EQ(renderer.created(), 0);
    auto shader = assets.loadShader("shader.bin");
    ASSERT_TRUE(shader);
    ASSERT_TRUE(assets.waitForPending());
    EXPECT_TRUE(shader->handle());
    shader = ShaderRef{};
    ASSERT_TRUE(assets.shutdown());
    EXPECT_EQ(renderer.created(), 1);
    EXPECT_EQ(renderer.destroyed(), 1);
}

TEST(AssetAllocation, TextureRegistrationKeepsEveryAcceptedCreationOwned) {
    const auto image = makeCheckerboard(4, 2);
    ASSERT_TRUE(image);
    verifyCreationHandoff(Config{}, [&](AssetSystem& assets) {
        return assets.createTexture(*image);
    });
}

TEST(AssetAllocation, SamplerRegistrationKeepsEveryAcceptedCreationOwned) {
    verifyCreationHandoff(Config{}, [](AssetSystem& assets) {
        return assets.createSampler({});
    });
}

}  // namespace
