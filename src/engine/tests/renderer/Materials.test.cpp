#include <gtest/gtest.h>

#include <array>

#include "starlight/math/Math.hh"
#include "starlight/renderer/frontend/RenderRequestBuilder.hh"
#include "starlight/renderer/shading/Material.hh"
#include "starlight/renderer/shading/MaterialBuilder.hh"

namespace {

sl::ShaderInterface makeShaderInterface() {
    return {
        .handle = sl::ShaderHandle{7, 2},
        .parameters = {
            {"surface", sizeof(sl::u32)}, {"transform", sizeof(sl::Mat4f)}
        },
    };
}

sl::ShaderParameters surfaceParameters() {
    return {
        .blocks = {sl::ShaderParameterBlock::fromValue("surface", sl::u32{3})}
    };
}

sl::ShaderParameters transformParameters(const sl::Mat4f& transform) {
    return {
        .blocks = {sl::ShaderParameterBlock::fromValue("transform", transform)}
    };
}

sl::Result<sl::RenderRequest> buildRequest(
    const sl::Material& material, sl::ShaderParameters parameters
) {
    return sl::RenderRequestBuilder{}
        .addDraw({.material = material, .parameters = std::move(parameters)})
        .build();
}

TEST(Material, ValidatesMaterialBlocksAndShaderInterface) {
    auto shader = makeShaderInterface();
    EXPECT_TRUE(sl::Material::create(shader, surfaceParameters()));
    EXPECT_TRUE(sl::Material::create(shader, {}));
    EXPECT_FALSE(sl::Material::create({}, {}));

    auto wrongSize = surfaceParameters();
    wrongSize.blocks.front().bytes.clear();
    EXPECT_FALSE(sl::Material::create(shader, wrongSize));

    auto unknown = surfaceParameters();
    unknown.blocks.front().name = "unknown";
    EXPECT_FALSE(sl::Material::create(shader, unknown));

    auto duplicate = surfaceParameters();
    duplicate.blocks.push_back(duplicate.blocks.front());
    EXPECT_FALSE(sl::Material::create(shader, duplicate));

    shader.parameters.push_back(shader.parameters.front());
    EXPECT_FALSE(sl::Material::create(shader, {}));
    shader = makeShaderInterface();
    shader.parameters.front().size = 0;
    EXPECT_FALSE(sl::Material::create(shader, {}));
    shader = makeShaderInterface();
    shader.parameters.front().name.clear();
    EXPECT_FALSE(sl::Material::create(shader, {}));
}

TEST(MaterialBuilder, BuildsIndependentMaterialSnapshots) {
    auto shader = makeShaderInterface();
    auto surface = surfaceParameters().blocks.front();
    sl::MaterialBuilder builder{shader};
    builder.addParameter(surface)
        .addTextureDependency(sl::TextureHandle{3, 1})
        .addSamplerDependency(sl::SamplerHandle{4, 2});
    shader.parameters.clear();
    surface.bytes.clear();

    const auto first = builder.build();
    ASSERT_TRUE(first);
    EXPECT_EQ(first->shaderInterface().handle, sl::ShaderHandle(7, 2));
    EXPECT_EQ(first->shaderInterface().parameters.size(), 2);
    ASSERT_EQ(first->parameters().blocks.size(), 1);
    EXPECT_EQ(
        first->parameters().blocks.front().bytes,
        surfaceParameters().blocks.front().bytes
    );
    EXPECT_EQ(
        first->parameters().resources.textures,
        (std::vector<sl::TextureHandle>{{3, 1}})
    );
    EXPECT_EQ(
        first->parameters().resources.samplers,
        (std::vector<sl::SamplerHandle>{{4, 2}})
    );

    builder.addParameter("transform", sl::Mat4f{1.0f});
    const auto second = builder.build();
    ASSERT_TRUE(second);
    EXPECT_EQ(second->parameters().blocks.size(), 2);
    EXPECT_EQ(first->parameters().blocks.size(), 1);
}

TEST(MaterialBuilder, PreservesFactoryValidation) {
    EXPECT_FALSE(sl::MaterialBuilder(sl::ShaderInterface{}).build());
    EXPECT_FALSE(
        sl::MaterialBuilder(makeShaderInterface())
            .addParameter("surface", sl::u64{3})
            .build()
    );
    EXPECT_FALSE(
        sl::MaterialBuilder(makeShaderInterface())
            .addParameter("unknown", sl::u32{3})
            .build()
    );
    EXPECT_FALSE(
        sl::MaterialBuilder(makeShaderInterface())
            .addParameter("surface", sl::u32{3})
            .addParameter("surface", sl::u32{4})
            .build()
    );
}

TEST(
    RenderRequestBuilder,
    RejectsMissingUnknownWrongSizedAndOverlappingDrawBlocks
) {
    const auto material =
        sl::Material::create(makeShaderInterface(), surfaceParameters());
    ASSERT_TRUE(material);
    const auto missing = buildRequest(*material, {});
    ASSERT_FALSE(missing);
    EXPECT_NE(missing.error().message().find("transform"), std::string::npos);

    auto draw = transformParameters(sl::Mat4f{1.0f});
    EXPECT_TRUE(buildRequest(*material, draw));
    draw.blocks.front().bytes.clear();
    EXPECT_FALSE(buildRequest(*material, draw));

    draw = transformParameters(sl::Mat4f{1.0f});
    draw.blocks.front().name = "unknown";
    EXPECT_FALSE(buildRequest(*material, draw));

    draw = transformParameters(sl::Mat4f{1.0f});
    draw.blocks.push_back(draw.blocks.front());
    EXPECT_FALSE(buildRequest(*material, draw));

    draw = transformParameters(sl::Mat4f{1.0f});
    draw.blocks.push_back(surfaceParameters().blocks.front());
    const auto overlap = buildRequest(*material, draw);
    ASSERT_FALSE(overlap);
    EXPECT_NE(overlap.error().message().find("Duplicate"), std::string::npos);

    const std::array draws{sl::MaterialDraw{.material = *material}};
    EXPECT_FALSE(sl::RenderRequestBuilder{}.addDraw(draws.front()).build());
}

TEST(
    RenderRequestBuilder, PreparationOwnsSnapshotsAndCombinesFrameDependencies
) {
    auto shader = makeShaderInterface();
    auto surface = surfaceParameters();
    surface.resources = {
        .textures = {sl::TextureHandle{3, 1}, sl::TextureHandle{3, 1}},
        .samplers = {sl::SamplerHandle{4, 1}, sl::SamplerHandle{4, 1}},
    };
    const auto material = sl::Material::create(shader, surface);
    ASSERT_TRUE(material);
    shader.parameters.clear();
    surface.blocks.front().bytes.clear();
    surface.resources.textures.clear();

    const auto left =
        glm::translate(sl::Mat4f{1.0f}, sl::Vec3f{-0.5f, 0.0f, 0.0f});
    const auto right =
        glm::translate(sl::Mat4f{1.0f}, sl::Vec3f{0.5f, 0.0f, 0.0f});
    std::array draws{
        sl::MaterialDraw{
            .material = *material,
            .primitives = {sl::PrimitiveHandle{1}},
            .parameters = transformParameters(left),
        },
        sl::MaterialDraw{
            .material = *material,
            .primitives = {sl::PrimitiveHandle{1}},
            .parameters = transformParameters(right),
        },
    };
    draws.front().parameters.resources = {
        .textures =
            {sl::TextureHandle{3, 1}, sl::TextureHandle{5, 2},
             sl::TextureHandle{5, 3}},
        .samplers = {sl::SamplerHandle{4, 1}, sl::SamplerHandle{6}},
    };
    sl::RenderRequest base{
        .scene = {.items = {sl::RenderItem{.shader = sl::ShaderHandle{8}}}},
        .views = {{.target = sl::SurfaceHandle{0}}},
        .clearColor = sl::Vec4f{0.25f},
        .resources = {.textures = {sl::TextureHandle{2}, sl::TextureHandle{2}}},
    };
    const auto prepared = sl::RenderRequestBuilder{base}
                              .addDraw(draws.front())
                              .addDraw(draws.back())
                              .build();
    ASSERT_TRUE(prepared);
    EXPECT_EQ(base.scene.items.size(), 1);
    ASSERT_EQ(prepared->scene.items.size(), 3);
    EXPECT_EQ(prepared->scene.items.front().shader, sl::ShaderHandle{8});
    EXPECT_EQ(prepared->views.size(), 1);
    EXPECT_EQ(prepared->clearColor, base.clearColor);
    EXPECT_EQ(
        prepared->resources.textures,
        (std::vector<sl::TextureHandle>{{2}, {3, 1}, {5, 2}, {5, 3}})
    );
    EXPECT_EQ(
        prepared->resources.samplers,
        (std::vector<sl::SamplerHandle>{{4, 1}, {6}})
    );

    const auto& leftItem = prepared->scene.items[1];
    const auto& rightItem = prepared->scene.items[2];
    EXPECT_EQ(leftItem.shader, sl::ShaderHandle(7, 2));
    EXPECT_EQ(leftItem.primitives, draws.front().primitives);
    EXPECT_EQ(
        leftItem.parameters[0].bytes, surfaceParameters().blocks[0].bytes
    );
    EXPECT_EQ(
        leftItem.parameters[1].bytes, transformParameters(left).blocks[0].bytes
    );
    EXPECT_EQ(
        rightItem.parameters[1].bytes,
        transformParameters(right).blocks[0].bytes
    );

    draws.front().parameters.blocks.front().bytes.clear();
    draws.front().parameters.resources.textures.clear();
    EXPECT_EQ(leftItem.parameters[1].bytes.size(), sizeof(sl::Mat4f));
    EXPECT_EQ(prepared->resources.textures.size(), 4);
    EXPECT_TRUE(sl::RenderRequestBuilder{}.addDraw(draws.back()).build());
}

TEST(RenderRequestBuilder, OwnsDrawDescriptionsBeforeBuild) {
    const auto material =
        sl::Material::create(makeShaderInterface(), surfaceParameters());
    ASSERT_TRUE(material);
    sl::MaterialDraw draw{
        .material = *material,
        .primitives = {sl::PrimitiveHandle{1}},
        .parameters = transformParameters(sl::Mat4f{1.0f}),
    };
    draw.parameters.resources.textures = {sl::TextureHandle{5, 2}};
    sl::RenderRequestBuilder builder;
    builder.addView({.target = sl::SurfaceHandle{0}})
        .setClearColor(sl::Vec4f{0.25f})
        .addDraw(draw);
    draw.primitives.clear();
    draw.parameters.blocks.clear();
    draw.parameters.resources.textures.clear();

    const auto request = builder.build();
    ASSERT_TRUE(request);
    ASSERT_EQ(request->scene.items.size(), 1);
    EXPECT_EQ(
        request->scene.items.front().primitives,
        (std::vector<sl::PrimitiveHandle>{{1}})
    );
    EXPECT_EQ(
        request->scene.items.front().parameters.back().bytes,
        transformParameters(sl::Mat4f{1.0f}).blocks.front().bytes
    );
    EXPECT_EQ(
        request->resources.textures, (std::vector<sl::TextureHandle>{{5, 2}})
    );
    ASSERT_EQ(request->views.size(), 1);
    EXPECT_EQ(
        request->views.front().target, sl::RenderTarget{sl::SurfaceHandle{0}}
    );
    EXPECT_EQ(request->clearColor, sl::Vec4f{0.25f});
}

TEST(RenderRequestBuilder, ReadsMaterialAtEachBuildAndKeepsOldSnapshots) {
    auto material =
        sl::Material::create(makeShaderInterface(), surfaceParameters());
    ASSERT_TRUE(material);
    sl::RenderRequestBuilder builder;
    builder.addDraw({
        .material = *material,
        .parameters = transformParameters(sl::Mat4f{1.0f}),
    });

    const auto updated = sl::MaterialBuilder{makeShaderInterface()}
                             .addParameter("surface", sl::u32{9})
                             .addTextureDependency(sl::TextureHandle{3, 1})
                             .build();
    ASSERT_TRUE(updated);
    *material = *updated;
    const auto first = builder.build();
    ASSERT_TRUE(first);
    EXPECT_EQ(
        first->scene.items.front().parameters.front().bytes,
        sl::ShaderParameterBlock::fromValue("surface", sl::u32{9}).bytes
    );

    const auto replacement = sl::MaterialBuilder{makeShaderInterface()}
                                 .addParameter("surface", sl::u32{12})
                                 .addTextureDependency(sl::TextureHandle{4, 2})
                                 .build();
    ASSERT_TRUE(replacement);
    *material = *replacement;
    const auto second = builder.build();
    ASSERT_TRUE(second);
    EXPECT_EQ(
        second->scene.items.front().parameters.front().bytes,
        sl::ShaderParameterBlock::fromValue("surface", sl::u32{12}).bytes
    );
    EXPECT_EQ(
        first->scene.items.front().parameters.front().bytes,
        sl::ShaderParameterBlock::fromValue("surface", sl::u32{9}).bytes
    );
    EXPECT_EQ(
        first->resources.textures, (std::vector<sl::TextureHandle>{{3, 1}})
    );
    EXPECT_EQ(
        second->resources.textures, (std::vector<sl::TextureHandle>{{4, 2}})
    );
}

TEST(
    RenderRequestBuilder, BuildRejectsInvalidDrawWithoutReturningPartialRequest
) {
    const auto material =
        sl::Material::create(makeShaderInterface(), surfaceParameters());
    ASSERT_TRUE(material);
    sl::RenderRequestBuilder builder;
    builder
        .addDraw({
            .material = *material,
            .parameters = transformParameters(sl::Mat4f{1.0f}),
        })
        .addDraw({.material = *material});
    const auto request = builder.build();
    ASSERT_FALSE(request);
    EXPECT_NE(request.error().message().find("transform"), std::string::npos);
    EXPECT_FALSE(builder.build());
}

TEST(RenderRequestBuilder, PreparedRequestsOutliveCpuMaterials) {
    sl::RenderRequest request;
    {
        auto parameters = surfaceParameters();
        parameters.resources.textures = {sl::TextureHandle{3, 1}};
        const auto material =
            sl::Material::create(makeShaderInterface(), std::move(parameters));
        ASSERT_TRUE(material);
        const std::array draws{sl::MaterialDraw{
            .material = *material,
            .primitives = {sl::PrimitiveHandle{1}},
            .parameters = transformParameters(sl::Mat4f{1.0f}),
        }};
        auto prepared =
            sl::RenderRequestBuilder{}.addDraw(draws.front()).build();
        ASSERT_TRUE(prepared);
        request = std::move(*prepared);
    }

    ASSERT_EQ(request.scene.items.size(), 1);
    const auto& item = request.scene.items.front();
    EXPECT_EQ(item.shader, sl::ShaderHandle(7, 2));
    ASSERT_EQ(item.parameters.size(), 2);
    EXPECT_EQ(
        item.parameters.front().bytes, surfaceParameters().blocks[0].bytes
    );
    EXPECT_EQ(
        item.parameters.back().bytes,
        transformParameters(sl::Mat4f{1.0f}).blocks[0].bytes
    );
    EXPECT_EQ(
        request.resources.textures, (std::vector<sl::TextureHandle>{{3, 1}})
    );
}

TEST(RenderRequestBuilder, AllowsAllBlocksToBeMaterialOwnedOrDrawOwned) {
    auto parameters = surfaceParameters();
    parameters.blocks.push_back(
        transformParameters(sl::Mat4f{1.0f}).blocks.front()
    );
    const auto allMaterial =
        sl::Material::create(makeShaderInterface(), parameters);
    ASSERT_TRUE(allMaterial);
    EXPECT_TRUE(buildRequest(*allMaterial, {}));

    const auto allDraw = sl::Material::create(makeShaderInterface(), {});
    ASSERT_TRUE(allDraw);
    EXPECT_TRUE(buildRequest(*allDraw, parameters));
    const auto noParameters =
        sl::Material::create({.handle = sl::ShaderHandle{0}}, {});
    ASSERT_TRUE(noParameters);
    EXPECT_TRUE(buildRequest(*noParameters, {}));
}

}  // namespace
