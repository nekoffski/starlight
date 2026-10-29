#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "starlight/asset/Texture.hh"
#include "starlight/asset/loaders/ShaderLoader.hh"
#include "starlight/math/Math.hh"
#include "starlight/renderer/frontend/Renderer.hh"

namespace {

TEST(Checkerboard, AlternatesCellsWithOpaqueRgbaPixels) {
    auto image = sl::makeCheckerboard(4, 2);
    ASSERT_TRUE(image);
    EXPECT_EQ(image->pixels.size(), 64);
    EXPECT_EQ(image->pixels[0], sl::Byte(240));
    EXPECT_EQ(image->pixels[8], sl::Byte(32));
    EXPECT_EQ(image->pixels[32], sl::Byte(32));
    EXPECT_EQ(image->pixels[40], sl::Byte(240));
    for (size_t i = 3; i < image->pixels.size(); i += 4) {
        EXPECT_EQ(image->pixels[i], sl::Byte(255));
    }
    EXPECT_FALSE(sl::makeCheckerboard(0, 2));
    EXPECT_FALSE(sl::makeCheckerboard(4, 0));
    EXPECT_FALSE(sl::makeCheckerboard(16385, 1));
}

TEST(ShaderParameters, OwnsColumnMajorMatrixCopy) {
    auto matrix =
        glm::translate(sl::Mat4f{1.0f}, sl::Vec3f{0.25f, -0.5f, 0.75f});
    const auto block = sl::ShaderParameterBlock::fromValue("transform", matrix);
    matrix = sl::Mat4f{0.0f};

    std::array<sl::f32, 16> values;
    ASSERT_EQ(block.bytes.size(), sizeof(values));
    std::memcpy(values.data(), block.bytes.data(), sizeof(values));
    EXPECT_EQ(block.name, "transform");
    EXPECT_FLOAT_EQ(values[0], 1.0f);
    EXPECT_FLOAT_EQ(values[12], 0.25f);
    EXPECT_FLOAT_EQ(values[13], -0.5f);
    EXPECT_FLOAT_EQ(values[14], 0.75f);
    EXPECT_FLOAT_EQ(values[15], 1.0f);
}

// Keep one frame pending so destruction must handle both CPU and GPU lifetime.
class BusyDevice : public sl::RenderDevice {
   public:
    bool alive{true};
    bool busy{true};
    std::vector<std::string> events;

    sl::Result<void> trySubmitFrame(RecordFrame) override {
        EXPECT_TRUE(alive);
        if (std::exchange(busy, false)) {
            events.push_back("busy");
            return sl::Error::unexpected(
                sl::ErrorCode::tooManyFramesInFlight, "busy"
            );
        }
        events.push_back("submit");
        return {};
    }
    void waitIdle() override { events.push_back("idle"); }
    void destroyTexture(sl::TextureHandle) override {
        events.push_back("destroy");
        alive = false;
    }
    sl::Result<void> validateResources(
        const sl::ShaderResourceDependencies&
    ) const override {
        if (not alive) {
            return sl::Error::unexpected(
                sl::ErrorCode::invalidArgument, "released"
            );
        }
        return {};
    }
    sl::Result<sl::SurfaceHandle> attachSurface(
        std::shared_ptr<sl::RenderSurfaceProvider>
    ) override {
        return sl::SurfaceHandle{};
    }
    void destroySurface(sl::SurfaceHandle) override {}
    sl::Result<sl::ShaderHandle> createShader(
        const sl::ShaderDescription&
    ) override {
        return sl::ShaderHandle{};
    }
    void destroyShader(sl::ShaderHandle) override {}
    const sl::ShaderDescription* getShaderDescription(
        sl::ShaderHandle
    ) const override {
        return nullptr;
    }
    sl::Result<sl::GraphicsPipelineHandle> createGraphicsPipeline(
        const sl::GraphicsPipelineDescription&
    ) override {
        return sl::GraphicsPipelineHandle{};
    }
    void destroyGraphicsPipeline(sl::GraphicsPipelineHandle) override {}
    sl::Result<sl::DeviceBufferHandle> createBuffer(
        const sl::DeviceBufferDescription&
    ) override {
        return sl::DeviceBufferHandle{};
    }
    void destroyBuffer(sl::DeviceBufferHandle) override {}
    sl::Result<sl::TextureHandle> createTexture(
        const sl::TextureUploadData&
    ) override {
        return sl::TextureHandle{0};
    }
    sl::Result<sl::SamplerHandle> createSampler(
        const sl::SamplerDescription&
    ) override {
        return sl::SamplerHandle{0};
    }
    void destroySampler(sl::SamplerHandle) override {
        events.push_back("destroy sampler");
    }
};

TEST(TextureLifetime, DrainsAcceptedFramesAndWaitsBeforeRelease) {
    BusyDevice device;
    sl::Renderer renderer{{}, device};
    sl::RenderRequest request;
    request.resources.textures = {sl::TextureHandle{0}};
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    renderer.destroyTexture(sl::TextureHandle{0});
    EXPECT_EQ(
        device.events,
        (std::vector<std::string>{"busy", "submit", "idle", "destroy"})
    );
    EXPECT_FALSE(renderer.submit(request));
    renderer.destroySampler(sl::SamplerHandle{0});
    EXPECT_EQ(device.events.back(), "destroy sampler");
    EXPECT_EQ(device.events[device.events.size() - 2], "idle");
}

TEST(TextureLifetime, SamplerReleaseAlsoDrainsBeforeWaiting) {
    BusyDevice device;
    sl::Renderer renderer{{}, device};
    sl::RenderRequest request;
    request.resources.samplers = {sl::SamplerHandle{0}};
    ASSERT_TRUE(renderer.submit(request));
    renderer.tick();
    renderer.destroySampler(sl::SamplerHandle{0});
    EXPECT_EQ(
        device.events,
        (std::vector<std::string>{"busy", "submit", "idle", "destroy sampler"})
    );
}

// Exercise the public file-loading boundary with independent reflection
// fixtures.
TEST(ShaderReflection, ReadsTablesAndUniformsAndRejectsMalformedLayouts) {
    const auto directory =
        std::filesystem::temp_directory_path() /
        ("starlight-reflection-" +
         std::to_string(
             std::chrono::steady_clock::now().time_since_epoch().count()
         ));
    std::filesystem::create_directory(directory);
    sl::DeferCall cleanup{[&] { std::filesystem::remove_all(directory); }};
    const auto shader = directory / "shader.bin";
    std::ofstream(shader) << "compiled shader bytes";
    const auto metadata = shader.string() + ".json";
    const auto load = [&](const char* layout) {
        std::ofstream(metadata) << layout;
        return sl::ShaderLoader::loadShaderDescription(
            sl::Path{shader.string()}
        );
    };
    auto description = load(R"({"entryPoints":[],"globalScope":{"parameters":[
        {"name":"resources","binding":{"index":0},"type":{"kind":"parameterBlock","elementVarLayout":{"type":{
            "sizes":[{"kind":"uniform","value":512}],"fields":[
            {"binding":{"offset":0},"type":{"kind":"array","elementCount":64,"uniformStride":8,"elementType":{"kind":"resource","baseShape":"texture2D"}}}
        ]}}}},
        {"name":"draw","binding":{"index":1},"type":{"kind":"constantBuffer","elementVarLayout":{"type":{"sizes":[{"kind":"uniform","value":16}],"fields":[]}}}}
    ]}})");
    ASSERT_TRUE(description) << description.error().message();
    ASSERT_EQ(description->resourceTables.size(), 1);
    EXPECT_EQ(description->resourceTables[0].arrays[0].count, 64);
    EXPECT_EQ(description->resourceTables[0].arrays[0].stride, 8);
    EXPECT_EQ(description->uniforms[0].size, 16);
    auto spirv = load(R"({"entryPoints":[],"globalScope":{"parameters":[
        {"name":"resources","binding":{"kind":"subElementRegisterSpace","index":1},"type":{"kind":"parameterBlock","elementVarLayout":{"type":{
            "sizes":[{"kind":"descriptorTableSlot","value":2}],"fields":[
            {"binding":{"index":0},"type":{"kind":"array","elementCount":64,"elementType":{"kind":"resource","baseShape":"texture2D"}}},
            {"binding":{"index":1},"type":{"kind":"array","elementCount":64,"elementType":{"kind":"samplerState"}}}
        ]}}}}
    ]}})");
    ASSERT_TRUE(spirv) << spirv.error().message();
    EXPECT_EQ(spirv->resourceTables[0].binding, 1);
    EXPECT_EQ(spirv->resourceTables[0].arrays[1].binding, 1);
    EXPECT_EQ(
        spirv->resourceTables[0].arrays[1].kind, sl::ShaderResourceKind::sampler
    );
    EXPECT_FALSE(load(
        R"({"entryPoints":[],"globalScope":{"parameters":[{"name":"broken","binding":{"index":0},"type":{"kind":"parameterBlock"}}]}})"
    ));
    EXPECT_FALSE(load(
        R"({"entryPoints":[],"globalScope":{"parameters":[{"name":"negative","binding":{"index":-1},"type":{"kind":"constantBuffer"}}]}})"
    ));
}

}  // namespace
