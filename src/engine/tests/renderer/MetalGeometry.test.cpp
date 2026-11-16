#include <gtest/gtest.h>

#include <array>

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

TEST(MetalGeometry, FetchesMixedPackedFloatVectors) {
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
            float3 position [[attribute(0)]];
            float2 uv [[attribute(1)]];
            float4 color [[attribute(2)]];
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
    const sl::VertexLayout formats{
        sl::ValueType::f32_3x1, sl::ValueType::f32_2x1, sl::ValueType::f32_4x1
    };
    auto* layout = MTL::VertexDescriptor::vertexDescriptor();
    sl::u64 offset = 0;
    for (size_t i = 0; i < formats.size(); ++i) {
        const auto format = sl::toMetal(formats[i]);
        ASSERT_TRUE(format);
        auto* attribute = layout->attributes()->object(i);
        attribute->setFormat(*format);
        attribute->setOffset(offset);
        attribute->setBufferIndex(sl::metalVertexBufferIndex);
        offset += sl::LayoutHelper::valueTypeSize(formats[i]);
    }
    layout->layouts()->object(sl::metalVertexBufferIndex)->setStride(offset);
    description->setVertexDescriptor(layout);
    auto pipeline = NS::TransferPtr(
        context.device().newRenderPipelineState(description.get(), &error)
    );
    ASSERT_TRUE(pipeline) << sl::parseMetalError(error);

    const std::array<sl::f32, 27> vertices{
        -1, -1, 0, 0.25f, 0.5f, 0, 0, 0.75f, 1,
        3,  -1, 0, 0.25f, 0.5f, 0, 0, 0.75f, 1,
        -1, 3,  0, 0.25f, 0.5f, 0, 0, 0.75f, 1,
    };
    sl::DeviceBufferDescription upload{
        .usage = sl::DeviceBufferUsage::vertexBuffer
    };
    upload.bytes.resize(sizeof(vertices));
    std::memcpy(upload.bytes.data(), vertices.data(), sizeof(vertices));
    const auto buffer = resources.createBuffer(upload);
    ASSERT_TRUE(buffer);
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
    auto* commands = context.commandQueue().commandBuffer();
    auto* nativeEncoder = commands->renderCommandEncoder(pass);
    ASSERT_NE(nativeEncoder, nullptr);
    nativeEncoder->setRenderPipelineState(pipeline.get());
    {
        sl::MetalRenderPassEncoder encoder{resources, nativeEncoder};
        ASSERT_TRUE(encoder.setVertexBuffer({.handle = *buffer, .offset = 0}));
        encoder.draw(3);
    }
    commands->commit();
    commands->waitUntilCompleted();
    ASSERT_EQ(commands->status(), MTL::CommandBufferStatusCompleted);
    std::array<sl::Byte, 4> pixel;
    target->getBytes(pixel.data(), pixel.size(), MTL::Region(0, 0, 1, 1), 0);
    EXPECT_NEAR(pixel[0], 64, 1);
    EXPECT_NEAR(pixel[1], 128, 1);
    EXPECT_NEAR(pixel[2], 191, 1);
    EXPECT_EQ(pixel[3], 255);
    resources.destroyBuffer(*buffer);
}

}  // namespace
