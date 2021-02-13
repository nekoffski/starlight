#include <gtest/gtest.h>

#include "sl/core/Json.h"
#include "sl/core/error/Errors.hpp"

using namespace sl::core;
using namespace sl::core::error;
using namespace testing;

namespace {

class JsonTests : public Test {
public:
    JsonBuilder jsonBuilder;
};

TEST_F(JsonTests, givenInvalidJsonString_whenParsing_shouldThrow) {
    std::string corruptedJson = "{\"Hello\": {}";
    EXPECT_THROW(parseJson(corruptedJson), JsonError);
}

TEST_F(JsonTests, givenValidJsonString_whenParsing_shouldReturnJsonObject) {
    std::string corruptedJson = "{\"Hello\": \"World\"}";
    auto json = parseJson(corruptedJson);

    ASSERT_TRUE(json.isMember("Hello"));
    EXPECT_EQ(json["Hello"], "World");
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingEmptyJson_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.asString();

    EXPECT_EQ(jsonString, "{}");
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingInvalidJson_shouldThrow) {
    jsonBuilder.beginObject();

    EXPECT_THROW(jsonBuilder.asString(), JsonError);
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingEmptyArray_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.beginArray("my_array").endArray().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("my_array"));
    EXPECT_TRUE(json["my_array"].isArray());
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingNestedJson_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.beginObject("nested").endObject().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("nested"));
    EXPECT_TRUE(json["nested"].isObject());
}

TEST_F(JsonTests, givenJsonBuilder_whenAddingIntegerValue_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.addField("myInteger", 5).asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("myInteger"));

    auto val = json["myInteger"];
    ASSERT_TRUE(val.isInt());
    EXPECT_EQ(val.asInt(), 5);
}

TEST_F(JsonTests, givenJsonBuilder_whenAddingStringValue_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.addField("myFloat", 5.0f).asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("myFloat"));

    auto val = json["myFloat"];
    ASSERT_TRUE(val.isDouble());
    EXPECT_EQ(val.asFloat(), 5.0f);
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayOfObjects_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.beginArray("arr").beginObject().endObject().beginObject().endObject().endArray().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("arr"));

    auto val = json["arr"];
    ASSERT_TRUE(val.isArray());
    EXPECT_TRUE(val[0].isObject());
    EXPECT_TRUE(val[1].isObject());
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayOfArrays_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.beginArray("arr").beginArray().endArray().beginArray().endArray().endArray().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("arr"));

    auto val = json["arr"];
    ASSERT_TRUE(val.isArray());
    EXPECT_TRUE(val[0].isArray());
    EXPECT_TRUE(val[1].isArray());
}

TEST_F(JsonTests, givenJsonBuilder_whenAddingFloatValue_shouldGiveCorrectResult) {
    auto jsonString = jsonBuilder.addField("myStr", "str"s).asString();
    auto json = parseJson(jsonString);

    std::cout << jsonString << '\n';

    ASSERT_TRUE(json.isMember("myStr"));

    auto val = json["myStr"];
    ASSERT_TRUE(val.isString());
    EXPECT_EQ(val.asString(), "str");
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingComplexObject_shouldGiveCorrectResult) {
    auto jsonString =
        jsonBuilder.addField("1", 1).addField("2", 2).beginArray("arr0").endArray().beginObject("nested").beginArray("arr").endArray().endObject().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("1"));
    ASSERT_EQ(json["1"], 1);

    ASSERT_TRUE(json.isMember("2"));
    ASSERT_EQ(json["2"], 2);

    ASSERT_TRUE(json.isMember("arr0"));
    ASSERT_TRUE(json["arr0"].isArray());

    ASSERT_TRUE(json.isMember("nested"));
    auto n = json["nested"];

    ASSERT_TRUE(n.isMember("arr"));
    ASSERT_TRUE(n["arr"].isArray());
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayWithStrElements_shouldGiveCorrectResults) {
    auto jsonString = jsonBuilder.beginArray("arr").addField("a"s).addField("b"s).endArray().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("arr"));
    auto& arr = json["arr"];

    ASSERT_EQ(arr.size(), 2);
    ASSERT_EQ(arr[0], "a"s);
    ASSERT_EQ(arr[1], "b"s);
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayWithIntElements_shouldGiveCorrectResults) {
    auto jsonString = jsonBuilder.beginArray("arr").addField(1).addField(2).endArray().asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("arr"));
    auto& arr = json["arr"];

    ASSERT_EQ(arr.size(), 2);
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[1], 2);
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayFromStringVector_shouldGiveCorrectResults) {
    auto stringVector = std::vector<std::string>{ "abcd"s, "efgh"s };
    auto jsonString = jsonBuilder.addField("array", stringVector).asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("array"));
    auto& arr = json["array"];

    ASSERT_EQ(arr.size(), stringVector.size());
    for (int i = 0; i < stringVector.size(); ++i)
        EXPECT_EQ(arr[i], stringVector[i]);
}

TEST_F(JsonTests, givenJsonBuilder_whenCreatingArrayFromIntVector_shouldGiveCorrectResults) {
    auto intVector = std::vector<int>{ 1, 2, 3, 4 };
    auto jsonString = jsonBuilder.addField("array", intVector).asString();
    auto json = parseJson(jsonString);

    ASSERT_TRUE(json.isMember("array"));
    auto& arr = json["array"];

    ASSERT_EQ(arr.size(), intVector.size());
    for (int i = 0; i < intVector.size(); ++i)
        EXPECT_EQ(arr[i], intVector[i]);
}
}
