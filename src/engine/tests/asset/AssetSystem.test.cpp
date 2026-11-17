#include "starlight/asset/AssetSystem.hh"

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <kcore/Random.hh>
#include <kcore/runtime/ThreadSafeQueue.hh>
#include <limits>
#include <new>
#include <type_traits>

#include "starlight/geometry/GeometryDataBuilder.hh"
#include "starlight/geometry/Vertex.hh"
#include "starlight/renderer/executor/RenderExecutor.hh"
#include "starlight/renderer/frontend/RenderRequestBuilder.hh"
#include "starlight/renderer/frontend/Renderer.hh"

namespace {

using namespace sl;

static_assert(not std::is_constructible_v<ShaderAsset, RendererProxy>);
static_assert(
    not std::is_constructible_v<ShaderAsset, RendererProxy, kc::Path>
);
static_assert(not std::is_constructible_v<TextureAsset, RendererProxy>);
static_assert(not std::is_default_constructible_v<SamplerAsset>);
static_assert(not std::is_constructible_v<GeometryAsset, RendererProxy>);

template <typename Asset>
concept PublicSamplerFactory =
    requires(RendererProxy& proxy, const SamplerDescription& description) {
        Asset::create(proxy, description);
    };
static_assert(not PublicSamplerFactory<SamplerAsset>);

class TestEncoder : public RenderPassEncoder {
   public:
    std::vector<IndexRange> indexedDraws;
    std::vector<DeviceBufferSlice> indexBindings;
    std::vector<std::vector<ShaderParameterBlock>> parameterBindings;

    Result<void> setPipeline(GraphicsPipelineHandle) override { return {}; }
    Result<void> setVertexBuffer(const DeviceBufferSlice&) override {
        return {};
    }
    Result<void> setIndexBuffer(const DeviceBufferSlice& slice) override {
        indexBindings.push_back(slice);
        return {};
    }
    Result<void> setShaderParameters(
        const ShaderDescription&,
        std::span<const ShaderParameterBlock> parameters,
        const ShaderResourceDependencies&
    ) override {
        parameterBindings.emplace_back(parameters.begin(), parameters.end());
        return {};
    }
    void draw(u32) override {}
    void drawIndexed(const IndexedDrawDescription& draw) override {
        indexedDraws.push_back({draw.firstIndex, draw.indexCount});
    }
};

class TestRecorder : public RenderFrameRecorder {
   public:
    TestEncoder encoder;

    Result<void> renderPass(
        const RenderPassDescription&, const RenderPassCallback& record
    ) override {
        return record(encoder);
    }
};

class TestDevice : public RenderDevice {
   public:
    TestRecorder recorder;

    std::unordered_map<ShaderHandle, ShaderDescription> shaders;
    std::unordered_map<TextureHandle, TextureUploadData> textures;
    std::unordered_map<SamplerHandle, SamplerDescription> samplers;
    std::unordered_map<DeviceBufferHandle, DeviceBufferDescription> buffers;
    std::vector<Str> events;
    u32 pipelineCount{0};
    std::vector<GraphicsPipelineDescription> pipelineDescriptions;
    bool busy{false};
    bool failVertexBuffer{false};
    bool failIndexBuffer{false};
    bool throwOnIndexBuffer{false};
    u32 bufferAttempts{0};

    Result<ShaderHandle> createShader(const ShaderDescription& data) override {
        if (data.byteCode == Bytes{'!'}) {
            return Error::unexpected(
                ErrorCode::deviceOperationFailed, "Shader creation failed"
            );
        }
        ShaderHandle handle{++m_shaderId};
        shaders.emplace(handle, data);
        return handle;
    }
    void destroyShader(ShaderHandle handle) override {
        events.push_back("destroy shader");
        shaders.erase(handle);
    }
    const ShaderDescription* getShaderDescription(
        ShaderHandle handle
    ) const override {
        const auto found = shaders.find(handle);
        return found == shaders.end() ? nullptr : &found->second;
    }
    Result<TextureHandle> createTexture(
        const TextureUploadData& data
    ) override {
        if (data.width == 0) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument, "Invalid texture"
            );
        }
        TextureHandle handle{++m_textureId};
        textures.emplace(handle, data);
        return handle;
    }
    void destroyTexture(TextureHandle handle) override {
        events.push_back("destroy texture");
        textures.erase(handle);
    }
    Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription& description
    ) override {
        pipelineDescriptions.push_back(description);
        events.push_back("create pipeline");
        return GraphicsPipelineHandle{++pipelineCount};
    }
    void destroyGraphicsPipeline(GraphicsPipelineHandle handle) override {
        events.push_back("destroy pipeline " + std::to_string(handle.id));
    }
    Result<void> trySubmitFrame(RecordFrame record) override {
        if (std::exchange(busy, false)) {
            events.push_back("busy");
            return Error::unexpected(ErrorCode::tooManyFramesInFlight, "busy");
        }
        const auto r = record(recorder);
        if (r) {
            events.push_back("submit");
        }
        return r;
    }
    void waitIdle() override { events.push_back("idle"); }
    Result<void> validateResources(
        const ShaderResourceDependencies& dependencies
    ) const override {
        for (const auto texture : dependencies.textures) {
            if (not textures.contains(texture)) {
                return Error::unexpected(
                    kc::ErrorCode::invalidArgument, "Stale texture"
                );
            }
        }
        for (const auto sampler : dependencies.samplers) {
            if (not samplers.contains(sampler)) {
                return Error::unexpected(
                    kc::ErrorCode::invalidArgument, "Stale sampler"
                );
            }
        }
        return {};
    }
    Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider>
    ) override {
        return SurfaceHandle{0};
    }
    void destroySurface(SurfaceHandle) override {}
    Result<DeviceBufferHandle> createBuffer(
        const DeviceBufferDescription& description
    ) override {
        ++bufferAttempts;
        const bool indexBuffer =
            description.usage == DeviceBufferUsage::indexBuffer;
        if (indexBuffer && throwOnIndexBuffer) {
            throw std::bad_alloc{};
        }
        if (indexBuffer ? failIndexBuffer : failVertexBuffer) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Injected buffer allocation failure"
            );
        }
        DeviceBufferHandle handle{++m_bufferId};
        buffers.emplace(handle, description);
        return handle;
    }
    void destroyBuffer(DeviceBufferHandle handle) override {
        events.push_back("destroy buffer");
        EXPECT_EQ(buffers.erase(handle), 1);
    }
    Result<SamplerHandle> createSampler(
        const SamplerDescription& description
    ) override {
        const bool supportedFilter =
            description.filter == TextureFilter::nearest ||
            description.filter == TextureFilter::linear;
        const bool supportedAddressMode =
            description.addressMode == TextureAddressMode::repeat ||
            description.addressMode == TextureAddressMode::clampToEdge;
        if (not supportedFilter || not supportedAddressMode) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument, "Invalid sampler"
            );
        }
        SamplerHandle handle{++m_samplerId};
        samplers.emplace(handle, description);
        return handle;
    }
    void destroySampler(SamplerHandle handle) override {
        events.push_back("destroy sampler");
        samplers.erase(handle);
    }

   private:
    u32 m_shaderId{0};
    u32 m_textureId{0};
    u32 m_samplerId{0};
    u32 m_bufferId{0};
};

class Assets : public testing::Test {
   protected:
    void SetUp() override {
        directory = std::filesystem::temp_directory_path() /
                    ("starlight-assets-" + kc::RandomEngine::get().uuid());
        ASSERT_TRUE(std::filesystem::create_directory(directory));
        cfg.paths.assets = kc::Path{directory.string()};
        writeShader('A');
        executor.start();
        assets = std::make_unique<AssetSystem>(cfg, proxy);
    }
    void TearDown() override {
        if (assets) {
            EXPECT_TRUE(assets->shutdown());
            assets.reset();
        }
        executor.stop();
        std::filesystem::remove_all(directory);
    }
    void writeShader(char byte) {
        std::ofstream(directory / "shader.bin", std::ios::binary).put(byte);
        std::ofstream(
            directory / "shader.bin.json"
        ) << R"({"entryPoints":[{"stage":"vertex","name":"vertexMain","parameters":[]}]})";
    }
    void publish() {
        ASSERT_TRUE(proxy.flushRenderer());
        ASSERT_TRUE(assets->update());
        ASSERT_TRUE(proxy.flushRenderer());
    }
    Config cfg;
    TestDevice device;
    Renderer renderer{cfg, device};
    RenderExecutor executor{renderer};
    RendererProxy proxy{executor.createSubmitter()};
    std::unique_ptr<AssetSystem> assets;
    std::filesystem::path directory;
};

GeometryData triangleGeometry() {
    return GeometryDataBuilder<Vertex3>{}
        .addVertex(Vec3f{0, 0, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{1, 0, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{0, 1, 0}, Vec3f{0, 0, 1})
        .addIndices({0, 1, 2})
        .build();
}

GeometryData quadGeometry() {
    return GeometryDataBuilder<Vertex3>{}
        .addVertex(Vec3f{-1, -1, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{1, -1, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{1, 1, 0}, Vec3f{0, 0, 1})
        .addVertex(Vec3f{-1, 1, 0}, Vec3f{0, 0, 1})
        .addIndices({0, 1, 2, 0, 2, 3})
        .build();
}

TEST(RendererProxy, ClosedAndExpiredQueuesRejectEveryOperation) {
    auto queue = std::make_shared<kc::ThreadSafeQueue<RendererCommand>>();
    RendererProxy proxy{RendererCommandSubmitter{queue}};
    queue->close();

    const auto rejects = [](const auto& result) {
        ASSERT_FALSE(result);
        EXPECT_EQ(result.error().code(), ErrorCode::rendererCommandRejected);
    };
    const auto checkOperations = [&] {
        rejects(proxy.createPrimitive({}));
        rejects(proxy.createPrimitiveAsync({}));
        rejects(proxy.destroyPrimitive({}));
        rejects(proxy.createShader({}));
        rejects(proxy.createShaderAsync({}));
        rejects(proxy.destroyShader({}));
        rejects(proxy.getShaderInterface({}));
        rejects(proxy.createTexture({}));
        rejects(proxy.createTextureAsync({}));
        rejects(proxy.destroyTexture({}));
        rejects(proxy.createSampler({}));
        rejects(proxy.createSamplerAsync({}));
        rejects(proxy.destroySampler({}));
        rejects(proxy.createRenderTarget({}));
        rejects(proxy.submit({}));
        rejects(proxy.flushRenderer());
    };
    checkOperations();
    queue.reset();
    checkOperations();
}

TEST(RendererProxy, AsyncCreationOwnsPayloadAndWaitsForRenderThreadReply) {
    auto queue = std::make_shared<kc::ThreadSafeQueue<RendererCommand>>();
    RendererProxy proxy{RendererCommandSubmitter{queue}};
    ShaderDescription description{.byteCode = Bytes{'A'}};
    auto completion = proxy.createShaderAsync(description);
    ASSERT_TRUE(completion);
    description.byteCode.clear();
    EXPECT_EQ(
        completion->wait_for(std::chrono::seconds{0}),
        std::future_status::timeout
    );

    auto command = queue->tryPop();
    ASSERT_TRUE(command);
    auto& creation = std::get<RendererCreateShader>(*command);
    EXPECT_EQ(creation.description.byteCode, Bytes{'A'});
    creation.completion.set_value(ShaderHandle{42});
    auto shader = completion->get();
    ASSERT_TRUE(shader);
    EXPECT_EQ(*shader, ShaderHandle{42});
}

TEST(RendererProxy, SynchronousCallsReturnRenderThreadResults) {
    Config cfg;
    TestDevice device;
    Renderer renderer{cfg, device};
    RenderExecutor executor{renderer};
    RendererProxy proxy{executor.createSubmitter()};
    executor.start();

    auto shader = proxy.createShader({.byteCode = Bytes{'A'}});
    ASSERT_TRUE(shader);
    auto interface = proxy.getShaderInterface(*shader);
    ASSERT_TRUE(interface);
    EXPECT_EQ(interface->handle, *shader);

    auto failedShader = proxy.createShader({.byteCode = Bytes{'!'}});
    ASSERT_FALSE(failedShader);
    EXPECT_EQ(failedShader.error().code(), ErrorCode::deviceOperationFailed);
    EXPECT_EQ(failedShader.error().message(), "Shader creation failed");

    auto failedTexture = proxy.createTexture({});
    ASSERT_FALSE(failedTexture);
    EXPECT_EQ(failedTexture.error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(failedTexture.error().message(), "Invalid texture");

    EXPECT_TRUE(proxy.createRenderTarget({}));
    EXPECT_TRUE(proxy.submit({}));
    EXPECT_TRUE(proxy.destroyShader(*shader));
    ASSERT_TRUE(proxy.flushRenderer());
    EXPECT_TRUE(device.shaders.empty());
}

TEST_F(Assets, GeometryPublishesOwnedBytesAndReleasesAfterLastReference) {
    auto data = triangleGeometry();
    const auto vertexBytes = data.vertexData;
    auto geometry = assets->createGeometry(data);
    ASSERT_TRUE(geometry);
    EXPECT_TRUE(geometry->pending());
    EXPECT_EQ(geometry->handle().error().code(), ErrorCode::assetNotReady);
    auto retained = *geometry;
    data.vertexData.clear();
    data.indexData.clear();
    publish();
    ASSERT_TRUE(geometry->handle());
    EXPECT_EQ(geometry->version(), 1);
    ASSERT_TRUE(retained.handle());
    EXPECT_EQ(*retained.handle(), *geometry->handle());
    ASSERT_EQ(device.buffers.size(), 2);
    const auto vertex =
        std::ranges::find_if(device.buffers, [](const auto& entry) {
            return entry.second.usage == DeviceBufferUsage::vertexBuffer;
        });
    ASSERT_NE(vertex, device.buffers.end());
    EXPECT_EQ(vertex->second.bytes, vertexBytes);
    geometry = GeometryRef{};
    publish();
    EXPECT_EQ(device.buffers.size(), 2);
    EXPECT_EQ(assets->shutdown().error().code(), ErrorCode::assetInUse);
    retained = GeometryRef{};
    EXPECT_EQ(device.buffers.size(), 2);
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_TRUE(device.buffers.empty());
    ASSERT_TRUE(assets->shutdown());
    EXPECT_FALSE(assets->createGeometry(triangleGeometry()));
}

TEST_F(Assets, GeometryCreationIsCollectedAfterReferencesDisappear) {
    {
        auto geometry = assets->createGeometry(triangleGeometry());
        ASSERT_TRUE(geometry);
    }
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_TRUE(device.buffers.empty());
    EXPECT_EQ(
        device.events,
        (std::vector<Str>{"idle", "destroy buffer", "destroy buffer"})
    );
}

TEST_F(
    Assets, GeometryReportsValidationAndAllocationErrorsWithoutLeakingBuffers
) {
    auto invalid = assets->createGeometry({});
    ASSERT_TRUE(invalid);
    publish();
    EXPECT_FALSE(invalid->pending());
    EXPECT_EQ(invalid->handle().error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(invalid->version(), 0);
    EXPECT_EQ(device.bufferAttempts, 0);

    device.failIndexBuffer = true;
    auto failed = assets->createGeometry(triangleGeometry());
    ASSERT_TRUE(failed);
    publish();
    ASSERT_FALSE(failed->handle());
    EXPECT_EQ(failed->error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(failed->error().message(), "Injected buffer allocation failure");
    EXPECT_TRUE(device.buffers.empty());
}

TEST(GeometryUpload, RejectsMalformedUploadsBeforeAllocatingBuffers) {
    TestDevice device;
    RendererResourcePool pool{{}, device};
    std::vector<GeometryData> invalid;
    invalid.emplace_back();
    auto data = triangleGeometry();
    data.vertexLayout.attributes.clear();
    invalid.push_back(data);
    data = triangleGeometry();
    data.vertexCount = 0;
    invalid.push_back(data);
    data = triangleGeometry();
    data.indexCount = 2;
    invalid.push_back(data);
    data = triangleGeometry();
    data.vertexData.pop_back();
    invalid.push_back(data);
    data = triangleGeometry();
    data.indexData.push_back(Byte{0});
    invalid.push_back(data);
    data = triangleGeometry();
    data.vertexLayout.attributes.front().type = static_cast<ValueType>(-1);
    invalid.push_back(data);
    data = triangleGeometry();
    const u32 outOfRange = data.vertexCount;
    std::memcpy(data.indexData.data(), &outOfRange, sizeof(outOfRange));
    invalid.push_back(data);
    data = triangleGeometry();
    data.vertexCount = std::numeric_limits<u32>::max();
    invalid.push_back(data);
    for (const auto& upload : invalid) {
        const auto primitive = pool.createPrimitive(upload);
        ASSERT_FALSE(primitive);
        EXPECT_EQ(primitive.error().code(), kc::ErrorCode::invalidArgument);
    }
    EXPECT_EQ(device.bufferAttempts, 0);
    EXPECT_TRUE(device.buffers.empty());
}

TEST(GeometryUpload, CleansUpPartialCreationAndRemovesDestroyedRecords) {
    TestDevice device;
    RendererResourcePool pool{{}, device};
    device.failVertexBuffer = true;
    EXPECT_FALSE(pool.createPrimitive(triangleGeometry()));
    EXPECT_TRUE(device.buffers.empty());
    device.failVertexBuffer = false;
    device.failIndexBuffer = true;
    EXPECT_FALSE(pool.createPrimitive(triangleGeometry()));
    EXPECT_TRUE(device.buffers.empty());
    device.failIndexBuffer = false;
    device.throwOnIndexBuffer = true;
    EXPECT_THROW(pool.createPrimitive(triangleGeometry()), std::bad_alloc);
    EXPECT_TRUE(device.buffers.empty());
    device.throwOnIndexBuffer = false;
    const auto primitive = pool.createPrimitive(triangleGeometry());
    ASSERT_TRUE(primitive);
    ASSERT_NE(pool.getPrimitive(*primitive), nullptr);
    ASSERT_EQ(device.buffers.size(), 2);
    pool.destroyPrimitive(*primitive);
    EXPECT_EQ(pool.getPrimitive(*primitive), nullptr);
    EXPECT_TRUE(device.buffers.empty());
    const auto events = device.events;
    pool.destroyPrimitive(*primitive);
    EXPECT_EQ(device.events, events);
    const auto next = pool.createPrimitive(triangleGeometry());
    ASSERT_TRUE(next);
    EXPECT_NE(*primitive, *next);
    EXPECT_EQ(pool.getPrimitive(*primitive), nullptr);
    pool.destroyPrimitive(*next);
}

TEST(GeometryUpload, AcceptsMixedPackedFloatVectors) {
    TestDevice device;
    RendererResourcePool pool{{}, device};
    auto data = triangleGeometry();
    constexpr u32 packedStride = (2 + 3 + 4) * sizeof(f32);
    data.vertexLayout = {
        packedStride,
        {{0, ValueType::f32_2x1, 0},
         {1, ValueType::f32_3x1, 8},
         {2, ValueType::f32_4x1, 20}}
    };
    data.vertexData.resize(data.vertexCount * packedStride);
    const auto primitive = pool.createPrimitive(data);
    ASSERT_TRUE(primitive);
    ASSERT_NE(pool.getPrimitive(*primitive), nullptr);
    EXPECT_EQ(pool.getPrimitive(*primitive)->vertexLayout, data.vertexLayout);
    EXPECT_EQ(device.buffers.size(), 2);
    pool.destroyPrimitive(*primitive);
    EXPECT_TRUE(device.buffers.empty());
}

TEST(GeometryUpload, RejectsUnsupportedFormatsBeforeAllocatingBuffers) {
    TestDevice device;
    RendererResourcePool pool{{}, device};
    for (auto format :
         {ValueType::u8, ValueType::u16, ValueType::u32, ValueType::u64,
          ValueType::f32, ValueType::f32_4x4}) {
        auto data = triangleGeometry();
        data.vertexLayout = {
            u32(LayoutHelper::valueTypeSize(format)), {{0, format, 0}}
        };
        data.vertexData.resize(
            data.vertexCount * LayoutHelper::valueTypeSize(format)
        );
        const auto primitive = pool.createPrimitive(data);
        ASSERT_FALSE(primitive);
        EXPECT_EQ(primitive.error().code(), kc::ErrorCode::invalidArgument);
    }
    EXPECT_EQ(device.bufferAttempts, 0);
}

TEST(GeometryUpload, RejectsInvalidExplicitLayoutsBeforeAllocatingBuffers) {
    TestDevice device;
    RendererResourcePool pool{{}, device};
    const auto valid = Vertex3::layout();
    std::vector<VertexLayout> invalid;
    for (auto stride : {0u, 2u, maxVertexStride + 4}) {
        auto layout = valid;
        layout.stride = stride;
        invalid.push_back(layout);
    }
    auto layout = valid;
    layout.attributes[0].location = maxVertexAttributes;
    invalid.push_back(layout);
    layout = valid;
    layout.attributes[1].location = layout.attributes[0].location;
    invalid.push_back(layout);
    for (auto offset : {1u, valid.stride, std::numeric_limits<u32>::max()}) {
        layout = valid;
        layout.attributes[0].offset = offset;
        invalid.push_back(layout);
    }
    for (const auto& badLayout : invalid) {
        auto data = triangleGeometry();
        data.vertexLayout = badLayout;
        const auto primitive = pool.createPrimitive(data);
        ASSERT_FALSE(primitive);
        EXPECT_EQ(primitive.error().code(), kc::ErrorCode::invalidArgument);
    }
    EXPECT_EQ(device.bufferAttempts, 0);
}

TEST(GeometryLayout, ReusesExtraAttributesAndCachesEachPhysicalLayout) {
    TestDevice device;
    Renderer renderer{{}, device};
    auto data = triangleGeometry();
    const auto first = renderer.createPrimitive(data);
    ASSERT_TRUE(first);
    data.vertexLayout.stride += sizeof(f32);
    for (auto& attribute : data.vertexLayout.attributes) {
        attribute.offset += sizeof(f32);
    }
    data.vertexData.resize(data.vertexCount * data.vertexLayout.stride);
    const auto padded = renderer.createPrimitive(data);
    ASSERT_TRUE(padded);
    const auto shader = renderer.createShader(
        {.byteCode = Bytes{'A'},
         .vertexInputs = ShaderVertexInputs{{0, ValueType::f32_3x1}}}
    );
    ASSERT_TRUE(shader);
    RenderRequest request{
        .scene =
            {.items =
                 {{.primitives =
                       {{.primitive = *first},
                        {.primitive = *padded},
                        {.primitive = *first}},
                   .shader = *shader}}},
        .views = {{.target = SurfaceHandle{0}}},
    };
    ASSERT_TRUE(renderer.submit(request));
    renderer.flush();
    ASSERT_EQ(device.pipelineDescriptions.size(), 2);
    EXPECT_EQ(device.pipelineDescriptions[0].vertexLayout, Vertex3::layout());
    EXPECT_EQ(device.pipelineDescriptions[1].vertexLayout, data.vertexLayout);
    EXPECT_EQ(device.pipelineDescriptions[0].shader, *shader);
    EXPECT_EQ(device.pipelineDescriptions[1].shader, *shader);
    ASSERT_TRUE(renderer.submit(request));
    renderer.flush();
    EXPECT_EQ(device.pipelineCount, 2);

    for (auto input :
         {VertexInput{3, ValueType::f32_3x1},
          VertexInput{0, ValueType::f32_4x1}}) {
        device.shaders.at(*shader).vertexInputs = ShaderVertexInputs{input};
        const auto r = renderer.submit(request);
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error().code(), kc::ErrorCode::invalidArgument);
    }
    renderer.destroyPrimitive(*first);
    renderer.destroyPrimitive(*padded);
    renderer.destroyShader(*shader);
}

TEST(
    GeometryLifetime,
    DrainsAcceptedFramesBeforeReleasingBuffersAndRejectsStaleUses
) {
    TestDevice device;
    Renderer renderer{{}, device};
    const auto primitive = renderer.createPrimitive(triangleGeometry());
    const auto shader = renderer.createShader(
        {.byteCode = Bytes{'A'},
         .vertexInputs = ShaderVertexInputs{{0, ValueType::f32_3x1}}}
    );
    ASSERT_TRUE(primitive);
    ASSERT_TRUE(shader);
    RenderRequest request{
        .scene =
            {.items =
                 {{.primitives = {{.primitive = *primitive}},
                   .shader = *shader}}},
        .views = {{.target = SurfaceHandle{0}}},
    };
    auto stale = request;
    ++stale.scene.items.front().primitives.front().primitive.generation;
    EXPECT_FALSE(renderer.submit(stale));
    ASSERT_TRUE(renderer.submit(request));
    device.busy = true;
    ASSERT_TRUE(renderer.tick());
    EXPECT_EQ(device.events, (std::vector<Str>{"busy"}));
    renderer.destroyPrimitive(*primitive);
    EXPECT_EQ(
        device.events, (std::vector<Str>{
                           "busy", "create pipeline", "submit", "idle",
                           "destroy buffer", "destroy buffer"
                       })
    );
    EXPECT_TRUE(device.buffers.empty());
    const auto rejected = renderer.submit(request);
    ASSERT_FALSE(rejected);
    EXPECT_EQ(rejected.error().message(), "Invalid primitive dependency");
    const auto events = device.events;
    renderer.destroyPrimitive(*primitive);
    EXPECT_EQ(device.events, events);
}

TEST(GeometryRanges, RejectsInvalidRangesBeforeAcceptingFrames) {
    TestDevice device;
    Renderer renderer{{}, device};
    const auto primitive = renderer.createPrimitive(quadGeometry());
    const auto shader = renderer.createShader(
        {.byteCode = Bytes{'A'}, .vertexInputs = ShaderVertexInputs{}}
    );
    ASSERT_TRUE(primitive);
    ASSERT_TRUE(shader);
    RenderRequest request{
        .scene =
            {.items =
                 {{.primitives = {{.primitive = *primitive}},
                   .shader = *shader}}},
        .views = {{.target = SurfaceHandle{0}}},
    };
    for (const auto range :
         {IndexRange{0, 0}, IndexRange{1, 3}, IndexRange{0, 2},
          IndexRange{6, 3}, IndexRange{3, 6},
          IndexRange{std::numeric_limits<u32>::max(), 3},
          IndexRange{3, std::numeric_limits<u32>::max()}}) {
        request.scene.items.front().primitives.front().indexRange = range;
        const auto r = renderer.submit(request);
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error().code(), kc::ErrorCode::invalidArgument);
        EXPECT_FALSE(renderer.tick());
    }
    EXPECT_EQ(device.pipelineCount, 0);
    renderer.destroyPrimitive(*primitive);
    renderer.destroyShader(*shader);
}

TEST(GeometryRanges, SharesBuffersAcrossMaterialsAndRetainsAcceptedSections) {
    TestDevice device;
    Renderer renderer{{}, device};
    const auto primitive = renderer.createPrimitive(quadGeometry());
    const auto shader = renderer.createShader({
        .byteCode = Bytes{'A'},
        .vertexInputs = ShaderVertexInputs{{0, ValueType::f32_3x1}},
        .uniforms = {{0, sizeof(u32), "surface"}},
    });
    ASSERT_TRUE(primitive);
    ASSERT_TRUE(shader);
    const auto interface = renderer.getShaderInterface(*shader);
    ASSERT_TRUE(interface);
    const auto left = Material::create(
        *interface,
        {.blocks = {ShaderParameterBlock::fromValue("surface", u32{3})}}
    );
    const auto right = Material::create(
        *interface,
        {.blocks = {ShaderParameterBlock::fromValue("surface", u32{9})}}
    );
    ASSERT_TRUE(left);
    ASSERT_TRUE(right);
    auto request = RenderRequestBuilder{}
                       .addView({.target = SurfaceHandle{0}})
                       .addView({.target = SurfaceHandle{1}})
                       .addDraw(
                           {.material = *left,
                            .primitives =
                                {{.primitive = *primitive,
                                  .indexRange = IndexRange{0, 3}}}}
                       )
                       .addDraw(
                           {.material = *right,
                            .primitives =
                                {{.primitive = *primitive,
                                  .indexRange = IndexRange{3, 3}}}}
                       )
                       .build();
    ASSERT_TRUE(request);
    const auto submitted = *request;
    ASSERT_TRUE(renderer.submit(*request));
    device.busy = true;
    renderer.tick();
    EXPECT_TRUE(device.recorder.encoder.indexedDraws.empty());
    request->scene.items.clear();
    renderer.destroyPrimitive(*primitive);

    const auto& encoder = device.recorder.encoder;
    EXPECT_EQ(
        encoder.indexedDraws,
        (std::vector<IndexRange>{{0, 3}, {3, 3}, {0, 3}, {3, 3}})
    );
    ASSERT_EQ(encoder.indexBindings.size(), 4);
    for (const auto& binding : encoder.indexBindings) {
        EXPECT_EQ(binding.handle, encoder.indexBindings.front().handle);
        EXPECT_EQ(binding.offset, 0);
    }
    ASSERT_EQ(encoder.parameterBindings.size(), 4);
    EXPECT_EQ(
        encoder.parameterBindings[0].front().bytes,
        left->parameters().blocks.front().bytes
    );
    EXPECT_EQ(
        encoder.parameterBindings[1].front().bytes,
        right->parameters().blocks.front().bytes
    );
    EXPECT_EQ(device.bufferAttempts, 2);
    EXPECT_EQ(std::ranges::count(device.events, "destroy buffer"), 2);
    EXPECT_TRUE(device.buffers.empty());
    EXPECT_FALSE(renderer.submit(submitted));
    renderer.destroyShader(*shader);
}

TEST_F(Assets, ReusesPendingShaderPathsAndPublishesToEveryReference) {
    auto shader = assets->loadShader("shader.bin");
    ASSERT_TRUE(shader);
    EXPECT_TRUE(shader->pending());
    EXPECT_EQ(shader->version(), 0);
    EXPECT_EQ(shader->handle().error().code(), ErrorCode::assetNotReady);

    std::filesystem::remove(directory / "shader.bin");
    auto same = assets->loadShader("./shader.bin");
    ASSERT_TRUE(same);
    EXPECT_EQ(*shader, *same);
    publish();
    EXPECT_FALSE(shader->pending());
    EXPECT_EQ(shader->version(), 1);
    ASSERT_TRUE(shader->handle());
    EXPECT_EQ(*shader->handle(), *same->handle());
    EXPECT_EQ(device.shaders.size(), 1);
}

TEST_F(Assets, PreservesSymlinkParentPathsForLoadingAndReloading) {
    const auto pack = directory / "pack";
    std::filesystem::create_directories(pack / "nested");
    std::filesystem::create_directory_symlink(
        pack / "nested", directory / "link"
    );
    std::ofstream(pack / "shader.bin", std::ios::binary).put('B');
    std::filesystem::copy_file(
        directory / "shader.bin.json", pack / "shader.bin.json"
    );

    auto linked = assets->loadShader("link/../shader.bin");
    auto root = assets->loadShader("shader.bin");
    auto same = assets->loadShader("./link//.././shader.bin");
    auto absolute = assets->loadShader(
        kc::Path::join(cfg.paths.assets, "link/../shader.bin")
    );
    ASSERT_TRUE(linked);
    ASSERT_TRUE(root);
    ASSERT_TRUE(same);
    ASSERT_TRUE(absolute);
    EXPECT_EQ(*linked, *same);
    EXPECT_EQ(*linked, *absolute);
    EXPECT_NE(*linked, *root);
    publish();
    EXPECT_EQ(device.shaders.at(*linked->handle()).byteCode, Bytes{'B'});
    EXPECT_EQ(device.shaders.at(*root->handle()).byteCode, Bytes{'A'});

    std::ofstream(pack / "shader.bin", std::ios::binary).put('C');
    ASSERT_TRUE(linked->reload());
    publish();
    EXPECT_EQ(device.shaders.at(*same->handle()).byteCode, Bytes{'C'});
    EXPECT_EQ(device.shaders.at(*root->handle()).byteCode, Bytes{'A'});
}

TEST_F(Assets, CleansUpOnlyAfterTheLastReferenceAndAnUpdate) {
    auto image = makeCheckerboard(4, 2);
    ASSERT_TRUE(image);
    auto texture = assets->createTexture(*image);
    ASSERT_TRUE(texture);
    auto retained = *texture;
    publish();
    const auto handle = *texture->handle();
    texture = TextureRef{};
    publish();
    EXPECT_TRUE(device.textures.contains(handle));
    retained = TextureRef{};
    EXPECT_TRUE(device.textures.contains(handle));
    publish();
    EXPECT_FALSE(device.textures.contains(handle));
}

TEST_F(Assets, CollectsCreationResultsAfterReferencesDisappear) {
    {
        auto texture = assets->createTexture(*makeCheckerboard(4, 2));
        ASSERT_TRUE(texture);
        EXPECT_TRUE(texture->pending());
    }
    publish();
    EXPECT_TRUE(device.textures.empty());
    EXPECT_EQ(device.events, (std::vector<Str>{"idle", "destroy texture"}));
}

TEST_F(Assets, TextureReplacementPreservesIdentityAndPublishesOnlyOnSuccess) {
    auto texture = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(texture);
    auto same = *texture;
    publish();
    const auto original = *texture->handle();
    auto image = makeCheckerboard(8, 2);
    ASSERT_TRUE(texture->replace(*image));
    image->pixels.clear();
    EXPECT_TRUE(texture->pending());
    EXPECT_EQ(*same.handle(), original);
    EXPECT_EQ(same.version(), 1);
    EXPECT_EQ(texture->replace(*image).error().code(), ErrorCode::assetBusy);
    publish();
    EXPECT_EQ(*texture, same);
    EXPECT_EQ(same.version(), 2);
    EXPECT_NE(*same.handle(), original);
    EXPECT_FALSE(device.textures.contains(original));
    EXPECT_EQ(device.textures.at(*same.handle()).width, 8);
    EXPECT_FALSE(device.textures.at(*same.handle()).pixels.empty());

    const auto replacement = *same.handle();
    ASSERT_TRUE(texture->replace({}));
    publish();
    EXPECT_EQ(*same.handle(), replacement);
    EXPECT_EQ(same.version(), 2);
    EXPECT_EQ(same.error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_FALSE(same.pending());
    ASSERT_TRUE(texture->replace(*makeCheckerboard(4, 1)));
    publish();
    EXPECT_EQ(same.version(), 3);
    EXPECT_FALSE(same.error().valid());
}

TEST_F(Assets, FailedInitialCreationCanBeRetriedThroughReplacement) {
    auto texture = assets->createTexture({});
    ASSERT_TRUE(texture);
    publish();
    EXPECT_FALSE(texture->pending());
    EXPECT_EQ(texture->version(), 0);
    EXPECT_EQ(texture->handle().error().code(), kc::ErrorCode::invalidArgument);
    ASSERT_TRUE(texture->replace(*makeCheckerboard(4, 2)));
    publish();
    EXPECT_TRUE(texture->handle());
    EXPECT_EQ(texture->version(), 1);
}

TEST_F(Assets, ReloadReadsShaderAgainAndPreservesTheLastGoodVersionOnFailure) {
    auto shader = assets->loadShader("shader.bin");
    ASSERT_TRUE(shader);
    EXPECT_EQ(shader->reload().error().code(), ErrorCode::assetBusy);
    publish();
    const auto original = *shader->handle();
    writeShader('B');
    ASSERT_TRUE(shader->reload());
    EXPECT_EQ(*shader->handle(), original);
    publish();
    EXPECT_FALSE(device.shaders.contains(original));
    EXPECT_EQ(device.shaders.at(*shader->handle()).byteCode, Bytes{'B'});
    EXPECT_EQ(shader->version(), 2);

    const auto current = *shader->handle();
    writeShader('!');
    ASSERT_TRUE(shader->reload());
    publish();
    EXPECT_EQ(*shader->handle(), current);
    EXPECT_EQ(shader->version(), 2);
    EXPECT_EQ(shader->error().code(), ErrorCode::deviceOperationFailed);
    std::filesystem::remove(directory / "shader.bin.json");
    const auto r = shader->reload();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), kc::ErrorCode::fileNotFound);
    EXPECT_EQ(shader->error().code(), kc::ErrorCode::fileNotFound);
    EXPECT_EQ(*shader->handle(), current);
}

TEST_F(Assets, ShaderReloadKeepsPublishedResourceWhenCreationIsRejected) {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;
    auto queue = std::make_shared<Queue>();
    RendererProxy localProxy{RendererCommandSubmitter{queue}};
    AssetStore<ShaderAsset, ShaderRef> store{
        localProxy, &RendererProxy::destroyShader
    };
    auto shader = store.create(kc::Path::join(cfg.paths.assets, "shader.bin"));
    ASSERT_TRUE(shader);
    auto creation = queue->tryPop();
    ASSERT_TRUE(creation);
    const ShaderHandle original{7, 2};
    std::get<RendererCreateShader>(*creation).completion.set_value(original);
    ASSERT_TRUE(store.update());

    queue->close();
    const auto r = shader->reload();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), ErrorCode::rendererCommandRejected);
    EXPECT_EQ(shader->error().code(), r.error().code());
    EXPECT_EQ(shader->error().message(), r.error().message());
    EXPECT_FALSE(shader->pending());
    EXPECT_EQ(shader->version(), 1);
    EXPECT_EQ(*shader->handle(), original);

    shader = ShaderRef{};
    auto cleanupQueue = std::make_shared<Queue>();
    localProxy = RendererProxy{RendererCommandSubmitter{cleanupQueue}};
    ASSERT_TRUE(store.update());
    const auto release = cleanupQueue->tryPop();
    ASSERT_TRUE(release);
    EXPECT_EQ(std::get<RendererDestroyShader>(*release).handle, original);
}

TEST_F(Assets, ShaderInterfaceFollowsThePublishedVersion) {
    std::ofstream(directory / "shader.bin.json") << R"({
        "entryPoints": [{"stage": "vertex", "name": "vertexMain", "parameters": []}],
        "globalScope": {"parameters": [{
            "name": "transform", "binding": {"index": 0},
            "type": {"kind": "constantBuffer", "elementVarLayout": {"type": {
                "sizes": [{"kind": "uniform", "value": 64}], "fields": []
            }}}
        }]}
    })";
    auto shader = assets->loadShader("shader.bin");
    ASSERT_TRUE(shader);
    EXPECT_EQ(shader->interface().error().code(), ErrorCode::assetNotReady);
    publish();

    const ShaderRef& reference = *shader;
    const auto original = reference.interface();
    ASSERT_TRUE(original);
    EXPECT_EQ(original->handle, *shader->handle());
    ASSERT_EQ(original->parameters.size(), 1);
    EXPECT_EQ(original->parameters.front().name, "transform");
    EXPECT_EQ(original->parameters.front().size, 64);

    writeShader('B');
    ASSERT_TRUE(shader->reload());
    const auto pending = reference.interface();
    ASSERT_TRUE(pending);
    EXPECT_EQ(pending->handle, original->handle);
    ASSERT_EQ(pending->parameters.size(), 1);
    EXPECT_EQ(pending->parameters.front().name, "transform");
    publish();

    const auto replacement = reference.interface();
    ASSERT_TRUE(replacement);
    EXPECT_EQ(replacement->handle, *shader->handle());
    EXPECT_NE(replacement->handle, original->handle);
    EXPECT_TRUE(replacement->parameters.empty());
    // Previously returned snapshots retain their captured layout.
    EXPECT_EQ(original->parameters.front().name, "transform");

    writeShader('!');
    ASSERT_TRUE(shader->reload());
    publish();
    const auto afterFailure = reference.interface();
    ASSERT_TRUE(afterFailure);
    EXPECT_EQ(afterFailure->handle, replacement->handle);
    EXPECT_TRUE(afterFailure->parameters.empty());
}

TEST_F(Assets, DoesNotCacheUnusedShadersOrDeduplicateProceduralTextures) {
    ShaderHandle original;
    {
        auto shader = assets->loadShader("shader.bin");
        ASSERT_TRUE(shader);
        publish();
        original = *shader->handle();
    }
    publish();
    EXPECT_TRUE(device.shaders.empty());
    auto shader = assets->loadShader("shader.bin");
    ASSERT_TRUE(shader);
    publish();
    EXPECT_NE(*shader->handle(), original);

    auto left = assets->createTexture(*makeCheckerboard(4, 2));
    auto right = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(left);
    ASSERT_TRUE(right);
    EXPECT_NE(*left, *right);
    publish();
    EXPECT_NE(*left->handle(), *right->handle());
}

TEST_F(Assets, SamplersShareReferencesAndHaveIndependentLifetimes) {
    auto first = assets->createSampler({});
    auto second = assets->createSampler({});
    auto texture = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(first);
    ASSERT_TRUE(second);
    ASSERT_TRUE(texture);
    EXPECT_TRUE(first->pending());
    EXPECT_EQ(first->version(), 0);
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_FALSE(first->pending());
    EXPECT_EQ(first->version(), 1);
    ASSERT_TRUE(first->handle());
    ASSERT_TRUE(second->handle());
    const auto original = *first->handle();
    RenderRequest request{
        .resources = {.textures = {*texture->handle()}, .samplers = {original}}
    };
    ASSERT_TRUE(proxy.submit(request));
    EXPECT_NE(original, *second->handle());
    EXPECT_EQ(device.samplers.size(), 2);
    auto retained = *first;
    EXPECT_EQ(retained, *first);
    first = SamplerRef{};
    publish();
    EXPECT_TRUE(device.samplers.contains(original));
    retained = SamplerRef{};
    publish();
    EXPECT_FALSE(device.samplers.contains(original));
    EXPECT_FALSE(proxy.submit(request));
    EXPECT_TRUE(device.samplers.contains(*second->handle()));
    EXPECT_TRUE(device.textures.contains(*texture->handle()));
}

TEST_F(Assets, SamplerCreationErrorsAndPendingCleanupUseTheCommonLifecycle) {
    const auto invalidFilter = static_cast<TextureFilter>(-1);
    auto invalid = assets->createSampler({.filter = invalidFilter});
    ASSERT_TRUE(invalid);
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_FALSE(invalid->pending());
    EXPECT_EQ(invalid->handle().error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(invalid->version(), 0);
    invalid = SamplerRef{};
    {
        auto sampler = assets->createSampler({.filter = TextureFilter::linear});
        ASSERT_TRUE(sampler);
        EXPECT_TRUE(sampler->pending());
    }
    ASSERT_TRUE(assets->shutdown());
    EXPECT_TRUE(device.samplers.empty());
    EXPECT_EQ(std::ranges::count(device.events, "destroy sampler"), 1);
    EXPECT_FALSE(assets->createSampler({}));
}

TEST_F(Assets, WaitForPendingPublishesAllAssetsAndReplacementCleanup) {
    auto shader = assets->loadShader("shader.bin");
    auto texture = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(shader);
    ASSERT_TRUE(texture);
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_FALSE(shader->pending());
    EXPECT_FALSE(texture->pending());
    EXPECT_EQ(shader->version(), 1);
    EXPECT_EQ(texture->version(), 1);
    ASSERT_TRUE(shader->handle());
    ASSERT_TRUE(texture->handle());

    const auto original = *texture->handle();
    ASSERT_TRUE(texture->replace(*makeCheckerboard(8, 2)));
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_EQ(texture->version(), 2);
    EXPECT_FALSE(device.textures.contains(original));
    EXPECT_EQ(device.textures.at(*texture->handle()).width, 8);

    texture = TextureRef{};
    shader = ShaderRef{};
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_TRUE(device.textures.empty());
    EXPECT_TRUE(device.shaders.empty());
}

TEST_F(Assets, WaitForPendingCompletesFailedOperationsAndKeepsTheirErrors) {
    auto texture = assets->createTexture({});
    ASSERT_TRUE(texture);
    ASSERT_TRUE(assets->waitForPending());
    EXPECT_FALSE(texture->pending());
    EXPECT_EQ(texture->handle().error().code(), kc::ErrorCode::invalidArgument);
    EXPECT_EQ(texture->error().code(), kc::ErrorCode::invalidArgument);
}

TEST_F(Assets, ShutdownRequiresReferencesReleasedAndDrainsPendingResources) {
    auto texture = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(texture);
    const auto r = assets->shutdown();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), ErrorCode::assetInUse);
    auto sampler = assets->createSampler({});
    ASSERT_TRUE(sampler);
    texture = TextureRef{};
    EXPECT_EQ(assets->shutdown().error().code(), ErrorCode::assetInUse);
    sampler = SamplerRef{};
    ASSERT_TRUE(assets->shutdown());
    EXPECT_TRUE(device.textures.empty());
    EXPECT_TRUE(device.samplers.empty());
    EXPECT_FALSE(assets->createTexture(*makeCheckerboard(4, 2)));
    EXPECT_FALSE(assets->loadShader("shader.bin"));
    EXPECT_TRUE(assets->shutdown());
    EXPECT_TRUE(assets->update());
    EXPECT_TRUE(assets->waitForPending());
}

TEST_F(
    Assets, MovingAReferenceTransfersRetentionAndPendingReplacementIsCleanedUp
) {
    auto texture = assets->createTexture(*makeCheckerboard(4, 2));
    ASSERT_TRUE(texture);
    publish();
    auto retained = std::move(*texture);
    EXPECT_FALSE(texture->handle());
    EXPECT_FALSE(texture->replace({}));
    ASSERT_TRUE(retained.replace(*makeCheckerboard(8, 2)));
    retained = TextureRef{};
    ASSERT_TRUE(assets->shutdown());
    EXPECT_TRUE(device.textures.empty());
    EXPECT_EQ(std::ranges::count(device.events, "destroy texture"), 2);
}

TEST_F(Assets, CpuLoadingErrorsDoNotRegisterAnAsset) {
    const auto missing = assets->loadShader("missing.bin");
    ASSERT_FALSE(missing);
    EXPECT_EQ(missing.error().code(), kc::ErrorCode::fileNotFound);
    std::ofstream(directory / "shader.bin.json") << "{";
    const auto invalid = assets->loadShader("shader.bin");
    ASSERT_FALSE(invalid);
    EXPECT_EQ(invalid.error().code(), kc::ErrorCode::jsonParsingError);
    writeShader('A');
    auto shader = assets->loadShader("shader.bin");
    ASSERT_TRUE(shader);
    publish();
    EXPECT_TRUE(shader->handle());
}

TEST(AssetReference, EmptyReferencesCannotAccessResources) {
    ShaderRef shader;
    TextureRef texture;
    SamplerRef sampler;
    GeometryRef geometry;
    EXPECT_FALSE(geometry.handle());
    EXPECT_FALSE(geometry.pending());
    EXPECT_FALSE(sampler.handle());
    EXPECT_FALSE(shader.handle());
    EXPECT_FALSE(shader.reload());
    EXPECT_EQ(
        shader.interface().error().code(), kc::ErrorCode::invalidArgument
    );
    EXPECT_FALSE(texture.handle());
    EXPECT_FALSE(texture.replace({}));
    EXPECT_FALSE(shader.pending());
    EXPECT_EQ(shader.version(), 0);
    EXPECT_TRUE(shader.error().valid());
}

TEST(GeometryUpload, RejectedCreationDoesNotLeaveAnAssetRegistered) {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;
    auto queue = std::make_shared<Queue>();
    RendererProxy proxy{RendererCommandSubmitter{queue}};
    AssetStore<GeometryAsset, GeometryRef> store{
        proxy, &RendererProxy::destroyPrimitive
    };
    queue->close();
    const auto geometry = store.create(triangleGeometry());
    ASSERT_FALSE(geometry);
    EXPECT_EQ(geometry.error().code(), ErrorCode::rendererCommandRejected);
    EXPECT_FALSE(store.hasReferences());
    EXPECT_TRUE(store.update());
}

struct TestAssetRecord : AssetRecord<TextureHandle> {
    using AssetRecord::adoptCreationFuture;
};

TEST(AssetCreation, BrokenPromiseBecomesAnError) {
    TestAssetRecord record;
    {
        std::promise<Result<TextureHandle>> promise;
        record.adoptCreationFuture(promise.get_future());
    }
    record.poll();
    EXPECT_FALSE(record.pending());
    EXPECT_EQ(record.error().code(), ErrorCode::invalidFuture);
    EXPECT_FALSE(record.handle());
    EXPECT_TRUE(record.empty());
}

TEST(AssetRetirement, RejectedReleaseKeepsItsHandleAndOtherRecordsProgress) {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;
    auto queue = std::make_shared<Queue>();
    RendererProxy proxy{RendererCommandSubmitter{queue}};
    AssetStore<TextureAsset, TextureRef> store{
        proxy, &RendererProxy::destroyTexture
    };
    const auto image = makeCheckerboard(4, 2);
    ASSERT_TRUE(image);

    auto first = store.create(*image);
    ASSERT_TRUE(first);
    auto firstRecord = decltype(store)::weakReference(*first).lock();
    auto creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{7, 2}
    );
    ASSERT_TRUE(store.update());
    ASSERT_EQ(*first->handle(), (TextureHandle{7, 2}));

    ASSERT_TRUE(first->replace(*image));
    creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{9, 1}
    );
    auto second = store.create(*image);
    ASSERT_TRUE(second);
    auto secondRecord = decltype(store)::weakReference(*second).lock();
    creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{8, 1}
    );
    EXPECT_TRUE(second->pending());

    queue->close();
    const auto r = store.update();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), ErrorCode::rendererCommandRejected);
    ASSERT_TRUE(firstRecord->nextRetiredHandle());
    EXPECT_EQ(*firstRecord->nextRetiredHandle(), (TextureHandle{7, 2}));
    EXPECT_EQ(*first->handle(), (TextureHandle{9, 1}));
    EXPECT_FALSE(second->pending());
    EXPECT_EQ(second->version(), 1);
    ASSERT_TRUE(second->handle());
    EXPECT_EQ(*second->handle(), (TextureHandle{8, 1}));

    auto retryQueue = std::make_shared<Queue>();
    proxy = RendererProxy{RendererCommandSubmitter{retryQueue}};
    ASSERT_TRUE(store.update());
    EXPECT_FALSE(firstRecord->nextRetiredHandle());
    const auto release = retryQueue->tryPop();
    ASSERT_TRUE(release);
    EXPECT_EQ(
        std::get<RendererDestroyTexture>(*release).handle, (TextureHandle{7, 2})
    );
    first = TextureRef{};
    second = TextureRef{};
    firstRecord.reset();
    secondRecord.reset();
    EXPECT_TRUE(store.update());
}

TEST(
    ShaderLifetime,
    RetiresMatchingPipelinesAfterAcceptedFramesAndRejectsStaleDraws
) {
    TestDevice device;
    ShaderDescription description{.vertexInputs = ShaderVertexInputs{}};
    const auto shader = device.createShader(description);
    const auto other = device.createShader(description);
    ASSERT_TRUE(shader);
    ASSERT_TRUE(other);
    Renderer renderer{{}, device};
    const auto primitive = renderer.createPrimitive(triangleGeometry());
    ASSERT_TRUE(primitive);
    RenderRequest request{
        .scene =
            {.items =
                 {{.primitives = {{.primitive = *primitive}},
                   .shader = *shader},
                  {.primitives = {{.primitive = *primitive}},
                   .shader = *other}}},
        .views = {{.target = SurfaceHandle{0}}},
    };
    ASSERT_TRUE(renderer.submit(request));
    device.busy = true;
    renderer.tick();
    renderer.destroyShader(*shader);
    EXPECT_EQ(
        device.events,
        (std::vector<Str>{
            "busy", "create pipeline", "create pipeline", "submit", "idle",
            "destroy pipeline 1", "destroy shader"
        })
    );
    EXPECT_FALSE(renderer.submit(request));
    request.scene.items.erase(request.scene.items.begin());
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    EXPECT_EQ(device.pipelineCount, 2);
    renderer.destroyShader(*other);
    EXPECT_EQ(device.events[device.events.size() - 2], "destroy pipeline 2");
    renderer.destroyPrimitive(*primitive);
}

}  // namespace
