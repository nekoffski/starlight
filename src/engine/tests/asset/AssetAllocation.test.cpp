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
#include <unordered_set>

#include "starlight/asset/AssetSystem.hh"
#include "starlight/geometry/GeometryDataBuilder.hh"
#include "starlight/geometry/Vertex.hh"
#include "starlight/renderer/frontend/Renderer.hh"

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
        return RendererProxy{RendererCommandSubmitter{m_queue}};
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
                        std::is_same_v<Command, RendererCreatePrimitive>
                    ) {
                        value.completion.set_value(
                            PrimitiveHandle{++m_created}
                        );
                    } else if constexpr (
                        std::is_same_v<Command, RendererDestroyShader> ||
                        std::is_same_v<Command, RendererDestroyTexture> ||
                        std::is_same_v<Command, RendererDestroySampler> ||
                        std::is_same_v<Command, RendererDestroyPrimitive>
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

// Track pipeline ownership without allocating after either injection hook.
class PipelineDevice : public RenderDevice,
                       public RenderFrameRecorder,
                       public RenderPassEncoder {
   public:
    kc::MoveOnlyFunction<void()> afterPipelineCreation;
    kc::MoveOnlyFunction<void()> afterWaitIdle;
    std::unordered_set<GraphicsPipelineHandle> pipelines;
    std::unordered_set<ShaderHandle> shaders{ShaderHandle{1}, ShaderHandle{2}};
    u32 createdPipelines{0};
    u32 duplicateDestructions{0};
    bool boundStalePipeline{false};
    Error pipelineError;
    Error recordingError;

    Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription&
    ) override {
        if (pipelineError.valid()) {
            return Error::unexpected(pipelineError);
        }
        const GraphicsPipelineHandle handle{++createdPipelines};
        pipelines.insert(handle);
        if (afterPipelineCreation) {
            afterPipelineCreation();
        }
        return handle;
    }
    void destroyGraphicsPipeline(GraphicsPipelineHandle handle) override {
        if (pipelines.erase(handle) != 1) {
            ++duplicateDestructions;
        }
    }
    const ShaderDescription* getShaderDescription(
        ShaderHandle handle
    ) const override {
        return shaders.contains(handle) ? &m_shader : nullptr;
    }
    void destroyShader(ShaderHandle handle) override { shaders.erase(handle); }
    void waitIdle() override {
        if (afterWaitIdle) {
            afterWaitIdle();
        }
    }
    Result<void> trySubmitFrame(RecordFrame record) override {
        recordingError.reset();
        const auto r = record(*this);
        if (not r) {
            recordingError = r.error();
        }
        return r;
    }
    Result<void> renderPass(
        const RenderPassDescription&, const RenderPassCallback& record
    ) override {
        return record(*this);
    }
    Result<void> setPipeline(GraphicsPipelineHandle handle) override {
        boundStalePipeline |= not pipelines.contains(handle);
        return {};
    }
    Result<void> setVertexBuffer(const DeviceBufferSlice&) override {
        return {};
    }
    Result<void> setIndexBuffer(const DeviceBufferSlice&) override {
        return {};
    }
    Result<void> setShaderParameters(
        const ShaderDescription&, std::span<const ShaderParameterBlock>,
        const ShaderResourceDependencies&
    ) override {
        return {};
    }
    void draw(u32) override {}
    void drawIndexed(const IndexedDrawDescription&) override {}
    Result<DeviceBufferHandle> createBuffer(
        const DeviceBufferDescription&
    ) override {
        return DeviceBufferHandle{++m_bufferId};
    }
    void destroyBuffer(DeviceBufferHandle) override {}
    Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider>
    ) override {
        return SurfaceHandle{};
    }
    void destroySurface(SurfaceHandle) override {}
    Result<ShaderHandle> createShader(const ShaderDescription&) override {
        return ShaderHandle{1};
    }
    Result<TextureHandle> createTexture(const TextureUploadData&) override {
        return TextureHandle{};
    }
    void destroyTexture(TextureHandle) override {}
    Result<SamplerHandle> createSampler(const SamplerDescription&) override {
        return SamplerHandle{};
    }
    void destroySampler(SamplerHandle) override {}
    Result<void> validateResources(
        const ShaderResourceDependencies&
    ) const override {
        return {};
    }

   private:
    ShaderDescription m_shader{.vertexInputs = ShaderVertexInputs{}};
    u32 m_bufferId{0};
};

GeometryData pipelineGeometry() {
    return GeometryDataBuilder<Vertex3>{}
        .addVertex(Vec3f{0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{1, 0, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{0, 1, 0}, Vec3f{0, 0, 1})
        .addIndices({0, 1, 2})
        .build();
}

RenderRequest pipelineRequest(
    PrimitiveHandle primitive, ShaderHandle shader = ShaderHandle{1}
) {
    return {
        .scene =
            {.items =
                 {{.primitives = {{.primitive = primitive}},
                   .shader = shader}}},
        .views = {{.target = SurfaceHandle{0}}},
    };
}

TEST(PipelineAllocation, CacheInsertionFailureReleasesPipelineAndCanRetry) {
    PipelineDevice device;
    Renderer renderer{{}, device};
    auto primitive = renderer.createPrimitive(pipelineGeometry());
    ASSERT_TRUE(primitive);
    const auto request = pipelineRequest(*primitive);
    ASSERT_TRUE(renderer.submit(request));

    std::optional<AllocationFailure> failure;
    device.afterPipelineCreation = [&] { failure.emplace(0); };
    bool threw = false;
    try {
        renderer.tick();
    } catch (const std::bad_alloc&) {
        threw = true;
    }
    const bool injected = failure && failure->triggered();
    failure.reset();
    device.afterPipelineCreation = {};
    ASSERT_TRUE(injected);
    EXPECT_TRUE(threw);
    EXPECT_EQ(device.createdPipelines, 1);
    EXPECT_TRUE(device.pipelines.empty());

    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    EXPECT_EQ(device.createdPipelines, 2);
    EXPECT_EQ(device.pipelines.size(), 1);
    EXPECT_FALSE(device.boundStalePipeline);
    renderer.destroyShader(ShaderHandle{1});
    EXPECT_TRUE(device.pipelines.empty());
    EXPECT_EQ(device.duplicateDestructions, 0);
    renderer.destroyPrimitive(*primitive);
}

TEST(PipelineAllocation, RetirementAllocatesBeforeDestroyingAnyPipeline) {
    constexpr size_t maxRetirementAllocationPoints = 16;
    for (size_t allowed = 0; allowed < maxRetirementAllocationPoints;
         ++allowed) {
        PipelineDevice device;
        Renderer renderer{{}, device};
        auto data = pipelineGeometry();
        auto primitive = renderer.createPrimitive(data);
        data.vertexLayout.attributes.pop_back();
        auto otherLayout = renderer.createPrimitive(data);
        ASSERT_TRUE(primitive);
        ASSERT_TRUE(otherLayout);
        auto request = pipelineRequest(*primitive);
        request.scene.items.front().primitives.push_back(
            {.primitive = *otherLayout}
        );
        request.scene.items.push_back(
            {.primitives = {{.primitive = *primitive}},
             .shader = ShaderHandle{2}}
        );
        ASSERT_TRUE(renderer.submit(request));
        renderer.tick();
        ASSERT_EQ(device.pipelines.size(), 3);

        std::optional<AllocationFailure> failure;
        device.afterWaitIdle = [&] { failure.emplace(allowed); };
        bool threw = false;
        try {
            renderer.destroyShader(ShaderHandle{1});
        } catch (const std::bad_alloc&) {
            threw = true;
        }
        const bool injected = failure && failure->triggered();
        failure.reset();
        device.afterWaitIdle = {};
        if (injected) {
            EXPECT_TRUE(threw);
            EXPECT_EQ(device.pipelines.size(), 3);
            EXPECT_TRUE(device.shaders.contains(ShaderHandle{1}));
            ASSERT_TRUE(renderer.submit(request));
            renderer.tick();
            EXPECT_EQ(device.createdPipelines, 3);
            EXPECT_FALSE(device.boundStalePipeline);
            renderer.destroyShader(ShaderHandle{1});
        }
        EXPECT_EQ(device.pipelines.size(), 1);
        EXPECT_FALSE(device.shaders.contains(ShaderHandle{1}));
        EXPECT_TRUE(device.shaders.contains(ShaderHandle{2}));
        ASSERT_TRUE(
            renderer.submit(pipelineRequest(*primitive, ShaderHandle{2}))
        );
        renderer.tick();
        EXPECT_EQ(device.createdPipelines, 3);
        renderer.destroyShader(ShaderHandle{2});
        EXPECT_TRUE(device.pipelines.empty());
        EXPECT_EQ(device.duplicateDestructions, 0);
        renderer.destroyPrimitive(*primitive);
        renderer.destroyPrimitive(*otherLayout);
        if (not injected) {
            EXPECT_FALSE(threw);
            return;
        }
    }
    FAIL() << "Never retired without an injected allocation failure";
}

TEST(PipelineErrors, PreservesBackendErrorAndRetriesWithoutCachingFailure) {
    PipelineDevice device;
    Renderer renderer{{}, device};
    auto primitive = renderer.createPrimitive(pipelineGeometry());
    ASSERT_TRUE(primitive);
    const auto request = pipelineRequest(*primitive);
    device.pipelineError =
        Error{kc::ErrorCode::invalidArgument, "Unsupported pipeline"};
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    EXPECT_EQ(device.recordingError.code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(device.recordingError.message(), "Unsupported pipeline");
    EXPECT_EQ(device.createdPipelines, 0);
    EXPECT_TRUE(device.pipelines.empty());

    device.pipelineError.reset();
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    EXPECT_FALSE(device.recordingError.valid());
    EXPECT_EQ(device.createdPipelines, 1);
    renderer.destroyShader(ShaderHandle{1});
    EXPECT_TRUE(device.pipelines.empty());
    renderer.destroyPrimitive(*primitive);
}

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

TEST(AssetAllocation, GeometryRegistrationKeepsEveryAcceptedCreationOwned) {
    const auto data = GeometryDataBuilder<Vertex3>{}
                          .addVertex(Vec3f{0}, Vec3f{0, 0, 1})
                          .addVertex(Vec3f{1, 0, 0}, Vec3f{0, 0, 1})
                          .addVertex(Vec3f{0, 1, 0}, Vec3f{0, 0, 1})
                          .addIndices({0, 1, 2})
                          .build();
    verifyCreationHandoff(Config{}, [&](AssetSystem& assets) {
        return assets.createGeometry(data);
    });
}

}  // namespace
