#pragma once


#include <string>
#include "fl/Headers.h"

namespace sl::ai {

class FuzzyController {
public:
    explicit FuzzyController(const std::string& schemaPath) {
		m_fuzzyEngine = fl::FllImporter{}.fromFile(schemaPath);
	}

protected:
    template <typename T>
    void setInputVariable(const std::string& label, T value) {
		m_fuzzyEngine->getInputVariable(label)->setValue(std::move(value));
	}

    template <typename T>
    T getValue(const std::string& label) {
        return m_fuzzyEngine->getOutputVariable(label)->getValue();
    }

    void process() {
		m_fuzzyEngine->process();
    }

private:
	fl::Engine* m_fuzzyEngine;

};
}
