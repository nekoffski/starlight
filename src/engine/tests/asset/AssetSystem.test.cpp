#include "starlight/asset/AssetSystem.hh"

#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <kcore/Random.hh>
#include <kcore/runtime/ThreadSafeQueue.hh>

namespace {

using namespace sl;

class TestEncoder : public RenderPassEncoder {
   public:
    Result<void> setPipeline(GraphicsPipelineHandle) override { return {}; }
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
    void drawIndexed(u32, u32) override {}
};

class TestRecorder : public RenderFrameRecorder {
   public:
    Result<void> renderPass(
        const RenderPassDescription&, const RenderPassCallback& record
    ) override {
        TestEncoder encoder;
        return record(encoder);
    }
};

class TestDevice : public RenderDevice {
   public:
    std::unordered_map<ShaderHandle, ShaderDescription> shaders;
    std::unordered_map<TextureHandle, TextureUploadData> textures;
    std::unordered_map<SamplerHandle, SamplerDescription> samplers;
    std::vector<Str> events;
    u32 pipelineCount{0};
    bool busy{false};

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
        const GraphicsPipelineDescription&
    ) override {
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
        TestRecorder recorder;
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
        const DeviceBufferDescription&
    ) override {
        return DeviceBufferHandle{0};
    }
    void destroyBuffer(DeviceBufferHandle) override {}
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
    EXPECT_FALSE(sampler.handle());
    EXPECT_FALSE(shader.handle());
    EXPECT_FALSE(shader.reload());
    EXPECT_FALSE(texture.handle());
    EXPECT_FALSE(texture.replace({}));
    EXPECT_FALSE(shader.pending());
    EXPECT_EQ(shader.version(), 0);
    EXPECT_TRUE(shader.error().valid());
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
    RendererProxy proxy{RenderExecutor::Submitter{queue}};
    AssetStore<TextureAsset, TextureRef> store{
        proxy, &RendererProxy::destroyTexture
    };
    const auto image = makeCheckerboard(4, 2);
    ASSERT_TRUE(image);

    auto firstRecord = std::make_shared<TextureAsset>(proxy);
    ASSERT_TRUE(firstRecord->replace(*image));
    auto creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{7, 2}
    );
    auto first = store.add(firstRecord);
    ASSERT_TRUE(store.update());
    ASSERT_EQ(*first.handle(), (TextureHandle{7, 2}));

    ASSERT_TRUE(first.replace(*image));
    creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{9, 1}
    );
    auto secondRecord = std::make_shared<TextureAsset>(proxy);
    ASSERT_TRUE(secondRecord->replace(*image));
    creation = queue->tryPop();
    ASSERT_TRUE(creation);
    std::get<RendererCreateTexture>(*creation).completion.set_value(
        TextureHandle{8, 1}
    );
    auto second = store.add(secondRecord);
    EXPECT_TRUE(second.pending());

    queue->close();
    const auto r = store.update();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), ErrorCode::rendererCommandRejected);
    ASSERT_TRUE(firstRecord->nextRetiredHandle());
    EXPECT_EQ(*firstRecord->nextRetiredHandle(), (TextureHandle{7, 2}));
    EXPECT_EQ(*first.handle(), (TextureHandle{9, 1}));
    EXPECT_FALSE(second.pending());
    EXPECT_EQ(second.version(), 1);
    ASSERT_TRUE(second.handle());
    EXPECT_EQ(*second.handle(), (TextureHandle{8, 1}));

    auto retryQueue = std::make_shared<Queue>();
    proxy = RendererProxy{RenderExecutor::Submitter{retryQueue}};
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
    ShaderDescription description{.vertexLayout = VertexLayout{}};
    const auto shader = device.createShader(description);
    const auto other = device.createShader(description);
    ASSERT_TRUE(shader);
    ASSERT_TRUE(other);
    Renderer renderer{{}, device};
    RenderRequest request{
        .scene = {.items = {{.shader = *shader}, {.shader = *other}}},
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
}

}  // namespace
