#include "Json.h"

#include <memory>

#include "Logger.h"
#include "error/Errors.hpp"

namespace sl::core {

Json::Value parseJson(const std::string& jsonString) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errors;

    const char* p = jsonString.c_str();
    auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
    if (!reader->parse(p, p + jsonString.size(), &root, &errors))
        throw error::JsonError(error::ErrorCode::InvalidJsonString);
    return root;
}

JsonBuilder::JsonBuilder() {
    reset();
    beginObject();
}

std::string JsonBuilder::asString() {
    endObject();

    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errors;

    if (!Json::parseFromStream(builder, m_jsonStream, &root, &errors)) {
        auto msg = std::string{ errors } + "\n" + m_jsonStream.str();
        SL_WARN(msg);
        throw error::JsonError(error::ErrorCode::InvalidJsonString, msg);
    }

    reset();
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

JsonBuilder& JsonBuilder::beginObject() {
    if (m_shouldInsertComma)
        m_jsonStream << ',';
    m_jsonStream << '{';
    m_shouldInsertComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::beginObject(const std::string& name) {
    if (m_shouldInsertComma)
        m_jsonStream << ',';
    m_jsonStream << '"' << name << "\": {";
    m_shouldInsertComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::endObject() {
    m_jsonStream << '}';
    m_shouldInsertComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::beginArray(const std::string& name) {
    if (m_shouldInsertComma)
        m_jsonStream << ',';
    m_jsonStream << '"' << name << "\": [";
    m_shouldInsertComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::beginArray() {
    if (m_shouldInsertComma)
        m_jsonStream << ',';
    m_jsonStream << '[';
    m_shouldInsertComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::endArray() {
    m_jsonStream << ']';
    m_shouldInsertComma = true;
    return *this;
}

void JsonBuilder::reset() {
    m_jsonStream.clear();
    m_shouldInsertComma = false;
}
}
