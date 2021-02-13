#pragma once

#include <jsoncpp/json/json.h>
#include <sstream>

using namespace std::literals::string_literals;

namespace sl::core {

Json::Value parseJson(const std::string&);

class JsonBuilder {
public:
    explicit JsonBuilder();

    std::string asString();
    JsonBuilder& beginObject();
    JsonBuilder& beginObject(const std::string& name);
    JsonBuilder& endObject();
    JsonBuilder& beginArray();
    JsonBuilder& beginArray(const std::string& name);
    JsonBuilder& endArray();

    JsonBuilder& addField(const std::string& value) {
        return addFieldImpl('"' + value + '"');
    }

    JsonBuilder& addField(const std::string& key, const std::string& value) {
        return addFieldImpl(key, '"' + value + '"');
    }

    template <typename T>
    JsonBuilder& addField(const T& item) {
        return addFieldImpl(item);
    }

    template <typename T>
    JsonBuilder& addField(const std::string& key, const T& value) {
        return addFieldImpl(key, value);
    }

    template <typename T>
    JsonBuilder& addField(const std::string& key, const std::vector<T>& values) {
        beginArray(key);
        for (const auto& value : values)
            addField(value);
        return endArray();
    }

private:
    template <typename T>
    JsonBuilder& addFieldImpl(const T& value) {
        if (m_shouldInsertComma)
            m_jsonStream << ',';

        m_jsonStream << value;
        m_shouldInsertComma = true;
        return *this;
    }

    template <typename T>
    JsonBuilder& addFieldImpl(const std::string& key, const T& value) {
        if (m_shouldInsertComma)
            m_jsonStream << ',';
        m_jsonStream << '"' << key << "\": " << value;
        m_shouldInsertComma = true;
        return *this;
    }

    void reset();

    std::stringstream m_jsonStream;
    bool m_shouldInsertComma;
};
}
