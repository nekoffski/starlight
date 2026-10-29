#include <gtest/gtest.h>

#include "starlight/asset/Texture.hh"
#include "starlight/math/Math.hh"
#include "starlight/platform/metal/MetalRenderPass.hh"

namespace {

TEST(MetalTextures, SamplesNonzeroIndicesAndRejectsStaleHandles) {
    auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());
    auto available = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    if (not available || not available->supportsFamily(MTL::GPUFamilyMetal3)) {
        GTEST_SKIP() << "A Metal 3 device is required";
    }
    sl::MetalContext context;
    sl::MetalResourcePool resources{{}, context};
    auto image = sl::makeCheckerboard(4, 2);
    ASSERT_TRUE(image);
    auto unused = resources.createTexture(*image);
    auto texture = resources.createTexture(*image);
    auto unusedSampler = resources.createSampler({});
    auto sampler = resources.createSampler({});
    ASSERT_TRUE(unused);
    ASSERT_TRUE(texture);
    ASSERT_TRUE(unusedSampler);
    ASSERT_TRUE(sampler);
    ASSERT_EQ(texture->id, 1);
    ASSERT_EQ(sampler->id, 1);
    auto badImage = *image;
    badImage.pixels.pop_back();
    EXPECT_FALSE(resources.createTexture(badImage));

    badImage = *image;
    badImage.width = sl::maxTextureDimension2D + 1;
    EXPECT_FALSE(resources.createTexture(badImage));

    badImage = *image;
    badImage.format = sl::TextureFormat::bgra8unorm;
    EXPECT_FALSE(resources.createTexture(badImage));

    sl::SamplerDescription badSampler;
    badSampler.filter = static_cast<sl::TextureFilter>(-1);
    EXPECT_FALSE(resources.createSampler(badSampler));
    badSampler = {};
    badSampler.addressMode = static_cast<sl::TextureAddressMode>(-1);
    EXPECT_FALSE(resources.createSampler(badSampler));

    // Independent MSL fixture tests the production table encoder without an
    // Xcode toolchain.
    const char* source = R"(
        #include <metal_stdlib>
        using namespace metal;
        struct Resources { array<texture2d<float>, 2> textures; array<sampler, 2> samplers; };
        struct Params { uint textureIndex; uint samplerIndex; uint2 padding; };
        struct Output { float4 position [[position]]; float2 uv; };
        vertex Output vertexMain(uint id [[vertex_id]], constant float4x4& transform [[buffer(2)]]) {
            const float2 positions[3] = {float2(-1,-1),float2(3,-1),float2(-1,3)};
            return {transform * float4(positions[id],0,1),positions[id] * 0.5 + 0.5};
        }
        fragment float4 fragmentMain(Output input [[stage_in]],
            constant Resources& resources [[buffer(0)]], constant Params& draw [[buffer(1)]]) {
            return resources.textures[draw.textureIndex].sample(resources.samplers[draw.samplerIndex],input.uv);
        }
    )";
    auto options = NS::TransferPtr(MTL::CompileOptions::alloc()->init());
    options->setLanguageVersion(MTL::LanguageVersion3_0);
    NS::Error* error = nullptr;
    auto library = NS::TransferPtr(context.device().newLibrary(
        NS::String::string(source, NS::UTF8StringEncoding), options.get(),
        &error
    ));
    ASSERT_TRUE(library) << sl::parseMetalError(error);
    auto vertex = NS::TransferPtr(library->newFunction(
        NS::String::string("vertexMain", NS::UTF8StringEncoding)
    ));
    auto fragment = NS::TransferPtr(library->newFunction(
        NS::String::string("fragmentMain", NS::UTF8StringEncoding)
    ));
    auto pipelineDescription =
        NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    pipelineDescription->setVertexFunction(vertex.get());
    pipelineDescription->setFragmentFunction(fragment.get());
    pipelineDescription->colorAttachments()->object(0)->setPixelFormat(
        MTL::PixelFormatRGBA8Unorm
    );
    auto pipeline = NS::TransferPtr(context.device().newRenderPipelineState(
        pipelineDescription.get(), &error
    ));
    ASSERT_TRUE(pipeline) << sl::parseMetalError(error);
    auto* targetDescription = MTL::TextureDescriptor::texture2DDescriptor(
        MTL::PixelFormatRGBA8Unorm, 4, 4, false
    );
    targetDescription->setUsage(MTL::TextureUsageRenderTarget);
    targetDescription->setStorageMode(MTL::StorageModeShared);
    auto target =
        NS::TransferPtr(context.device().newTexture(targetDescription));
    ASSERT_TRUE(target);
    auto* pass = MTL::RenderPassDescriptor::renderPassDescriptor();
    pass->colorAttachments()->object(0)->setTexture(target.get());
    pass->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    pass->colorAttachments()->object(0)->setClearColor(
        MTL::ClearColor{0, 0, 0, 0}
    );
    pass->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
    auto* commandBuffer = context.commandQueue().commandBuffer();
    auto* nativeEncoder = commandBuffer->renderCommandEncoder(pass);
    ASSERT_NE(nativeEncoder, nullptr);
    nativeEncoder->setRenderPipelineState(pipeline.get());
    sl::ShaderDescription shader;
    shader.uniforms.push_back({1, 16, "draw"});
    shader.uniforms.push_back({2, sizeof(sl::Mat4f), "transform"});
    shader.resourceTables.push_back(
        {"resources",
         0,
         32,
         {{sl::ShaderResourceKind::texture, 2, 0, 8, 0},
          {sl::ShaderResourceKind::sampler, 2, 16, 8, 0}}}
    );
    const std::array<sl::u32, 4> indices{texture->id, sampler->id, 0, 0};
    std::array parameters{
        sl::ShaderParameterBlock::fromValue("draw", indices),
        sl::ShaderParameterBlock::fromValue("transform", sl::Mat4f{1.0f}),
    };
    sl::ShaderResourceDependencies usage{{*texture}, {*sampler}};
    {
        sl::MetalRenderPassEncoder encoder{resources, nativeEncoder};
        EXPECT_FALSE(encoder.setShaderParameters(shader, {}, usage));
        const auto blocks = std::span(parameters);
        auto invalidShader = shader;
        invalidShader.uniforms.front().binding = sl::metalVertexBufferIndex;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        invalidShader = shader;
        invalidShader.resourceTables.front().binding =
            shader.uniforms.front().binding;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        invalidShader = shader;
        invalidShader.resourceTables.front().size =
            sl::metalMaxInlineBufferSize + 1;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        invalidShader = shader;
        invalidShader.resourceTables.front().arrays.front().stride = 0;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        invalidShader = shader;
        invalidShader.resourceTables.front().arrays.front().offset =
            shader.resourceTables.front().size;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        invalidShader = shader;
        invalidShader.resourceTables.front().arrays.front().count = texture->id;
        EXPECT_FALSE(encoder.setShaderParameters(invalidShader, blocks, usage));

        auto result = encoder.setShaderParameters(shader, blocks, usage);
        ASSERT_TRUE(result) << result.error().message();
        encoder.draw(3);
    }
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();
    ASSERT_EQ(commandBuffer->status(), MTL::CommandBufferStatusCompleted);
    std::array<sl::Byte, 64> pixels;
    target->getBytes(pixels.data(), 16, MTL::Region(0, 0, 4, 4), 0);
    EXPECT_TRUE(pixels[0] == sl::Byte(32) || pixels[0] == sl::Byte(240));
    EXPECT_EQ(pixels[0], pixels[1]);
    EXPECT_NE(pixels[0], pixels[8]);
    EXPECT_EQ(pixels[0], pixels[40]);
    EXPECT_EQ(pixels[3], sl::Byte(255));
    const auto translation =
        glm::translate(sl::Mat4f{1.0f}, sl::Vec3f{0.5f, 0.0f, 0.0f});
    const auto modelToClip =
        glm::scale(translation, sl::Vec3f{0.5f, 0.5f, 1.0f});
    parameters.back() =
        sl::ShaderParameterBlock::fromValue("transform", modelToClip);
    auto* transformedCommands = context.commandQueue().commandBuffer();
    auto* transformedEncoder = transformedCommands->renderCommandEncoder(pass);
    ASSERT_NE(transformedEncoder, nullptr);
    transformedEncoder->setRenderPipelineState(pipeline.get());
    {
        sl::MetalRenderPassEncoder encoder{resources, transformedEncoder};
        const auto bound =
            encoder.setShaderParameters(shader, parameters, usage);
        ASSERT_TRUE(bound) << bound.error().message();
        encoder.draw(3);
    }
    transformedCommands->commit();
    transformedCommands->waitUntilCompleted();
    ASSERT_EQ(transformedCommands->status(), MTL::CommandBufferStatusCompleted);
    target->getBytes(pixels.data(), 16, MTL::Region(0, 0, 4, 4), 0);
    const auto alphaAt = [&](size_t x, size_t y) {
        return pixels[(y * image->width + x) * sl::rgba8BytesPerPixel + 3];
    };
    EXPECT_EQ(alphaAt(1, 1), 0);
    EXPECT_EQ(alphaAt(2, 1), 255);
    EXPECT_EQ(alphaAt(2, 3), 0);

    resources.destroyTexture(*texture);
    resources.destroySampler(*sampler);
    EXPECT_FALSE(resources.validateResources(usage));
    auto replacement = resources.createTexture(*image);
    ASSERT_TRUE(replacement);
    EXPECT_EQ(replacement->id, texture->id);
    EXPECT_NE(replacement->generation, texture->generation);
    EXPECT_EQ(resources.getTexture(*texture), nullptr);
    auto replacementSampler = resources.createSampler({});
    ASSERT_TRUE(replacementSampler);
    EXPECT_EQ(replacementSampler->id, sampler->id);
    EXPECT_NE(replacementSampler->generation, sampler->generation);
    EXPECT_EQ(resources.getSampler(*sampler), nullptr);
}

}  // namespace
