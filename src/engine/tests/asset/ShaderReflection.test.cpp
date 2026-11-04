#include "starlight/asset/loaders/ShaderReflection.hh"

#include <gtest/gtest.h>

#include <limits>

#include "starlight/renderer/core/Texture.hh"

namespace {

using kc::Json;
using sl::ShaderReflection;

Json vertexEntryPoint() {
    return Json::parse(R"({
        "name": "vertexMain", "stage": "vertex", "parameters": [
            {"binding": {"index": 7}, "type": {
                "kind": "vector", "elementCount": 3,
                "elementType": {"scalarType": "float32"}
            }},
            {"binding": {"index": 2}, "type": {
                "kind": "vector", "elementCount": 4,
                "elementType": {"scalarType": "float32"}
            }},
            {"name": "vertexID", "type": {"kind": "scalar", "scalarType": "uint32"}}
        ]
    })");
}

Json uniformBlock() {
    return Json::parse(R"({
        "name": "draw", "binding": {"index": 1}, "type": {
            "kind": "constantBuffer", "elementVarLayout": {"type": {
                "sizes": [{"kind": "uniform", "value": 16}], "fields": []
            }}
        }
    })");
}

Json resourceBlock() {
    return Json::parse(R"({
        "name": "resources", "binding": {"index": 0}, "type": {
            "kind": "parameterBlock", "elementVarLayout": {"type": {
                "sizes": [{"kind": "uniform", "value": 1024}], "fields": [
                    {"binding": {"offset": 0}, "type": {
                        "kind": "array", "elementCount": 64, "uniformStride": 8,
                        "elementType": {"kind": "resource", "baseShape": "texture2D"}
                    }},
                    {"binding": {"offset": 512}, "type": {
                        "kind": "array", "elementCount": 64, "uniformStride": 8,
                        "elementType": {"kind": "samplerState"}
                    }}
                ]
            }}
        }
    })");
}

Json metadataWithBlock(Json block) {
    return {
        {"entryPoints", Json::array()},
        {"globalScope", {{"parameters", Json::array({std::move(block)})}}}
    };
}

void expectInvalid(const Json& metadata) {
    SCOPED_TRACE(metadata.dump());
    const auto reflection = ShaderReflection::parse(metadata);
    ASSERT_FALSE(reflection);
    EXPECT_EQ(reflection.error().code(), sl::ErrorCode::shaderParsingError);
}

TEST(ShaderReflection, OwnsEntryPointsAndPreservesVertexInputOrder) {
    Json metadata{
        {"entryPoints", Json::array(
                            {vertexEntryPoint(),
                             {{"name", "fragmentMain"}, {"stage", "fragment"}}}
                        )}
    };
    const auto reflection = ShaderReflection::parse(metadata);
    ASSERT_TRUE(reflection);
    metadata["entryPoints"][0]["name"] = "changed";

    ASSERT_EQ(reflection->stages.size(), 2);
    EXPECT_EQ(
        reflection->stages.at(sl::ShaderStage::vertex).entryPoint, "vertexMain"
    );
    EXPECT_EQ(
        reflection->stages.at(sl::ShaderStage::fragment).entryPoint,
        "fragmentMain"
    );
    ASSERT_TRUE(reflection->vertexLayout);
    EXPECT_EQ(
        *reflection->vertexLayout,
        (sl::VertexLayout{sl::ValueType::f32_3x1, sl::ValueType::f32_4x1})
    );
}

TEST(ShaderReflection, AllowsMissingOptionalGlobalsAndUnknownFields) {
    for (auto metadata :
         {Json{{"entryPoints", Json::array()}},
          Json{{"entryPoints", Json::array()}, {"globalScope", Json::object()}},
          Json{
              {"entryPoints", Json::array()},
              {"globalScope", {{"parameters", Json::array()}}}
          }}) {
        metadata["unparsedField"] = {{"value", "ignored"}};
        const auto reflection = ShaderReflection::parse(metadata);
        ASSERT_TRUE(reflection);
        EXPECT_TRUE(reflection->stages.empty());
        EXPECT_FALSE(reflection->vertexLayout);
        EXPECT_TRUE(reflection->uniforms.empty());
        EXPECT_TRUE(reflection->resourceTables.empty());
    }
}

TEST(ShaderReflection, ReadsUniformAndMetalResourceLayouts) {
    auto metadata = metadataWithBlock(resourceBlock());
    metadata["globalScope"]["parameters"].push_back(uniformBlock());
    const auto reflection = ShaderReflection::parse(metadata);
    ASSERT_TRUE(reflection);
    ASSERT_EQ(reflection->uniforms.size(), 1);
    const auto& uniform = reflection->uniforms.front();
    EXPECT_EQ(uniform.name, "draw");
    EXPECT_EQ(uniform.binding, 1);
    EXPECT_EQ(uniform.size, 16);

    ASSERT_EQ(reflection->resourceTables.size(), 1);
    const auto& table = reflection->resourceTables.front();
    EXPECT_EQ(table.name, "resources");
    EXPECT_EQ(table.binding, 0);
    EXPECT_EQ(table.size, 1024);
    ASSERT_EQ(table.arrays.size(), 2);
    EXPECT_EQ(table.arrays[0].kind, sl::ShaderResourceKind::texture);
    EXPECT_EQ(table.arrays[0].count, 64);
    EXPECT_EQ(table.arrays[0].offset, 0);
    EXPECT_EQ(table.arrays[0].stride, 8);
    EXPECT_EQ(table.arrays[0].binding, 0);
    EXPECT_EQ(table.arrays[1].kind, sl::ShaderResourceKind::sampler);
    EXPECT_EQ(table.arrays[1].count, 64);
    EXPECT_EQ(table.arrays[1].offset, 512);
    EXPECT_EQ(table.arrays[1].stride, 8);
    EXPECT_EQ(table.arrays[1].binding, 0);
}

TEST(ShaderReflection, PreservesSpirvDescriptorBindingsWithoutMetalByteLayout) {
    auto block = resourceBlock();
    block["binding"]["index"] = 3;
    auto& element = block["type"]["elementVarLayout"]["type"];
    element["sizes"] =
        Json::array({{{"kind", "descriptorTableSlot"}, {"value", 2}}});
    for (size_t i = 0; i < element["fields"].size(); ++i) {
        auto& field = element["fields"][i];
        field["binding"] = {{"index", i + 4}};
        field["type"].erase("uniformStride");
    }
    const auto reflection = ShaderReflection::parse(metadataWithBlock(block));
    ASSERT_TRUE(reflection);
    ASSERT_EQ(reflection->resourceTables.size(), 1);
    const auto& table = reflection->resourceTables.front();
    EXPECT_EQ(table.binding, 3);
    EXPECT_EQ(table.size, 0);
    ASSERT_EQ(table.arrays.size(), 2);
    EXPECT_EQ(table.arrays[0].binding, 4);
    EXPECT_EQ(table.arrays[1].binding, 5);
    for (const auto& array : table.arrays) {
        EXPECT_EQ(array.offset, 0);
        EXPECT_EQ(array.stride, 0);
    }
}

TEST(ShaderReflection, RejectsMalformedDocumentsAndOptionalGlobalFields) {
    for (const auto& metadata :
         {Json{}, Json::array(), Json(42), Json::object(),
          Json{{"entryPoints", Json::object()}},
          Json{{"entryPoints", Json::array({nullptr})}}}) {
        expectInvalid(metadata);
    }
    for (const auto& invalid :
         {Json{}, Json::array(), Json(42), Json("wrong")}) {
        expectInvalid(
            {{"entryPoints", Json::array()}, {"globalScope", invalid}}
        );
    }
}

TEST(ShaderReflection, RejectsMalformedGlobalParameterContainers) {
    for (const auto& invalid :
         {Json{}, Json::object(), Json(42), Json("wrong")}) {
        expectInvalid(
            {{"entryPoints", Json::array()},
             {"globalScope", {{"parameters", invalid}}}}
        );
    }
    for (const auto& field : {"sizes", "fields"}) {
        auto block = uniformBlock();
        block["type"]["elementVarLayout"]["type"][field] = Json::object();
        expectInvalid(metadataWithBlock(block));
    }
}

TEST(ShaderReflection, RejectsMalformedEntryPointsAndVertexInputs) {
    for (auto entry :
         {Json{{"name", "vertexMain"}},
          Json{{"name", ""}, {"stage", "fragment"}},
          Json{{"name", 42}, {"stage", "fragment"}},
          Json{{"name", "main"}, {"stage", "unknown"}},
          Json{{"name", "main"}, {"stage", Json::array()}},
          Json{{"name", "main"}, {"stage", "vertex"}}}) {
        expectInvalid({{"entryPoints", Json::array({entry})}});
    }
    for (const auto& parameter :
         {Json{}, Json(42), Json{{"binding", nullptr}},
          Json{{"binding", Json::object()}, {"type", Json::object()}}}) {
        auto entry = vertexEntryPoint();
        entry["parameters"] = Json::array({parameter});
        expectInvalid({{"entryPoints", Json::array({entry})}});
    }
    for (const auto& value :
         {Json(-1), Json(3.5), Json(true), Json("3"),
          Json(std::numeric_limits<sl::u64>::max())}) {
        auto entry = vertexEntryPoint();
        entry["parameters"][0]["type"]["elementCount"] = value;
        expectInvalid({{"entryPoints", Json::array({entry})}});
    }
}

TEST(ShaderReflection, ValidatesBlockNamesKindsAndSizes) {
    for (const auto& field : {"name", "binding", "type"}) {
        auto block = uniformBlock();
        block.erase(field);
        expectInvalid(metadataWithBlock(block));
    }
    auto block = uniformBlock();
    block["name"] = "";
    expectInvalid(metadataWithBlock(block));
    block = uniformBlock();
    block["type"]["kind"] = "unsupported";
    expectInvalid(metadataWithBlock(block));
    block = uniformBlock();
    block["type"]["elementVarLayout"]["type"]["sizes"][0]["value"] = 0;
    expectInvalid(metadataWithBlock(block));

    auto metadata = metadataWithBlock(resourceBlock());
    block = uniformBlock();
    block["name"] = "resources";
    metadata["globalScope"]["parameters"].push_back(block);
    expectInvalid(metadata);
}

TEST(ShaderReflection, ChecksNumericRangesWithoutTruncatingOrWrapping) {
    for (const auto& value :
         {Json(-1), Json(1.5), Json(true), Json("1"),
          Json(sl::u64(std::numeric_limits<sl::u32>::max()) + 1),
          Json(std::numeric_limits<sl::u64>::max())}) {
        auto block = uniformBlock();
        block["binding"]["index"] = value;
        expectInvalid(metadataWithBlock(block));
        block = uniformBlock();
        block["type"]["elementVarLayout"]["type"]["sizes"][0]["value"] = value;
        expectInvalid(metadataWithBlock(block));
        for (const auto& path :
             {"/binding/offset", "/binding/index", "/type/uniformStride",
              "/type/elementCount"}) {
            block = resourceBlock();
            auto& field =
                block["type"]["elementVarLayout"]["type"]["fields"][0];
            field[Json::json_pointer{path}] = value;
            expectInvalid(metadataWithBlock(block));
        }
    }

    auto block = uniformBlock();
    block["binding"]["index"] = std::numeric_limits<sl::u32>::max();
    block["type"]["elementVarLayout"]["type"]["sizes"][0]["value"] =
        std::numeric_limits<sl::u32>::max();
    const auto reflection = ShaderReflection::parse(metadataWithBlock(block));
    ASSERT_TRUE(reflection);
    ASSERT_EQ(reflection->uniforms.size(), 1);
    EXPECT_EQ(
        reflection->uniforms[0].binding, std::numeric_limits<sl::u32>::max()
    );
    EXPECT_EQ(
        reflection->uniforms[0].size, std::numeric_limits<sl::u32>::max()
    );
}

TEST(ShaderReflection, RejectsUnsupportedAndMixedResourceArrays) {
    for (auto count : {0u, sl::resourceTableCapacity + 1}) {
        auto block = resourceBlock();
        block["type"]["elementVarLayout"]["type"]["fields"][0]["type"]
             ["elementCount"] = count;
        expectInvalid(metadataWithBlock(block));
    }
    for (const auto& resource :
         {Json{{"kind", "scalar"}},
          Json{{"kind", "resource"}, {"baseShape", "textureCube"}},
          Json{
              {"kind", "resource"},
              {"baseShape", "texture2D"},
              {"access", "readWrite"}
          }}) {
        auto block = resourceBlock();
        block["type"]["elementVarLayout"]["type"]["fields"][0]["type"]
             ["elementType"] = resource;
        expectInvalid(metadataWithBlock(block));
    }
    auto block = resourceBlock();
    block["type"]["elementVarLayout"]["type"]["fields"][1]["type"]["kind"] =
        "scalar";
    expectInvalid(metadataWithBlock(block));
}

}  // namespace
