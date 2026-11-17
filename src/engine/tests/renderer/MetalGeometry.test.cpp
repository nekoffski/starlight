#include <gtest/gtest.h>

#include <array>

#include "starlight/platform/metal/MetalGraphicsPipeline.hh"
#include "starlight/platform/metal/MetalRenderPass.hh"

namespace {

TEST(MetalGeometry, RejectsUnsupportedVertexFormats) {
    for (auto type :
         {sl::ValueType::u8, sl::ValueType::u16, sl::ValueType::u32,
          sl::ValueType::u64, sl::ValueType::f32, sl::ValueType::f32_4x4,
          static_cast<sl::ValueType>(-1)}) {
        const auto format = sl::toMetal(type);
        ASSERT_FALSE(format);
        EXPECT_EQ(format.error().code(), kc::ErrorCode::invalidArgument);
    }
}

TEST(MetalGeometry, FetchesExplicitLayoutsAndSelectsIndexRanges) {
    auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());
    auto available = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    if (not available) {
        GTEST_SKIP() << "A Metal device is required";
    }
    sl::MetalContext context;
    sl::MetalResourcePool resources{{}, context};
    const char* source = R"(
        #include <metal_stdlib>
        using namespace metal;
        struct Input {
            float3 position [[attribute(7)]];
            float2 uv [[attribute(2)]];
            float4 color [[attribute(5)]];
        };
        struct Output { float4 position [[position]]; float4 color; };
        vertex Output vertexMain(Input input [[stage_in]]) {
            return {float4(input.position, 1), float4(input.uv, input.color.zw)};
        }
        fragment float4 fragmentMain(Output input [[stage_in]]) {
            return input.color;
        }
    )";
    NS::Error* error = nullptr;
    auto library = NS::TransferPtr(context.device().newLibrary(
        NS::String::string(source, NS::UTF8StringEncoding), nullptr, &error
    ));
    ASSERT_TRUE(library) << sl::parseMetalError(error);
    auto vertex = NS::TransferPtr(library->newFunction(
        NS::String::string("vertexMain", NS::UTF8StringEncoding)
    ));
    auto fragment = NS::TransferPtr(library->newFunction(
        NS::String::string("fragmentMain", NS::UTF8StringEncoding)
    ));
    auto description =
        NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    description->setVertexFunction(vertex.get());
    description->setFragmentFunction(fragment.get());
    description->colorAttachments()->object(0)->setPixelFormat(
        MTL::PixelFormatRGBA8Unorm
    );
    const sl::VertexLayout vertexLayout{
        .stride = 40,
        .attributes = {
            {5, sl::ValueType::f32_4x1, 24},
            {2, sl::ValueType::f32_2x1, 16},
            {7, sl::ValueType::f32_3x1, 4},
            {1, sl::ValueType::f32_2x1, 16}
        }
    };
    const auto layout = sl::createMetalVertexDescriptor(vertexLayout);
    ASSERT_TRUE(layout);
    description->setVertexDescriptor(layout->get());
    auto pipeline = NS::TransferPtr(
        context.device().newRenderPipelineState(description.get(), &error)
    );
    ASSERT_TRUE(pipeline) << sl::parseMetalError(error);

    const std::array<sl::f32, 60> vertices{
        99, -1, -1, 0, 0.25f, 0.5f,  0, 0, 0.75f, 1,
        99, 3,  -1, 0, 0.25f, 0.5f,  0, 0, 0.75f, 1,
        99, -1, 3,  0, 0.25f, 0.5f,  0, 0, 0.75f, 1,
        99, -1, -1, 0, 0.75f, 0.25f, 0, 0, 0.5f,  1,
        99, 3,  -1, 0, 0.75f, 0.25f, 0, 0, 0.5f,  1,
        99, -1, 3,  0, 0.75f, 0.25f, 0, 0, 0.5f,  1,
    };
    sl::DeviceBufferDescription upload{
        .usage = sl::DeviceBufferUsage::vertexBuffer
    };
    upload.bytes.resize(sizeof(vertices));
    std::memcpy(upload.bytes.data(), vertices.data(), sizeof(vertices));
    const auto buffer = resources.createBuffer(upload);
    ASSERT_TRUE(buffer);
    // The prefix distinguishes the binding offset from the draw's first index.
    const std::array<sl::u32, 9> indices{3, 4, 5, 0, 1, 2, 3, 4, 5};
    upload.usage = sl::DeviceBufferUsage::indexBuffer;
    upload.bytes.resize(sizeof(indices));
    std::memcpy(upload.bytes.data(), indices.data(), sizeof(indices));
    const auto indexBuffer = resources.createBuffer(upload);
    ASSERT_TRUE(indexBuffer);
    auto* targetDescription = MTL::TextureDescriptor::texture2DDescriptor(
        MTL::PixelFormatRGBA8Unorm, 1, 1, false
    );
    targetDescription->setUsage(MTL::TextureUsageRenderTarget);
    targetDescription->setStorageMode(MTL::StorageModeShared);
    auto target =
        NS::TransferPtr(context.device().newTexture(targetDescription));
    ASSERT_TRUE(target);
    auto* pass = MTL::RenderPassDescriptor::renderPassDescriptor();
    pass->colorAttachments()->object(0)->setTexture(target.get());
    pass->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    pass->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
    for (const auto firstIndex : {0u, 3u}) {
        auto* commands = context.commandQueue().commandBuffer();
        auto* nativeEncoder = commands->renderCommandEncoder(pass);
        ASSERT_NE(nativeEncoder, nullptr);
        nativeEncoder->setRenderPipelineState(pipeline.get());
        {
            sl::MetalRenderPassEncoder encoder{resources, nativeEncoder};
            ASSERT_TRUE(
                encoder.setVertexBuffer({.handle = *buffer, .offset = 0})
            );
            ASSERT_TRUE(encoder.setIndexBuffer(
                {.handle = *indexBuffer, .offset = 3 * sizeof(sl::u32)}
            ));
            encoder.drawIndexed({.indexCount = 3, .firstIndex = firstIndex});
        }
        commands->commit();
        commands->waitUntilCompleted();
        ASSERT_EQ(commands->status(), MTL::CommandBufferStatusCompleted);
        std::array<sl::Byte, 4> pixel;
        target->getBytes(
            pixel.data(), pixel.size(), MTL::Region(0, 0, 1, 1), 0
        );
        EXPECT_NEAR(pixel[0], firstIndex == 0 ? 64 : 191, 1);
        EXPECT_NEAR(pixel[1], firstIndex == 0 ? 128 : 64, 1);
        EXPECT_NEAR(pixel[2], firstIndex == 0 ? 191 : 128, 1);
        EXPECT_EQ(pixel[3], 255);
    }
    resources.destroyBuffer(*indexBuffer);
    resources.destroyBuffer(*buffer);
}

}  // namespace
